# This whol file assumes you're on Linux and have everything necessary installed
# to generate a pdf from a Doxyfile. If you're on Windows, like me, use WSL.

import os
import subprocess

# Get the version of the project from the Doxyfile
version = ""
with open("Doxyfile", "r") as f:
    for line in f:
        if "PROJECT_NUMBER" in line and "=" in line:
            version = line.split("=")[1].strip().replace("\"", "").replace(".", "-")
            break


# Run the doxygen command to generate the files
subprocess.run(["doxygen", "Doxyfile"])

# Run the make command to generate the pdf
os.chdir("latex")
subprocess.run(["make", "pdf"])

# Move latex/refman.pdf to the root directory
os.rename("refman.pdf", f"../EasyCLI-v{version}.pdf")

# Delete the latex directory as well as the html directory
os.chdir("..")
subprocess.run(["rm", "-rf", "latex"])
subprocess.run(["rm", "-rf", "html"])