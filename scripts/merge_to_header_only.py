#
# 2025-02-15 23:22:22 dpw
#

import os
import re

# Input files
header_files = ["include/datetimelib/datetimelib.hpp"]  # Adjust filenames
source_file = "src/datetimelib.cpp"  # Adjust filename
output_file = "single/datetimelib.hpp"

# Collect unique includes
includes = set()
source_code = []
header_contents = []

# Process headers
for header in header_files:
    with open(header, "r") as f:
        lines = f.readlines()
    
    for line in lines:
        if line.startswith("#include"):
            includes.add(line.strip())
        else:
            header_contents.append(line)

# Process source file
with open(source_file, "r") as f:
    lines = f.readlines()

for line in lines:
    if line.startswith("#include"):
        includes.add(line.strip())
    else:
        source_code.append(line)

# Merge everything into a single header
with open(output_file, "w") as f:
    f.write("// Merged Header-Only Library\n")
    # f.write("#pragma once\n\n")
    
    # Write includes
    for include in sorted(includes):
        f.write(include + "\n")

    f.write("\n// Header Definitions\n")
    f.writelines(header_contents)

    f.write("\n// Inline Implementation\n")
    # f.write("namespace mylib {\ninline namespace impl {\n")  # Adjust namespace

    for line in source_code:
        # Remove redundant includes
        if not line.startswith("#include"):
            f.write(line)

    # f.write("\n}  // namespace impl\n}  // namespace mylib\n")

