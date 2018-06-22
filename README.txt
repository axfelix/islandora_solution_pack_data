Islandora Data Solution Pack by Simon Fraser University Library

Prerequisites:

-Python version 2.5+ (not tested with Python 3)

Dependencies provided by installer script (tested on Ubuntu):

-nodejs (note: the version in Ubuntu sources is out of date; this script should
 automatically add a more up to date PPA on Ubuntu, but not tested against other distributions)
-Ethercalc (ethercalc.net)
-nodejs and redis (required by Ethercalc)
-PPSP (for converting SPSS files to CSV)
-Gnumeric (used to convert Excel/OpenOffice formats; may add a boatload of Gtk and/or X stuff on non-Gnome environments)
-Python csvkit (includes openpyxl)

Currently accepts upload of tabular data in XLSX, XLS, ODS, SAV (SPSS), and CSV format. All non-CSV formats are normalized to CSV format upon upload. If uploaded workbook contains multiple "worksheets," each worksheet is exported to CSV and each CSV is ingested by Islandora as an individual datastream of the collection object. Currently CSVs are not validated in any way; this may change in the future.

Running the installer script sets up a Redis-backed Ethercalc instance which is available on port 8000. Ethercalc is a collaborative real-time editor; essentially, a local version of Google Docs. Viewing a tabular data object in our repository automatically passes it to a new instance of Ethercalc using a simple POST command. These files can then be edited in the browser and downloaded as CSV if desired.

Our script configures Ethercalc to run with the flag "expire -86400", which means that each of these Ethercalc document instances is automatically deleted one day.

(86400 seconds) after the last time it is edited. These are effectively temporary files and should be treated as such -- the generated URL can be shared for short-term collaboration, but we do not intend for a document editor to run on top of our repository; Ethercalc is mainly used to provide a nice in-browser view for CSV files.

edit data_prereqs.sh line 23 and change '\' to '/'

for unix users edit the data_prereqs.sh with vi

``~$vi data_prereqs.sh``
``:set ff=unix``
``:wq``


