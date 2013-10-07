import sys
from subprocess import call
import openpyxl
import re

csvlist_filepath = '/tmp/csvlist.' + sys.argv[2]

# argv for filename here
xlsx = re.search(r'\.xlsx', sys.argv[1])
if xlsx is None:
	xlsxname = re.sub(r'\.[A-Za-z]{3}$',r'\.xlsx', sys.argv[1])
	# call(['ssconvert', sys.argv[1],xlsxname])
	call(['ssconvert','--export-type=Gnumeric_Excel:xlsx',sys.argv[1],xlsxname])
	workbookname = xlsxname
else:
	workbookname = sys.argv[1]
workbook = openpyxl.load_workbook(workbookname)
sheets = get_sheet_names(workbook)
totalsheets = len(sheets)
o = open(csvlist_filepath, 'wb')
if totalsheets!=1:
	workbooks = {}
	# for y in [1:totalsheets]:
	for y in [totalsheets]:
		workbook[y] = workbook
    	# for x in [1:totalsheets]:
    	for x in [totalsheets]:
        	if x != y:
            		workbook[y].remove_sheet(x)
        cleanname = re.sub('[^a-zA-Z0-9\n\.]','',sheets[y+1])
        filename = cleanname+'.xlsx'
        workbook[y].save(filename)
        csvname = cleanname+'.csv'
        call(['in2csv',filename,'>',csvname])
        o.write(csvname+'\n')
else:
	csvname = re.sub(r'\.xlsx?',r'\.csv', sys.argv[1])
	call(['in2csv',workbookname,'>',csvname])
	o.write(csvname)
