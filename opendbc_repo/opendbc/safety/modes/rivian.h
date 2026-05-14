#pragma once

#include "opendbc/safety/declarations.h"

// Set by rivian_init from the SECONDARY_TX param flag. Visible to rivian_tx_hook
// so the ext (FCM-intercept) panda can run a minimal TX filter on 0x110, allowing
// its stream to stay frame-identical to the int panda's.
//
// Why this is safe: the EPAS does 2-of-2 voting — it only acts on 0x110 when both
// pandas' streams match. The int panda runs the full safety gauntlet (rate-up/down,
// angle-error vs angle_meas, max_angle, frequency). The ext panda's job is to be
// a faithful mirror; any per-frame check on the ext panda whose result depends on
// per-panda state (desired_angle_last, angle_meas from the lagged FD-bus rebroadcast)
// can desynchronize and never recover, which would make the EPAS stop acting on
// valid commands. Limiting the ext panda to a max_angle sanity bound + the content-
// agnostic frequency limit removes those drift sources without weakening the safety
// story — a bad command rejected by the int panda still gets ignored by the EPAS
// because the two streams won't match.
//
// See test_rivian.py::TestRivianSecondarySafety::test_angle_cmd_mirrors_int_panda.
static bool rivian_secondary_tx = false;

static uint8_t rivian_get_counter(const CANPacket_t *msg) {
  // Signal: ESP_Status_Counter, VDM_PropStatus_Counter
  return msg->data[1] & 0xFU;
}

static uint32_t rivian_get_checksum(const CANPacket_t *msg) {
  // Signal: ESP_Status_Checksum, VDM_PropStatus_Checksum
  return msg->data[0];
}

static uint8_t _rivian_compute_checksum(const CANPacket_t *msg, uint8_t poly, uint8_t xor_output) {
  int len = GET_LEN(msg);

  uint8_t crc = 0;
  // Skip the checksum byte
  for (int i = 1; i < len; i++) {
    crc ^= msg->data[i];
    for (int j = 0; j < 8; j++) {
      if ((crc & 0x80U) != 0U) {
        crc = (crc << 1) ^ poly;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc ^ xor_output;
}

static uint32_t rivian_compute_checksum(const CANPacket_t *msg) {
  uint8_t chksum = 0;
  if (msg->addr == 0x208U) {
    chksum = _rivian_compute_checksum(msg, 0x1D, 0xB1);
  } else if (msg->addr == 0x150U) {
    chksum = _rivian_compute_checksum(msg, 0x1D, 0x9A);
  } else {
  }
  return chksum;
}

static bool rivian_get_quality_flag_valid(const CANPacket_t *msg) {
  bool valid = false;
  if (msg->addr == 0x208U) {
    valid = ((msg->data[3] >> 3) & 0x3U) == 0x1U;  // ESP_Vehicle_Speed_Q
  } else if (msg->addr == 0x150U) {
    valid = (msg->data[1] >> 6) == 0x1U;  // VDM_VehicleSpeedQ
  } else {
  }
  return valid;
}

static void rivian_rx_hook(const CANPacket_t *msg) {

  if (msg->bus == 0U)  {
    // Vehicle speed
    if (msg->addr == 0x208U) {
      float speed = ((msg->data[6] << 8) | msg->data[7]) * 0.01;
      vehicle_moving = speed > 0.0;
      UPDATE_VEHICLE_SPEED(speed * KPH_TO_MS);
    }

    // Gas pressed and second speed source for variable torque limit
    if (msg->addr == 0x150U) {
      gas_pressed = msg->data[3] | (msg->data[4] & 0xC0U);

      // Disable controls if speeds from VDM and ESP ECUs are too far apart.
      float vdm_speed = ((msg->data[5] << 8) | msg->data[6]) * 0.01 * KPH_TO_MS;
      speed_mismatch_check(vdm_speed);
    }

    // Driver torque (for driver-override detection during angle control)
    if (msg->addr == 0x380U) {
      int torque_driver_new = (((msg->data[2] << 4) | (msg->data[3] >> 4))) - 2050U;
      update_sample(&torque_driver, torque_driver_new);
    }

    // Brake pressed
    if (msg->addr == 0x38fU) {
      brake_pressed = (msg->data[2] >> 7) & 1U;
    }

    // Steering angle measurement (EPAS_AdasStatus.EPAS_InternalSas)
    // Signal: 47|14@0+ (0.1, -819.2) deg — Motorola BE, MSB at byte 5 bit 7,
    // continues into byte 6 bits 7:2 (raw=8192 means 0°).
    if (msg->addr == 0x390U) {
      int angle_meas_new = ((msg->data[5] << 6) | (msg->data[6] >> 2)) - 8192;
      update_sample(&angle_meas, angle_meas_new);
    }
  }

  // Cruise state from stock ACM on bus 2 (our HWP injection goes to bus 0, so bus 2
  // only ever carries stock FeatureStatus values).
  if (msg->bus == 2U) {
    if (msg->addr == 0x100U) {
      const int feature_status = msg->data[2] >> 5U;
      pcm_cruise_check(feature_status == 1);
    }
  }
}

static bool rivian_tx_hook(const CANPacket_t *msg) {
  // External steering-angle limits applied to ACM_SteeringControl (0x110).
  // VM-based path (Tesla pattern): rate derives from MAX_LATERAL_JERK / v² in
  // steer_angle_cmd_checks_vm; absolute angle from MAX_LATERAL_ACCEL / v². The
  // controller-side comfort cap (MAX_ANGLE_RATE) is tighter than the safety's
  // low-speed VM bound, so the controller never produces a frame the safety would
  // reject. Empty rate-up/down lookups disable the v1 (apply_std) path.
  const AngleSteeringLimits RIVIAN_STEERING_LIMITS = {
    .max_angle = 3600,           // 360° absolute (EPAS will fault well below this)
    .angle_deg_to_can = 10.,     // 0.1° resolution
    .max_angle_error = 100,      // 10° tolerance vs measured
    .angle_error_min_speed = 5.0,
    .frequency = 100U,
    .angle_is_curvature = false,
    .enforce_angle_error = true,
    .inactive_angle_is_zero = false,  // when disabled, cmd must track measured angle
  };

  // Vehicle model params for steer_angle_cmd_checks_vm. Mirrors values.py CarSpecs
  // for RIVIAN_R1 — slip_factor = calc_slip_factor(VehicleModel(CP)) over the
  // static CP from CarInterface.get_non_essential_params('RIVIAN_R1'). Matches
  // exactly what the controller's VehicleModel(CP) computes at runtime, so the
  // VM math is bit-identical on both sides.
  const AngleSteeringParams RIVIAN_STEERING_PARAMS = {
    .slip_factor = -0.0005446f,
    .steer_ratio = 16.47f,
    .wheelbase = 3.08f,
  };

  const LongitudinalLimits RIVIAN_LONG_LIMITS = {
    .max_accel = 200,
    .min_accel = -350,
    .inactive_accel = 0,
  };

  bool tx = true;

  // Steering control (ACM_SteeringControl 0x110)
  // ACM_SteeringAngleRequest: 23|15@0+ (0.1, -1638.4) → bits 23:9 big-endian
  // ACM_EacEnabled: 13|2@0+ (1=Enabled)
  //
  // EacEnabled=1 (active): full standard check (rate limits, angle-error).
  // EacEnabled=0 (inactive): only enforce a max_angle sanity bound. The default
  // ±1 inactive-tolerance check in steer_angle_cmd_checks compares desired
  // against angle_meas — but our dual-panda topology has the ext panda's
  // angle_meas built from a rebroadcast of EPAS_InternalSas on the front-object
  // FD bus, which lags the primary-actuator broadcast (carstate's source) by
  // more than ±1 unit. That rejects ~42% of inactive frames on the ext panda.
  // EPAS ignores the angle field when EacEnabled=0, so the strict tracking
  // doesn't gain us anything on this car. The sanity bound keeps the rejection
  // of obvious garbage commands (e.g. all-zero 8-byte frames from other car
  // safety modes, which extract to desired_angle = -16384).
  if (msg->addr == 0x110U) {
    int raw_angle = (msg->data[2] << 7) | (msg->data[3] >> 1);
    int desired_angle = raw_angle - 16384;
    bool steer_control_enabled = ((msg->data[1] >> 4) & 0x3U) == 1U;
    bool out_of_range = (desired_angle > RIVIAN_STEERING_LIMITS.max_angle) ||
                        (desired_angle < -RIVIAN_STEERING_LIMITS.max_angle);
    if (rivian_secondary_tx) {
      // EPAS does 2-of-2 voting; int panda is the safety gatekeeper. Keep ext
      // panda's stream byte-identical to the int panda's by avoiding any check
      // whose result depends on per-panda state (desired_angle_last, angle_meas).
      // See the rivian_secondary_tx declaration comment for full reasoning.
      if (out_of_range) {
        tx = false;
      }
      // Gate rt-rate-cap on steer_control_enabled to mirror the int panda's
      // behavior (its rate cap lives inside steer_angle_cmd_checks_vm's active
      // branch). If we rate-cap inactive frames here but int panda doesn't,
      // a controller-side flood at EacEnabled=0 would diverge the two streams
      // and EPAS's 2-of-2 voter would fault on counter mismatch — same failure
      // mode as the double-count bug, just triggered differently.
      if (steer_control_enabled && rt_angle_rate_limit_check(RIVIAN_STEERING_LIMITS)) {
        tx = false;
      }
    } else {
      // Int panda: steer_angle_cmd_checks_vm handles the rate-limit check
      // internally (and only when active) — do NOT call rt_angle_rate_limit_check
      // again here or rt_angle_msgs increments twice per active TX, trips the
      // 120%-of-frequency cap, and rejects ~50% of frames during engagement
      // (EPAS then sees a counter gap and reports AngCtrlCntr).
      if (steer_angle_cmd_checks_vm(desired_angle, steer_control_enabled, RIVIAN_STEERING_LIMITS, RIVIAN_STEERING_PARAMS)) {
        tx = false;
      }
    }
  }

  // Longitudinal control
  if (msg->bus == 0U) {
    if (msg->addr == 0x160U) {
      int raw_accel = ((msg->data[2] << 3) | (msg->data[3] >> 5)) - 1024U;
      if (longitudinal_accel_checks(raw_accel, RIVIAN_LONG_LIMITS)) {
        tx = false;
      }
    }
  }

  return tx;
}

static safety_config rivian_init(uint16_t param) {
  // Int panda (primary actuator relay-cut):
  //   0x321 = SCCM_WheelTouch (bus 2 → ACM side, hides hold-wheel alert)
  //   0x162 = VDM_AdasSts     (bus 2 → ACM side, cancels stock ACC)
  //   0x110 = ACM_SteeringControl (bus 0 → car side, external angle to EPAS)
  //   0x100 = ACM_Status      (bus 0 → car side, FeatureStatus=HWP enable for EPAS)
  static const CanMsg RIVIAN_TX_MSGS[] = {
    {0x321, 2, 7, .check_relay = true},
    {0x162, 2, 8, .check_relay = true},
    {0x110, 0, 8, .check_relay = true},
    {0x100, 0, 8, .check_relay = true},
  };
  // 0x160 = ACM_longitudinalRequest (bus 0 → VDM)
  static const CanMsg RIVIAN_LONG_TX_MSGS[] = {
    {0x321, 2, 7, .check_relay = true},
    {0x160, 0, 5, .check_relay = true},
    {0x110, 0, 8, .check_relay = true},
    {0x100, 0, 8, .check_relay = true},
  };

  // Ext panda (front-object FD bus relay-cut): mirrors only 0x110 + 0x100 so the
  // FCM-side consumers see the same EAC-enable + HWP-status the int panda asserts
  // toward the EPAS.
  static const CanMsg RIVIAN_EXT_TX_MSGS[] = {
    {0x110, 0, 8, .check_relay = true},
    {0x100, 0, 8, .check_relay = true},
  };

  static RxCheck rivian_rx_checks[] = {
    {.msg = {{0x208, 0, 8, 50U, .max_counter = 14U}, { 0 }, { 0 }}},                                                             // ESP_Status (speed)
    {.msg = {{0x150, 0, 7, 50U, .max_counter = 14U}, { 0 }, { 0 }}},                                                             // VDM_PropStatus (gas pedal & 2nd speed)
    {.msg = {{0x380, 0, 5, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},  // EPAS_SystemStatus (driver torque)
    {.msg = {{0x38f, 0, 6, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},   // iBESP2 (brakes)
    {.msg = {{0x100, 2, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},  // ACM_Status (cruise state)
    {.msg = {{0x390, 0, 7, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},  // EPAS_AdasStatus (angle measurement)
  };

  // Ext panda taps the front-object FD bus, which still carries ESP/ACM/EPAS broadcasts.
  static RxCheck rivian_ext_rx_checks[] = {
    {.msg = {{0x208, 0, 8, 50U, .max_counter = 14U}, { 0 }, { 0 }}},
    {.msg = {{0x100, 2, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{0x390, 0, 7, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  };

  bool rivian_longitudinal = false;
  const int FLAG_RIVIAN_SECONDARY_TX = 2;
  rivian_secondary_tx = GET_FLAG(param, FLAG_RIVIAN_SECONDARY_TX);

  #ifdef ALLOW_DEBUG
    const int FLAG_RIVIAN_LONG_CONTROL = 1;
    rivian_longitudinal = GET_FLAG(param, FLAG_RIVIAN_LONG_CONTROL);
  #endif

  if (rivian_secondary_tx) {
    return BUILD_SAFETY_CFG(rivian_ext_rx_checks, RIVIAN_EXT_TX_MSGS);
  }

  // FIXME: cppcheck thinks that rivian_longitudinal is always false. This is not true
  // if ALLOW_DEBUG is defined but cppcheck is run without ALLOW_DEBUG
  // cppcheck-suppress knownConditionTrueFalse
  return rivian_longitudinal ? BUILD_SAFETY_CFG(rivian_rx_checks, RIVIAN_LONG_TX_MSGS) : \
                               BUILD_SAFETY_CFG(rivian_rx_checks, RIVIAN_TX_MSGS);
}

const safety_hooks rivian_hooks = {
  .init = rivian_init,
  .rx = rivian_rx_hook,
  .tx = rivian_tx_hook,
  .get_counter = rivian_get_counter,
  .get_checksum = rivian_get_checksum,
  .compute_checksum = rivian_compute_checksum,
  .get_quality_flag_valid = rivian_get_quality_flag_valid,
};
