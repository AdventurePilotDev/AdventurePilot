def checksum(data, poly, xor_output):
  crc = 0
  for byte in data:
    crc ^= byte
    for _ in range(8):
      if crc & 0x80:
        crc = (crc << 1) ^ poly
      else:
        crc <<= 1
      crc &= 0xFF
  return crc ^ xor_output


def create_lka_steering(packer, frame, acm_lka_hba_cmd, apply_torque, enabled, active, mads, lka_act_toi):
  # forward auto high beam and speed limit status and nothing else
  values = {s: acm_lka_hba_cmd[s] for s in (
    "ACM_hbaSysState",
    "ACM_hbaLamp",
    "ACM_hbaOnOffState",
    "ACM_slifOnOffState",
  )}

  values |= {
    "ACM_lkaHbaCmd_Counter": frame % 15,
    "ACM_lkaStrToqReq": apply_torque,
    "ACM_lkaActToi": lka_act_toi,

    "ACM_lkaLaneRecogState": 3 if mads.lka_icon_states else 0,
    "ACM_lkaSymbolState": 3 if mads.lka_icon_states else 0,

    # static values
    "ACM_lkaElkRequest": 0,
    "ACM_ldwlkaOnOffState": 2,  # 2=LKAS+LDW on
    "ACM_elkOnOffState": 1,  # 1=LKAS on
    # TODO: what are these used for?
    "ACM_ldwWarnTypeState": 2,  # always 2
    "ACM_ldwWarnTimingState": 1,  # always 1
    #"ACM_lkaHandsoffDisplayWarning": 1,  # TODO: we can send this when openpilot wants you to pay attention
  }

  data = packer.make_can_msg("ACM_lkaHbaCmd", 0, values)[1]
  values["ACM_lkaHbaCmd_Checksum"] = checksum(data[1:], 0x1D, 0x63)
  return packer.make_can_msg("ACM_lkaHbaCmd", 0, values)


def create_longitudinal(packer, frame, accel, enabled):
  values = {
    "ACM_longitudinalRequest_Counter": frame % 15,
    "ACM_AccelerationRequest": accel,
    "ACM_PrndRequest": 0,
    "ACM_longInterfaceEnable": 1 if enabled else 0,
    "ACM_VehicleHoldRequest": 0,
  }

  data = packer.make_can_msg("ACM_longitudinalRequest", 0, values)[1]
  values["ACM_longitudinalRequest_Checksum"] = checksum(data[1:], 0x1D, 0x12)
  return packer.make_can_msg("ACM_longitudinalRequest", 0, values)
