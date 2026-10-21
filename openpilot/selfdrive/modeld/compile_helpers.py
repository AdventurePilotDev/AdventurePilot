def estimate_pickle_max_size(onnx_size: int, num_variants: int = 1) -> int:
  # Each camera resolution has its own compiled JIT. QCOM programs with spatial
  # recurrent features can approach 2x the ONNX size per variant. Shared weights
  # may reduce the total, but must not be assumed when reserving chunk targets.
  # Overestimating only adds empty trailing chunks, which the reader supports.
  return num_variants * (2 * onnx_size + 10 * 1024 * 1024)
