from glob import glob
import os
from pathlib import Path

files = sorted(glob("fonts/*"))

for num, file in enumerate(files):
    path = Path(file)
    os.rename(file, f"{path.parent.joinpath(str(num) + path.suffix)}")
