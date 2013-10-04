from subprocess import call
import openpyxl
import re
# argv for filename here
xlsx = re.search(r'\.xlsx',argv[1])
if xlsx is None:
	xlsxname = re.sub(r'\.[A-Za-z]{3}$',r'\.xlsx',argv[1])
	call(['ssconvert',argv[1],xlsxname])
	workbookname = xlsxname
else:
	workbookname = argv[1]
workbook = load_workbook(workbookname)
sheets = get_sheet_names(workbook)
totalsheets = len(sheets)
o = open('csvlist', 'wb')
if totalsheets!=1:
	workbooks = {}
	for y in [1:totalsheets]:
		workbook[y] = workbook
    	for x in [1:totalsheets]:
        	if x != y:
            	workbook[y].remove_sheet(x)
        cleanname = re.sub('[^a-zA-Z0-9\n\.]','',sheets[y+1])
        filename = cleanname+'.xlsx'
        workbook[y].save(filename)
        csvname = cleanname+'.csv'
        call(['in2csv',filename,'>',csvname])
        o.write(csvname+'\n')
else:
	csvname = re.sub(r'\.xlsx?',r'\.csv',argv[1])
	call(['in2csv',workbookname,'>',csvname])
	o.write(csvname)