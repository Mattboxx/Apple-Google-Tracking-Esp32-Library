"""Check publication metadata without hardware or third-party Python packages."""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
manifest = json.loads((ROOT / "library.json").read_text(encoding="utf-8"))
properties = dict(line.split("=", 1) for line in (ROOT / "library.properties").read_text(encoding="utf-8").splitlines()
                  if "=" in line and not line.startswith("#"))
assert manifest["name"] == properties["name"] == "FindMyAdv"
assert manifest["version"] == properties["version"]
assert re.fullmatch(r"\d+\.\d+\.\d+", manifest["version"])
canonical = "https://github.com/Mattboxx/TagRelayLab-ESP32-Library"
assert manifest["homepage"] == properties["url"] == canonical
assert manifest["repository"]["url"] == canonical + ".git"
dependency = manifest["dependencies"]["h2zero/NimBLE-Arduino"]
assert re.fullmatch(r"\^[1-9]\d*\.\d+\.\d+", dependency)
minimum = dependency[1:]
next_major = int(minimum.split(".")[0]) + 1
assert properties["depends"] == f"NimBLE-Arduino (>={minimum} && <{next_major}.0.0)"
print("FindMyAdv metadata: names, version, canonical URLs and dependency ranges agree")
