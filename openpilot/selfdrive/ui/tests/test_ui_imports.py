"""Import every UI module, so an import-time error cannot reach a device.

This is deliberately the cheapest possible test: it imports each module and asserts nothing.
It exists because an import-time failure in any UI module is not a small bug. The UI entry
point pulls in most of the tree, so one bad module takes the whole interface down, the screen
never draws, and the device looks like it has hung during startup with no visible clue why.

The case that prompted it was a function annotated `-> rl.Texture | None`. pyray exposes
Texture as a cffi constructor function rather than a class, so the union raises TypeError, and
a return annotation is evaluated when the module is read rather than when the function runs.
Neither ruff nor any unit test could see it, because nothing imported that module outside the
running UI. Note that the same annotation on an instance attribute inside a method body is
harmless, since those are never evaluated, which is what makes the mistake easy to make.

Anything that only breaks at import is in scope here: a bad annotation, a circular import, a
module renamed by an upstream merge, a package that lost its files. Everything past import
belongs in a real test.
"""
import importlib
from pathlib import Path

import pytest

import openpilot.selfdrive.ui
import openpilot.system.ui

# Enough of the tree that a discovery bug is obvious rather than silent
MINIMUM_EXPECTED_MODULES = 100


def _modules_under(package) -> list[str]:
  names = []
  # __path__ rather than __file__: these are namespace packages with no __init__.py, so __file__ is None
  for root in (Path(entry) for entry in package.__path__):
    for path in sorted(root.rglob("*.py")):
      if "__pycache__" in path.parts or "tests" in path.parts or path.name.startswith("test_"):
        continue
      parts = [part for part in path.relative_to(root).with_suffix("").parts if part != "__init__"]
      names.append(".".join([package.__name__, *parts]))
  return names


UI_MODULES = sorted(set(_modules_under(openpilot.selfdrive.ui) + _modules_under(openpilot.system.ui)))


def test_modules_were_discovered():
  # Without this, a broken walk would collect nothing and every test below would pass vacuously
  assert len(UI_MODULES) >= MINIMUM_EXPECTED_MODULES, f"only found {len(UI_MODULES)} UI modules, discovery is broken"


@pytest.mark.parametrize("module_name", UI_MODULES)
def test_module_imports(module_name):
  importlib.import_module(module_name)
