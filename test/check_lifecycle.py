"""Run real library lifecycle code against a deterministic host scheduler.

This catches overlapping task creation; it does not emulate BLE or prove RF.
Requires a C++11 compiler (g++ by default, or CXX).
"""
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix="findmyadv-lifecycle-") as directory:
    executable = Path(directory) / ("lifecycle.exe" if os.name == "nt" else "lifecycle")
    subprocess.run([
        os.environ.get("CXX", "g++"), "-std=c++11",
        "-DFINDMYADV_DISABLE_BUILTIN_ACCELEROMETER=1",
        "-I", str(ROOT / "test/host"), str(ROOT / "test/host/lifecycle.cpp"),
        "-o", str(executable),
    ], check=True)
    subprocess.run([str(executable)], check=True)
print("Lifecycle: immediate restart, repeated restart, reconfigure, manual mode and bounded shutdown passed")
