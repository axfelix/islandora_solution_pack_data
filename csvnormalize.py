import sys
from subprocess import call
import openpyxl
import re
import os.path

# argv[2] is the process ID passed from inc/derivatives.inc.
# We need this to avoid nameing conflicts on the csvlist file.
csvlist_filepath = '/tmp/csvlist.' + sys.argv[2]

# Development only
if os.path.exists(csvlist_filepath):
  o = open(csvlist_filepath, 'ab')
  # o.write(sys.argv[1] + '\n')
  o.write("/tmp/test_ds.csv\n")
else:
  o = open(csvlist_filepath, 'wb')
  o.write("# Temporary data file. Do not edit.\n")
  # o.write(sys.argv[1] + '\n')
  o.write("/tmp/test_ds.csv\n")

