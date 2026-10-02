#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

// Rivian: short keep-alive for openpilot's 100 Hz control frames.
//
// The EPAS and VDM latch a fault that only clears when the car sleeps if ACM_SteeringControl (0x110) or
// ACM_longitudinalRequest (0x160) drop out for roughly 60-80 ms while active. Device-wide stalls of ~100 ms
// have been seen right after the first engage of a drive; they freeze card (and most other processes) but
// not pandad. While sendcan is quiet, pandad repeats the last frame of each control message with the
// counter advanced and the checksum recomputed, for at most MAX_FILL_FRAMES frames per gap.
//
// The repeated frames carry the same command as the last real frame and still go through the panda's
// safety TX checks. After a fill, card's counters are behind what the bus has seen, so every later frame
// has its counter shifted by the number of filled frames (mod 15) to keep the sequence continuous.

struct KeepaliveFrame {
  uint32_t addr;
  uint8_t bus;
  uint8_t len;
  uint8_t dat[8];
};

class RivianKeepalive {
public:
  static constexpr uint64_t GAP_NS = 25000000ULL;     // sendcan is 10 ms; start filling after 25 ms of silence
  static constexpr uint64_t PERIOD_NS = 10000000ULL;  // then one frame every 10 ms
  static constexpr int MAX_FILL_FRAMES = 15;          // ~150 ms, then let the car fault as before
  static constexpr uint64_t STALE_NS = 50000000ULL;   // only repeat messages that were part of the live stream
  static constexpr uint8_t COUNTER_MOD = 15;          // counters run 0-14

  static uint8_t checksum(const uint8_t *dat, size_t len, uint8_t xor_out) {
    uint8_t crc = 0;
    for (size_t i = 1; i < len; i++) {
      crc ^= dat[i];
      for (int b = 0; b < 8; b++) {
        crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x1D) : (uint8_t)(crc << 1);
      }
    }
    return crc ^ xor_out;
  }

  // true once a fill has shifted any counter, i.e. real frames now need rewriting
  bool needs_rewrite() const {
    for (const auto &s : slots) {
      if (s.offset != 0) return true;
    }
    return false;
  }

  // Call for every frame of a real sendcan message. Remembers tracked frames and, if a previous fill
  // shifted that message's counter, rewrites counter and checksum in place. Returns true if dat changed.
  bool on_frame(uint32_t addr, uint8_t bus, uint8_t *dat, size_t len, uint64_t now_ns) {
    Slot *s = find(addr, bus);
    if (s == nullptr || len != s->len) return false;

    memcpy(s->last, dat, len);
    s->last_ns = now_ns;

    if (s->offset == 0) return false;
    apply(*s, dat);
    return true;
  }

  // Call after a real sendcan message was handled. Returns how many fills the gap before it needed.
  int on_real_message(uint64_t now_ns) {
    int filled = fills_in_gap;
    last_gap_ns = (filled > 0) ? (now_ns - last_real_ns) : 0;
    fills_in_gap = 0;
    last_real_ns = now_ns;
    return filled;
  }

  uint64_t last_gap() const { return last_gap_ns; }

  // Call while sendcan is quiet. Returns the frames to send now, empty if no fill is due.
  std::vector<KeepaliveFrame> fill(uint64_t now_ns) {
    std::vector<KeepaliveFrame> out;
    if (last_real_ns == 0 || fills_in_gap >= MAX_FILL_FRAMES) return out;

    const uint64_t due_ns = (fills_in_gap == 0) ? (last_real_ns + GAP_NS) : (last_fill_ns + PERIOD_NS);
    if (now_ns < due_ns) return out;

    for (auto &s : slots) {
      // skip messages card wasn't sending right before the gap
      if (s.last_ns == 0 || s.last_ns + STALE_NS < last_real_ns) continue;

      s.offset = (s.offset + 1) % COUNTER_MOD;

      KeepaliveFrame &f = out.emplace_back();
      f.addr = s.addr;
      f.bus = s.bus;
      f.len = s.len;
      memcpy(f.dat, s.last, s.len);
      apply(s, f.dat);
    }

    if (!out.empty()) {
      fills_in_gap++;
      last_fill_ns = now_ns;
    }
    return out;
  }

private:
  struct Slot {
    uint32_t addr;
    uint8_t bus;
    uint8_t len;
    uint8_t xor_out;
    uint8_t offset = 0;    // fill frames sent so far, mod 15
    uint8_t last[8] = {};  // last real frame, as card sent it
    uint64_t last_ns = 0;
  };

  // all four are sent by card every 10 ms on bus 0: checksum in byte 0, counter in the low nibble of byte 1
  std::array<Slot, 4> slots = {{
    {0x100, 0, 8, 0x5F},  // ACM_Status
    {0x110, 0, 8, 0x41},  // ACM_SteeringControl
    {0x120, 0, 8, 0x63},  // ACM_lkaHbaCmd
    {0x160, 0, 5, 0x12},  // ACM_longitudinalRequest
  }};

  uint64_t last_real_ns = 0;
  uint64_t last_fill_ns = 0;
  uint64_t last_gap_ns = 0;
  int fills_in_gap = 0;

  Slot *find(uint32_t addr, uint8_t bus) {
    for (auto &s : slots) {
      if (s.addr == addr && s.bus == bus) return &s;
    }
    return nullptr;
  }

  static void apply(const Slot &s, uint8_t *dat) {
    const uint8_t counter = (uint8_t)(((dat[1] & 0x0F) + s.offset) % COUNTER_MOD);
    dat[1] = (dat[1] & 0xF0) | counter;
    dat[0] = checksum(dat, s.len, s.xor_out);
  }
};
