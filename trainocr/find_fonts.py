import subprocess
import re
output = subprocess.check_output("text2image --fonts_dir=fonts --list_available_fonts", shell=True).decode("utf-8")
fonts = re.findall(r" \d: (.*)", output)
for font in fonts:
    print(f'"{font}"', end=" ")
print()