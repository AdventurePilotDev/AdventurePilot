#pragma once

#include "opendbc/safety/declarations.h"

// Forward declaration: defined in safety.h, included after mode headers
static void stock_ecu_check(bool stock_ecu_detected);

static uint8_t rivian_get_counter(const CANPacket_t *msg) {
  // Signal: ESP_Status_Counter, VDM_PropStatus_Counter, VDM_AdasSts_Counter
  return msg->data[1] & 0xFU;
}

static uint32_t rivian_get_checksum(const CANPacket_t *msg) {
  // Signal: ESP_Status_Checksum, VDM_PropStatus_Checksum, VDM_AdasSts_Checksum
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
  } else if (msg->addr == 0x162U) {
    chksum = _rivian_compute_checksum(msg, 0x1D, 0xD1);
  } else {
  }
  return chksum;
}

static uint8_t rivian_prev_user_adas_request = 0U;

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

    // VDM_AdasSts: stalk position — used to manage MADS lateral state
    if (msg->addr == 0x162U) {
      const uint8_t user_adas_request = msg->data[7] & 0x7U;

      // UP_1 (value 1) is the MADS toggle gesture. Drive mads_button_press so
      // the panda MADS state machine can grant controls_allowed_lateral for Mode B
      // without requiring ACC to be active.
      mads_button_press = (user_adas_request == 1U) ? MADS_BUTTON_PRESSED : MADS_BUTTON_NOT_PRESSED;

      // UP_2 (value 2, past detent): do not force-disengage here. Python suppresses
      // pcmEnable via altButton2 to prevent unintended MADS engagement from disengaged.
      // Forcing mads_exit_controls() would clear controls_allowed_lateral while Python
      // MADS stays active, causing a lateral mismatch and immediate red screen.

      rivian_prev_user_adas_request = user_adas_request;
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

  if (msg->bus == 2U) {
    // Cruise state — also drives mads_state_update() via stock_ecu_check so that
    // controls_allowed_lateral is updated every time ACM_Status arrives (100 Hz).
    // acc_main_on is left false: lateral is not tied to ACC state, allowing Mode B
    // (MADS active without ACC) to work correctly for Keep Active / Pause modes.
    if (msg->addr == 0x100U) {
      const int feature_status = msg->data[2] >> 5U;
      pcm_cruise_check(feature_status == 1);
      stock_ecu_check(false);
    }
  }
}

static bool rivian_tx_hook(const CANPacket_t *msg) {
  const TorqueSteeringLimits RIVIAN_STEERING_LIMITS = {
    .max_torque = 385,
    .dynamic_max_torque = true,
    // 3-point envelope around the carcontroller's 4-point lookup
    // ([9,13,25,27]->[385,350,295,275]). Safety must permit anything the software
    // may send: this curve sits >= software at every speed (verified 9-27 m/s).
    .max_torque_lookup = {
      {9., 25., 27.},
      {385, 295, 275},
    },
    .max_rate_up = 3,
    .max_rate_down = 5,
    .max_rt_delta = 125,
    .driver_torque_multiplier = 2,
    .driver_torque_allowance = 100,
    .type = TorqueDriverLimited,
    // 2-frame blip: openpilot sends torque=0 and steer_req=0; panda holds last torque for rate limit
    .min_valid_request_frames = 89,
    .max_invalid_request_frames = 2,
    .min_valid_request_rt_interval = 810000,  // 810ms min between blips
    .has_steer_req_tolerance = true,
  };

  const LongitudinalLimits RIVIAN_LONG_LIMITS = {
    .max_accel = 200,
    .min_accel = -350,
    .inactive_accel = 0,
  };

  bool tx = true;

  if (msg->bus == 0U) {
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
  }

  return tx;
}

static safety_config rivian_init(uint16_t param) {
  // SCCM_WheelTouch: for hiding hold wheel alert
  // VDM_AdasSts: for canceling stock ACC
  // 0x120 = ACM_lkaHbaCmd, 0x321 = SCCM_WheelTouch, 0x162 = VDM_AdasSts
  static const CanMsg RIVIAN_TX_MSGS[] = {{0x120, 0, 8, .check_relay = true}, {0x321, 2, 7, .check_relay = true}, {0x162, 2, 8, .check_relay = true}};
  // 0x160 = ACM_longitudinalRequest
  static const CanMsg RIVIAN_LONG_TX_MSGS[] = {{0x120, 0, 8, .check_relay = true}, {0x321, 2, 7, .check_relay = true}, {0x160, 0, 5, .check_relay = true}};

  static RxCheck rivian_rx_checks[] = {
    {.msg = {{0x208, 0, 8, 50U, .max_counter = 14U}, { 0 }, { 0 }}},                                                              // ESP_Status (speed)
    {.msg = {{0x150, 0, 7, 50U, .max_counter = 14U}, { 0 }, { 0 }}},                                                              // VDM_PropStatus (gas pedal & 2nd speed)
    {.msg = {{0x162, 0, 8, 50U, .max_counter = 14U, .ignore_quality_flag = true}, { 0 }, { 0 }}},                                 // VDM_AdasSts (stalk requests)
    {.msg = {{0x380, 0, 5, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},   // EPAS_SystemStatus (driver torque)
    {.msg = {{0x38f, 0, 6, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},    // iBESP2 (brakes)
    {.msg = {{0x100, 2, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},   // ACM_Status (cruise state)
  };

  bool rivian_longitudinal = false;
  rivian_prev_user_adas_request = 0U;

  SAFETY_UNUSED(param);
  #ifdef ALLOW_DEBUG
    const int FLAG_RIVIAN_LONG_CONTROL = 1;
    rivian_longitudinal = GET_FLAG(param, FLAG_RIVIAN_LONG_CONTROL);
  #endif

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
