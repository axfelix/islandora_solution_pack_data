import sys
from subprocess import call
import openpyxl
import string
import re
import copy

# argv[2] is the process ID passed from inc/derivatives.inc.
# We need this to avoid naming conflicts on the csvlist file.
csvlist_file = '/tmp/csvlist.' + sys.argv[2]
csvlist_fileobject = open(csvlist_file,'w')
csvlist_filepath = csvlist_file + '.'
nopath = re.sub(r'.*/','', sys.argv[1])

# argv for filename here
xlsx = re.search(r'\.xlsx', sys.argv[1])
if xlsx is None:
	xlsxname = re.sub(r'\.[A-Za-z]{3}$','.xlsx', nopath)
	workbookname = csvlist_filepath+xlsxname
	call(['ssconvert','--export-type=Gnumeric_Excel:xlsx',sys.argv[1],workbookname])
else:
	workbookname = sys.argv[1]
workbook = openpyxl.load_workbook(workbookname)
sheets = workbook.get_sheet_names()
totalsheets = len(sheets)
if totalsheets!=1:
	workbooks = {}
	for y in sheets:
		if y != "DV-IDENTITY-0":
			workbooks[y] = copy.deepcopy(workbook)
			for x in sheets:
				if x != y:
					workbooks[y].remove_sheet(workbooks[y].get_sheet_by_name(x))
			cleanname = re.sub(r'[^a-zA-Z0-9\-_]','',y)
			filename = csvlist_filepath+cleanname+'.xlsx'
			workbooks[y].save(filename)
			csvname = csvlist_filepath+cleanname+'.csv'
			o = open(csvname, 'wb')
			call(['in2csv',filename], stdout=o)
			csvlist_fileobject.write(csvname+'\n')
else:
	csvname = re.sub(r'\.xlsx?','.csv', nopath)
	o = open(csvlist_filepath+csvname, 'wb')
	call(['in2csv',workbookname], stdout=o)
	csvlist_fileobject.write(csvlist_filepath+csvname)
