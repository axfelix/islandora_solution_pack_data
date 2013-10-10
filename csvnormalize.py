# Development placeholder script.

import sys
import os.path

csvlist_filepath = '/tmp/csvlist.' + sys.argv[2]

if os.path.exists(csvlist_filepath):
  o = open(csvlist_filepath, 'ab')
  # o.write("/tmp/test_ds.csv\n")
  o.write(sys.argv[1] + "\n")
else:
  o = open(csvlist_filepath, 'wb')
  # o.write("/tmp/test_ds.csv\n")
  o.write(sys.argv[1] + "\n")

o.close()

