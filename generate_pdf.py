## Pre-usage
# This whole file assumes you're on Linux and have everything necessary installed
# to generate a pdf from a Doxyfile. If you're on Windows, like me, use WSL.

## Usage
# This file is meant to be run from the root directory of the project.
# It will generate a pdf from the Doxyfile and move it to the root directory
# with the name EasyCLI-v{version}.pdf, where {version} is the version of the project with dashes instead of dots.

import os
import subprocess

# Get the version of the project from the Doxyfile
version = ""
with open("Doxyfile", "r") as f:
    for line in f:
        # If the line is the PROJECT_NUMBER line
        # It is the line that contains the version number, such as PROJECT_NUMBER = "0.2.1"
        if "PROJECT_NUMBER" in line and "=" in line:
            # Gets the right side of the equal sign, strips it of whitespace, quotes, and replaces . with -
            version = line.split("=")[1].strip().replace('"', "").replace(".", "-")
            # Get out of the loop early because we don't need to read the lines anymore.
            # Truly an unthinkable revolutionary optimization idea for python standards.
            # I expect my nobel prize in the mail.
            break


# Run the doxygen command to generate the files
subprocess.run(["doxygen", "Doxyfile"])

# Change the dir from the root to the dir in which the latex files are
os.chdir("latex")

# Run make pdf to generate the pdf
subprocess.run(["make", "pdf"])

# Move latex/refman.pdf to the root directory
# As we are in the latex directory, refman.pdf is in the latex directory,
# and we need to move it to the root directory
os.rename("refman.pdf", f"../EasyCLI-v{version}.pdf")

# Go back to the root directory
os.chdir("..")

# Remove the latex and html directories that Doxygen generated
subprocess.run(["rm", "-rf", "latex"])
subprocess.run(["rm", "-rf", "html"])
