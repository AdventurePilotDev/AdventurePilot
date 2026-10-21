// Standalone test for rivian_keepalive.h, no panda or messaging needed:
//   g++ -std=c++17 -I. openpilot/selfdrive/pandad/tests/test_rivian_keepalive.cc -o /tmp/test_rivian_keepalive && /tmp/test_rivian_keepalive
#undef NDEBUG
#include <cassert>
#include <cstdio>

#include "openpilot/selfdrive/pandad/rivian_keepalive.h"

using KA = RivianKeepalive;
static const uint64_t MS = 1000000ULL;

// frames from opendbc's riviancan.py + CANPacker for counters 3, 4, 13, 14
// (angle 1.2 deg active, feature status 2, accel -0.53 enabled, lka icons on)
struct Ref { uint32_t addr; uint8_t len; uint8_t c3[8], c4[8], c13[8], c14[8]; };
static const Ref REFS[] = {
  {0x110, 8, {0x26,0x13,0x80,0x18,0,0,0,0}, {0xa8,0x14,0x80,0x18,0,0,0,0}, {0x27,0x1d,0x80,0x18,0,0,0,0}, {0xc0,0x1e,0x80,0x18,0,0,0,0}},
  {0x100, 8, {0xe8,0x03,0x40,0,0,0,0,0}, {0x66,0x04,0x40,0,0,0,0,0}, {0xe9,0x0d,0x40,0,0,0,0,0}, {0x0e,0x0e,0x40,0,0,0,0,0}},
  {0x160, 5, {0x31,0x03,0x79,0x60,0x10}, {0xc5,0x04,0x79,0x60,0x10}, {0xc4,0x0d,0x79,0x60,0x10}, {0x7e,0x0e,0x79,0x60,0x10}},
  {0x120, 8, {0x8d,0x03,0x80,0x03,0x51,0x70,0x02,0x90}, {0x03,0x04,0x80,0x03,0x51,0x70,0x02,0x90},
             {0x8c,0x0d,0x80,0x03,0x51,0x70,0x02,0x90}, {0x6b,0x0e,0x80,0x03,0x51,0x70,0x02,0x90}},
};

static const uint8_t *ref(const Ref &r, int counter) {
  switch (counter) {
    case 3: return r.c3;
    case 4: return r.c4;
    case 13: return r.c13;
    default: return r.c14;
  }
}

// feed one real sendcan message with all four control frames at the given card counter
static void real_message(KA &ka, int counter, uint64_t now, uint8_t out[4][8]) {
  for (int i = 0; i < 4; i++) {
    memcpy(out[i], ref(REFS[i], counter), REFS[i].len);
    ka.on_frame(REFS[i].addr, 0, out[i], REFS[i].len, now);
  }
}

static const KeepaliveFrame *find(const std::vector<KeepaliveFrame> &v, uint32_t addr) {
  for (const auto &f : v) if (f.addr == addr) return &f;
  return nullptr;
}

int main() {
  // checksums match opendbc
  const uint8_t xors[] = {0x41, 0x5F, 0x12, 0x63};
  for (int i = 0; i < 4; i++) {
    for (int c : {3, 4, 13, 14}) {
      assert(KA::checksum(ref(REFS[i], c), REFS[i].len, xors[i]) == ref(REFS[i], c)[0]);
    }
  }

  {
    // no fill before any real message, and none while sendcan is on time
    KA ka;
    uint8_t buf[4][8];
    assert(ka.fill(100 * MS).empty());
    real_message(ka, 3, 1000 * MS, buf);
    assert(ka.on_real_message(1000 * MS) == 0);
    assert(!ka.needs_rewrite());
    assert(ka.fill(1010 * MS).empty());
    assert(ka.fill(1024 * MS).empty());
    for (int i = 0; i < 4; i++) assert(memcmp(buf[i], ref(REFS[i], 3), REFS[i].len) == 0);  // untouched
  }

  {
    // 100 ms stall after counter 3: first fill at 25 ms, then every 10 ms; the first fill is exactly what card
    // would have sent next (counter 4)
    KA ka;
    uint8_t buf[4][8];
    real_message(ka, 3, 1000 * MS, buf);
    ka.on_real_message(1000 * MS);

    auto f1 = ka.fill(1025 * MS);
    assert(f1.size() == 4);
    for (const auto &r : REFS) {
      const KeepaliveFrame *f = find(f1, r.addr);
      assert(f != nullptr && f->bus == 0 && f->len == r.len);
      assert(memcmp(f->dat, r.c4, r.len) == 0);
    }
    assert(ka.fill(1030 * MS).empty());  // not due yet
    int fills = 1;
    for (uint64_t t = 1035; t < 1100; t += 5) {
      auto f = ka.fill(t * MS);
      if (!f.empty()) {
        fills++;
        assert((f[0].dat[1] & 0x0F) == (3 + fills) % 15);
      }
    }
    assert(fills == 8);  // 25, 35, ..., 95 ms
    assert(ka.needs_rewrite());

    // card resumes with its own next counter (4): on the bus it must continue after the last fill (11 -> 12)
    real_message(ka, 4, 1100 * MS, buf);
    assert(ka.on_real_message(1100 * MS) == 8);
    assert(ka.last_gap() == 100 * MS);
    for (int i = 0; i < 4; i++) {
      assert((buf[i][1] & 0x0F) == 12);
      assert((buf[i][1] & 0xF0) == (ref(REFS[i], 4)[1] & 0xF0));                           // other bits in byte 1 kept
      assert(memcmp(&buf[i][2], &ref(REFS[i], 4)[2], REFS[i].len - 2) == 0);                // payload untouched
      assert(buf[i][0] == KA::checksum(buf[i], REFS[i].len, xors[i]));
    }

    // counter wraps 14 -> 0, never 15
    real_message(ka, 13, 1110 * MS, buf);
    assert((buf[0][1] & 0x0F) == (13 + 8) % 15);
    ka.on_real_message(1110 * MS);
  }

  {
    // capped at MAX_FILL_FRAMES per gap, and a new gap can be filled again afterwards
    KA ka;
    uint8_t buf[4][8];
    real_message(ka, 3, 1000 * MS, buf);
    ka.on_real_message(1000 * MS);
    int fills = 0;
    for (uint64_t t = 1005; t < 2000; t += 5) fills += ka.fill(t * MS).empty() ? 0 : 1;
    assert(fills == KA::MAX_FILL_FRAMES);
    assert(!ka.needs_rewrite());  // 15 fills is a whole counter cycle

    real_message(ka, 4, 2000 * MS, buf);
    assert(ka.on_real_message(2000 * MS) == KA::MAX_FILL_FRAMES);
    assert(!ka.fill(2025 * MS).empty());
  }

  {
    // a message card stopped sending is not repeated, and other buses/addresses/lengths are ignored
    KA ka;
    uint8_t d[8];
    memcpy(d, REFS[2].c3, 5);
    ka.on_frame(0x160, 0, d, 5, 1000 * MS);
    ka.on_real_message(1000 * MS);
    for (uint64_t t = 1010; t <= 1100; t += 10) {  // 0x110 keeps coming, 0x160 does not
      memcpy(d, REFS[0].c3, 8);
      assert(!ka.on_frame(0x110, 0, d, 8, t * MS));
      assert(!ka.on_frame(0x110, 2, d, 8, t * MS));
      assert(!ka.on_frame(0x163, 2, d, 8, t * MS));
      assert(!ka.on_frame(0x162, 0, d, 8, t * MS));  // the VDM's own frame on bus 0 is not ours
      assert(!ka.on_frame(0x160, 0, d, 8, t * MS));  // wrong length
      ka.on_real_message(t * MS);
    }
    auto f = ka.fill(1125 * MS);
    assert(f.size() == 1 && f[0].addr == 0x110);
  }

  {
    // VDM_AdasSts relay on bus 2, real frames from c17ea97dc5472650/0000008d (VDM counters 4, 5, 6 with an
    // unchanged payload): the fill after counter 4 must be exactly the frame the VDM sent next
    const uint8_t v4[8] = {0x38, 0x04, 0x10, 0x01, 0x03, 0xfe, 0x04, 0x08};
    const uint8_t v5[8] = {0x65, 0x05, 0x10, 0x01, 0x03, 0xfe, 0x04, 0x08};
    const uint8_t v6[8] = {0x82, 0x06, 0x10, 0x01, 0x03, 0xfe, 0x04, 0x08};
    assert(KA::checksum(v4, 8, 0xD1) == v4[0] && KA::checksum(v6, 8, 0xD1) == v6[0]);

    KA ka;
    uint8_t d[8];
    memcpy(d, v4, 8);
    assert(!ka.on_frame(0x162, 2, d, 8, 1000 * MS));
    ka.on_real_message(1000 * MS);

    auto f = ka.fill(1025 * MS);
    assert(f.size() == 1 && f[0].addr == 0x162 && f[0].bus == 2 && f[0].len == 8);
    assert(memcmp(f[0].dat, v5, 8) == 0);
    f = ka.fill(1035 * MS);
    assert(f.size() == 1 && memcmp(f[0].dat, v6, 8) == 0);
  }

  {
    // fills stay on a 10 ms grid when the poll comes a little late (2 ms poll: 25, 35, 45 ... not 27, 37, 47)
    KA ka;
    uint8_t buf[4][8];
    real_message(ka, 3, 1000 * MS, buf);
    ka.on_real_message(1000 * MS);
    int fills = 0;
    for (uint64_t t = 1001; t < 1100; t += 2) {  // odd milliseconds only, so every poll is 1 ms late
      if (!ka.fill(t * MS).empty()) {
        fills++;
        assert(t == 1025 + 10 * (uint64_t)(fills - 1) || t == 1026 + 10 * (uint64_t)(fills - 1));
      }
    }
    assert(fills == 8);  // 25, 35, ..., 95 ms
  }

  printf("ok\n");
  return 0;
}
