"""Slim telemetry for the opview app.

With opview streaming on, webrtcd sends each service as JSON over the WebRTC data channel. Sent whole, that was
about 235 messages and 330 kB a second (2026-10-10 data_stats), most of it fields opview never reads, every float
at full precision. Here each service is cut down to the fields opview reads (gerrylum/opview, lib/selfdrive/ui/
ui_state.dart, the apply* methods), floats are rounded, and each service gets its own rate.

A service missing from OPVIEW_FIELDS is still sent whole (floats rounded), so a new opview readout keeps working
before its fields are added here, just at the old cost. A field opview reads that is missing here arrives as
absent and opview shows its default for it: when the app starts reading a new field, add it here.
"""
import math
from typing import Any

import capnp

FLOAT_DIGITS = 3

# a leaf is True, or the number of decimal places to round it to when FLOAT_DIGITS is too coarse; a nested struct (or
# a list of structs) is a dict of its fields; UNION keeps whichever member of a union is set, as {member: fields}, the
# same shape capnp's to_dict() gives
UNION = "__union__"

_LEAD = {"present": True, "radar": True, "dRel": True, "yRel": True, "vRel": True, "vLead": True}
_DM = {
  "activePolicy": True, "isRHD": True,
  "visionPolicyState": {"faceDetected": True, "awarenessPercent": True, "pose": True},
  # older openpilot: flat fields
  "isActiveMode": True, "faceDetected": True, "awarenessStatus": True,
}

OPVIEW_FIELDS: dict[str, dict] = {
  "carState": {
    "vEgo": True, "vEgoCluster": True, "vCruiseCluster": True, "aEgo": True,
    "steeringAngleDeg": True, "steeringRateDeg": True, "steeringTorque": True,
    "leftBlinker": True, "rightBlinker": True, "leftBlindspot": True, "rightBlindspot": True,
    "gasPressed": True, "brakePressed": True, "gearShifter": True,
    "cruiseState": {"enabled": True},
  },
  "selfdriveState": {
    "enabled": True, "engageable": True, "experimentalMode": True, "state": True, "personality": True,
    "alertText1": True, "alertText2": True, "alertSize": True, "alertStatus": True,
  },
  "controlsState": {
    # curvatures, typically 0.0001 to 0.01 1/m: three places made the angle-steering torque bar move in coarse steps
    "curvature": 6, "desiredCurvature": 6, "vCruiseDEPRECATED": True,
    "lateralControlState": {UNION: {"saturated": True}},
  },
  "radarState": {"leadOne": _LEAD, "leadTwo": _LEAD},
  "longitudinalPlan": {"allowThrottle": True},
  "deviceState": {
    "deviceType": True, "started": True, "cpuTempC": True, "memoryUsagePercent": True,
    "freeSpacePercent": True, "networkStrength": True, "powerDrawW": True,
  },
  "narrowRoadCameraState": {"sensor": True},
  "roadCameraState": {"sensor": True},
  "extrinsicsCalibration": {"rpyCalib": True, "wideFromDeviceEuler": True, "calStatus": True, "calPerc": True, "height": True},
  "liveCalibration": {"rpyCalib": True, "wideFromDeviceEuler": True, "calStatus": True, "calPerc": True, "height": True},
  "longitudinalPlanSP": {
    "speedLimit": {
      "resolver": {
        "speedLimit": True, "speedLimitLast": True, "speedLimitOffset": True, "speedLimitValid": True,
        "speedLimitLastValid": True, "speedLimitFinalLast": True, "source": True,
      },
      "assist": {"state": True},
    },
  },
  "liveMapDataSP": {"speedLimitAheadValid": True, "speedLimitAhead": True, "speedLimitAheadDistance": True, "roadName": True},
  "carControl": {"latActive": True, "longActive": True, "actuators": {"accel": True, "steeringAngleDeg": True}},
  "carOutput": {"actuatorsOutput": {"torque": True, "torqueOutputCan": True}},
  # the lane centering indicator; whole, carControlSP would also carry its list of params
  "carControlSP": {"laneCentering": {
    "active": True, "holding": True, "clipped": True, "singleLine": True, "lineSide": True, "offset": True,
    # a curvature, typically 0.0001 to 0.001 1/m: three places would round it away
    "correction": 6,
  }},
  "selfdriveStateSP": {"mads": {"state": True, "enabled": True, "available": True}},
  "onroadEvents": {"name": True, "overrideLongitudinal": True, "overrideLateral": True},
  "driverMonitoringState": _DM,
}

# messages per second per service. modelV2 drives opview's redraw and is never thinned; anything not listed is
# thinned to OPVIEW_DEFAULT_RATE_HZ when it is published faster
OPVIEW_DEFAULT_RATE_HZ = 20.
OPVIEW_RATES_HZ: dict[str, float] = {
  # moment to moment: speed, alerts, lead box, torque bar
  "carState": 20., "selfdriveState": 20., "controlsState": 20., "radarState": 20., "carOutput": 20.,
  "carControlSP": 20.,
  # readouts that opview refreshes about 5 times a second
  "carControl": 10., "driverMonitoringState": 10., "selfdriveStateSP": 10., "onroadEvents": 10.,
  "longitudinalPlan": 5., "longitudinalPlanSP": 5.,
  # slow-changing
  "liveMapDataSP": 2., "deviceState": 2., "extrinsicsCalibration": 2., "liveCalibration": 2.,
  "narrowRoadCameraState": 1., "roadCameraState": 1.,
}


def min_gap_s(service: str) -> float | None:
  """shortest time between two sends of [service]; None: send every message"""
  if service == "modelV2":
    return None
  # 0.9 of the period: the send loop wakes about every 10 ms, so a full period would land a little under the rate
  return 0.9 / OPVIEW_RATES_HZ.get(service, OPVIEW_DEFAULT_RATE_HZ)


def rounded(value: Any, digits: int = FLOAT_DIGITS) -> Any:
  """[value] with every float rounded to [digits] places (non-finite floats as they are)"""
  if isinstance(value, float):
    return round(value, digits) if math.isfinite(value) else value
  if isinstance(value, dict):
    return {k: rounded(v, digits) for k, v in value.items()}
  if isinstance(value, list):
    return [rounded(v, digits) for v in value]
  return value


def _plain(value: Any, digits: int = FLOAT_DIGITS) -> Any:
  # a capnp value as JSON-ready Python, floats rounded
  if isinstance(value, (bool, int, float, str)) or value is None:
    return rounded(value, digits)
  if isinstance(value, bytes):
    return value.decode(errors="replace")
  if isinstance(value, capnp._DynamicStructReader):
    return rounded(value.to_dict(), digits)
  if isinstance(value, capnp._DynamicListReader):
    return [_plain(v, digits) for v in value]
  return str(value)  # enums


def pick(reader: Any, spec: dict) -> Any:
  """the fields of [reader] named in [spec]; a list of structs gives a list of picked structs"""
  if isinstance(reader, capnp._DynamicListReader):
    return [pick(item, spec) for item in reader]
  if UNION in spec:
    member = str(reader.which())
    return {member: pick(getattr(reader, member), spec[UNION])}
  out: dict[str, Any] = {}
  for key, sub in spec.items():
    try:
      value = getattr(reader, key)
    except (AttributeError, capnp.KjException):
      continue  # not in this branch's schema
    if sub is True:
      out[key] = _plain(value)
    elif isinstance(sub, int):
      out[key] = _plain(value, sub)
    else:
      out[key] = pick(value, sub)
  return out


def slim(service: str, msg: Any) -> Any:
  """the JSON-ready payload opview gets for one [service] message"""
  spec = OPVIEW_FIELDS.get(service)
  if spec is not None:
    return pick(msg, spec)
  return _plain(msg)
