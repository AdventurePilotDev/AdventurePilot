"""What the fork's jetlink tests share: the audit guard.

Install once here, and let each test start and stop it (GuardedTest below).
"""
from __future__ import annotations

import unittest

from jetlink.openpilot.testing import install_guard

GUARD = install_guard()


class GuardedTest(unittest.TestCase):
  """A test that fails if it reaches the machine's own params store, /dev/shm
  records, sysfs or sudo.

  Started in setUp() and stopped in tearDown(), not around the whole pytest
  call: OpenpilotTestCase enters its OPENPILOT_PREFIX before setUp() and tears
  it down after tearDown(), and that teardown lists /dev/shm/msgq_<prefix>,
  which the guard forbids. Between setUp() and tearDown() the guard sees only
  what the test itself does, params included."""

  def setUp(self) -> None:
    super().setUp()
    GUARD.start()

  def tearDown(self) -> None:
    touched = GUARD.stop()
    try:
      super().tearDown()
    finally:
      self.assertEqual(touched, [], f"the test reached the machine's own files: {touched}")
