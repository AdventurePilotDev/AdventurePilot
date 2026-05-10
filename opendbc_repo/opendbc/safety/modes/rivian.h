#pragma once

#include "opendbc/safety/declarations.h"

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

    // Driver torque
    if (msg->addr == 0x380U) {
      int torque_driver_new = (((msg->data[2] << 4) | (msg->data[3] >> 4))) - 2050U;
      update_sample(&torque_driver, torque_driver_new);
    }

    // Brake pressed
    if (msg->addr == 0x38fU) {
      brake_pressed = (msg->data[2] >> 7) & 1U;
    }
  }

  // ACM_Status (cruise state) - int panda sees it on its bus 2 (ACM-camera bus);
  // ext panda doing FCM intercept sees it on its bus 1
  if ((msg->bus == 1U) || (msg->bus == 2U)) {
    if (msg->addr == 0x100U) {
      const int feature_status = msg->data[2] >> 5U;
      pcm_cruise_check(feature_status == 1);
    }
  }
}

static bool rivian_tx_hook(const CANPacket_t *msg) {
  // Rivian utilizes more torque at low speed to maintain the same lateral accel
  const TorqueSteeringLimits RIVIAN_STEERING_LIMITS = {
    .max_torque = 350,
    .dynamic_max_torque = true,
    .max_torque_lookup = {
      {9., 17., 17.},
      {350, 250, 250},
    },
    .max_rate_up = 3,
    .max_rate_down = 5,
    .max_rt_delta = 125,
    .driver_torque_multiplier = 2,
    .driver_torque_allowance = 100,
    .type = TorqueDriverLimited,
  };

  const LongitudinalLimits RIVIAN_LONG_LIMITS = {
    .max_accel = 200,
    .min_accel = -350,
    .inactive_accel = 0,
  };

  bool tx = true;

  // bus is enforced by TX_MSGS whitelist (bus 0 for ACM-side, bus 2 for FCM intercept)
  // Steering control
  if (msg->addr == 0x120U) {
    int desired_torque = ((msg->data[2] << 3U) | (msg->data[3] >> 5U)) - 1024U;
    bool steer_req = (msg->data[3] >> 4) & 1U;

    if (steer_torque_cmd_checks(desired_torque, steer_req, RIVIAN_STEERING_LIMITS)) {
      tx = false;
    }
  }

  // Longitudinal control
  if (msg->addr == 0x160U) {
    int raw_accel = ((msg->data[2] << 3) | (msg->data[3] >> 5)) - 1024U;
    if (longitudinal_accel_checks(raw_accel, RIVIAN_LONG_LIMITS)) {
      tx = false;
    }
  }

  return tx;
}

static safety_config rivian_init(uint16_t param) {
  // SCCM_WheelTouch: for hiding hold wheel alert
  // VDM_AdasSts: for canceling stock ACC
  // 0x321 = SCCM_WheelTouch, 0x162 = VDM_AdasSts
  // 0x120 (ACM_lkaHbaCmd) is intercepted by the second (FCM) panda; see FLAG_RIVIAN_FCM_INTERCEPT below
  static const CanMsg RIVIAN_TX_MSGS[] = {{0x321, 2, 7, .check_relay = true}, {0x162, 2, 8, .check_relay = true}};
  // 0x160 = ACM_longitudinalRequest
  static const CanMsg RIVIAN_LONG_TX_MSGS[] = {{0x321, 2, 7, .check_relay = true}, {0x160, 0, 5, .check_relay = true}};
  // FCM-intercept (second panda): only the LKA command, on its bus 2 going toward the ACM.
  // check_relay disabled because in dual-intercept the ACM also broadcasts 0x120, so the
  // relay-malfunction detector can't distinguish "FCM still alive" from normal ACM traffic.
  static const CanMsg RIVIAN_FCM_TX_MSGS[] = {{0x120, 2, 8, .check_relay = false}};

  static RxCheck rivian_rx_checks[] = {
    {.msg = {{0x208, 0, 8, 50U, .max_counter = 14U}, { 0 }, { 0 }}},                                                             // ESP_Status (speed)
    {.msg = {{0x150, 0, 7, 50U, .max_counter = 14U}, { 0 }, { 0 }}},                                                             // VDM_PropStatus (gas pedal & 2nd speed)
    {.msg = {{0x380, 0, 5, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},  // EPAS_SystemStatus (driver torque)
    {.msg = {{0x38f, 0, 6, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},   // iBESP2 (brakes)
    {.msg = {{0x100, 2, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},  // ACM_Status (cruise state)
  };

  // FCM intercept (ext panda) only sees ACM_Status on its bus 1; other car-side messages live on the int panda's buses
  static RxCheck rivian_fcm_rx_checks[] = {
    {.msg = {{0x100, 1, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},  // ACM_Status (cruise state)
  };

  bool rivian_longitudinal = false;
  const int FLAG_RIVIAN_FCM_INTERCEPT = 2;
  bool rivian_fcm_intercept = GET_FLAG(param, FLAG_RIVIAN_FCM_INTERCEPT);

  #ifdef ALLOW_DEBUG
    const int FLAG_RIVIAN_LONG_CONTROL = 1;
    rivian_longitudinal = GET_FLAG(param, FLAG_RIVIAN_LONG_CONTROL);
  #endif

  if (rivian_fcm_intercept) {
    return BUILD_SAFETY_CFG(rivian_fcm_rx_checks, RIVIAN_FCM_TX_MSGS);
  }

  // FIXME: cppcheck thinks that rivian_longitudinal is always false. This is not true
  // if ALLOW_DEBUG is defined but cppcheck is run without ALLOW_DEBUG
  // cppcheck-suppress knownConditionTrueFalse
  return rivian_longitudinal ? BUILD_SAFETY_CFG(rivian_rx_checks, RIVIAN_LONG_TX_MSGS) : \
                               BUILD_SAFETY_CFG(rivian_rx_checks, RIVIAN_TX_MSGS);
}

// Block 0x120 (LKA command) from auto-forwarding across the relay on either panda.
// In dual-intercept, only the FCM panda's openpilot-injected 0x120 should reach the ACM —
// the FCM's native 0x120 must not pass through.
static bool rivian_fwd_hook(int bus_num, int addr) {
  SAFETY_UNUSED(bus_num);
  return addr == 0x120;
}

const safety_hooks rivian_hooks = {
  .init = rivian_init,
  .rx = rivian_rx_hook,
  .tx = rivian_tx_hook,
  .fwd = rivian_fwd_hook,
  .get_counter = rivian_get_counter,
  .get_checksum = rivian_get_checksum,
  .compute_checksum = rivian_compute_checksum,
  .get_quality_flag_valid = rivian_get_quality_flag_valid,
};
