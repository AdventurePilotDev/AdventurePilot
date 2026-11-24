import os

# Importing the UI library on a PC opens a 1x1 raylib window to measure the screen unless SCALE is set.
# With no usable display (headless CI, a locked or sleeping Mac) that segfaults during test collection.
# Tests never draw at real size, so a fixed scale is fine.
os.environ.setdefault("SCALE", "1")
