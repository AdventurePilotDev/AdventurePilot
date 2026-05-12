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


def create_angle_steering(packer, frame, angle_deg, enabled, bus):
  values = {
    "ACM_SteeringControl_Counter": frame % 16,
    "ACM_EacEnabled": 1 if enabled else 0,
    "ACM_HapticRequired": 0,
    "ACM_SteeringAngleRequest": angle_deg,
  }
  data = packer.make_can_msg("ACM_SteeringControl", bus, values)[1]
  values["ACM_SteeringControl_Checksum"] = checksum(data[1:], 0x1D, 0x41)
  return packer.make_can_msg("ACM_SteeringControl", bus, values)


def create_acm_status(packer, frame, feature_status, bus):
  # Always streamed on the ACM-side bus so stock ACM's broadcasts don't pass
  # through the open relay. FeatureStatus mirrors stock cruise state (0=Standby,
  # 1=Acc, 2=Hwp) — Hwp is what convinces the EPAS to accept the external 0x110.
  values = {
    "ACM_Status_Counter": frame % 16,
    "ACM_FeatureStatus": feature_status,
    "ACM_Unkown1": 1,
    "ACM_FaultStatus": 0,
    "ACM_FaultSupervisorState": 1,
  }
  data = packer.make_can_msg("ACM_Status", bus, values)[1]
  values["ACM_Status_Checksum"] = checksum(data[1:], 0x1D, 0x5F)
  return packer.make_can_msg("ACM_Status", bus, values)


def create_wheel_touch(packer, sccm_wheel_touch, enabled):
  values = {s: sccm_wheel_touch[s] for s in (
    "SCCM_WheelTouch_Counter",
    "SCCM_WheelTouch_HandsOn",
    "SCCM_WheelTouch_CapacitiveValue",
    "SETME_X52",
  )}

  # When only using ACC without lateral, the ACM warns the driver to hold the steering wheel on engagement
  # Tell the ACM that the user is holding the wheel to avoid this warning
  if enabled:
    values["SCCM_WheelTouch_HandsOn"] = 1
    values["SCCM_WheelTouch_CapacitiveValue"] = 100  # only need to send this value, but both are set for consistency

  data = packer.make_can_msg("SCCM_WheelTouch", 2, values)[1]
  values["SCCM_WheelTouch_Checksum"] = checksum(data[1:], 0x1D, 0x97)
  return packer.make_can_msg("SCCM_WheelTouch", 2, values)


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


def create_adas_status(packer, vdm_adas_status, interface_status):
  values = {s: vdm_adas_status[s] for s in (
    "VDM_AdasStatus_Checksum",
    "VDM_AdasStatus_Counter",
    "VDM_AdasDecelLimit",
    "VDM_AdasDriverAccelPriorityStatus",
    "VDM_AdasFaultStatus",
    "VDM_AdasAccelLimit",
    "VDM_AdasDriverModeStatus",
    "VDM_AdasUnkown1",
    "VDM_AdasInterfaceStatus",
    "VDM_AdasVehicleHoldStatus",
    "VDM_UserAdasRequest",
  )}

  if interface_status is not None:
    values["VDM_AdasInterfaceStatus"] = interface_status

  data = packer.make_can_msg("VDM_AdasSts", 2, values)[1]
  values["VDM_AdasStatus_Checksum"] = checksum(data[1:], 0x1D, 0xD1)
  return packer.make_can_msg("VDM_AdasSts", 2, values)
