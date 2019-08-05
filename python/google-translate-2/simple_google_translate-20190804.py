import csv
import os
import requests
import sys
import time

#import xlrd
import pandas as pd
import numpy as np

sleep_time  = 3

ERR_429     = 'request_err_429'
ERR_UNKNOWN = 'request_err_unknown'

new_xlsx_name  = 'google_translate.xlsx'
new_sheet_name = 'google_translate_sheet'

# https://www.jb51.net/article/163320.htm
def showExcelSheetHead(data_frame):
    #print('----------------------------------------------------------------------------------------')
    data = data_frame.head()
    print("[+] showExcelSheetHead:\n{}".format(data))

# https://blog.csdn.net/weixin_43245453/article/details/90747259
def selectExcelSheet(file_path):
    print('----------------------------------------------------------------------------------------')
    print("[+] selectExcelSheet")

    data  = pd.read_excel(file_path, None)
    #print("sheets\t\t{}".format(data.keys()))

    sheets = []
    for i, sheet_name in enumerate(data.keys()):
        sheets.append(sheet_name)
        print("\t[-] sheet {}:\t{}".format(i, sheet_name))

    chooice = int(input('Please select a sheet [sheet number] '))
    return sheets[chooice]

def selectExcelTitle(file_path, sheet_name):
    print('----------------------------------------------------------------------------------------')
    print("[+] selectExcelColumn")
    #data_frame = pd.DataFrame(pd.read_excel(file_path, sheet_name))
    sheet_content = pd.DataFrame(pd.read_excel(file_path, sheet_name))

    showExcelSheetHead(sheet_content)
    #print("sheet content\n{}".format(sheet_content))
    #print("{}".format(sheet_content))

    titles = []
    # deprecated
    #titles = sheet_content.ix[0]
    #titles = sheet_content.loc[0]
    #titles = sheet_content.iloc[0]

    #print(sheet_content.index)#获取行的索引名称
    #print(sheet_content.columns)#获取列的索引名称
    titles.append( (sheet_content.columns)[0] )
    titles.append( (sheet_content.columns)[1] )

    print()
    for i, title in enumerate(titles):
        #print('titles type:\t{}'.format(type(titles)))
        print('\t[-] title {}:\t{}'.format(i, title))


    chooice = int(input('Please select a column (keywords_title) to translate [column number] '))
    return titles[chooice], sheet_content

def getExcelShape(data_frame):
    return data_frame.shape
def getExcelRows(data_frame):
    return data_frame.shape[0]
def getExcelColumns(data_frame):
    return data_frame.shape[1]

# http://pytolearn.csd.auth.gr/b4-pandas/40/moddfcols.html
def insertExcelRow(data_frame, row_index, row_list):
    # insert a row
    data_frame.loc[row_index] = row_list

def insertExcelColumn(data_frame, column_index, column_name, column_list):
    # insert a column
    data_frame.insert(column_index, column_name, column_list)

def appendExcelColumn(data_frame, column_name, column_list):
    # insert a column
    #data_frame[column_name] = column_list

    # insert empty column
    data_frame[column_name] = pd.Series()

# https://pandas.pydata.org/pandas-docs/stable/reference/api/pandas.DataFrame.replace.html
# https://blog.csdn.net/sscc_learning/article/details/76993816
def updateExcelCell(data_frame, row_index, column_name, value):
    #data_frame.at[0, 'try_1'] = 666
    #data_frame['try_1'][0] = 'my_value'
    data_frame[column_name][row_index] = value



# Translate Excel columns
def translateExcelColumns(data_frame, keywords_title):
    print('----------------------------------------------------------------------------------------')
    print("[+] translateExcelColumns")

    global sleep_time
    global ERR_429
    global ERR_UNKNOWN
    global new_xlsx_name
    global new_sheet_name

    if new_sheet_name == '':
        new_xlsx_name  = input('Please name the new xlsx  [xlsx  name] ')
    if new_sheet_name == '':
        new_sheet_name = input('Please name the new sheet [sheet name] ')

    #keywords_column_name = '英文'
    translation_title = 'Google Translate'

    #data_frame = pd.DataFrame(pd.read_excel(file_path))
    #data_frame = pd.DataFrame(pd.read_excel(file_path, sheet_name))

    #showExcelSheetHead(data_frame)

    #insertExcelColumn(data_frame, 3, "try_1", [1,2,3,4,5])
    #appendExcelColumn(data_frame, "try_1", [1,2,3,4,5])
    #appendExcelColumn(data_frame, translation_title, [1,2,3,4,5])
    appendExcelColumn(data_frame, translation_title, [])
    #appendExcelColumn(data_frame, "try_1", None)

    #updateExcelCell(data_frame, 2, "try_1", "replace_value")

    #print("data_frame.index:\t{}".format(data_frame.index))
    #print("data_frame.columns:\t{}".format(data_frame.columns))
    #print(data_frame['英文'])
    #print("line 1")
    #print(data_frame.loc[0])
    #print("line 2")
    #print(data_frame.loc[1])

    nrows = getExcelRows(data_frame)
    print('[*] number of keywords to be translated:\t{}\n\n'.format(nrows))

    for i in range(0, nrows):
    #for i in range(1, nrows):
        keywords = data_frame[keywords_title][i]
        #print("[-] translate keywords:", keywords)
        res = translateLine(keywords)

        if res == ERR_429:
            print('(T_T) You are requesting "Google Translate" too frequently, please have a rest...')
            return
        elif res == ERR_UNKNOWN:
            print('(T_T) Unknown error happened')
            return

        #updateExcelCell(data_frame, i, "try_1", res)
        #updateExcelCell(data_frame, i, '译文', res)
        updateExcelCell(data_frame, i, translation_title, res)

        if i % 10 == 0:
            #data_frame.to_excel("new.xlsx")
            #data_frame.to_excel("new.xlsx", sheet_name=new_sheet_name)
            data_frame.to_excel(new_xlsx_name, sheet_name=new_sheet_name)

        # Avoid frequent requests, sleep 1 second
        time.sleep(sleep_time)

# simple way: ok
#    https://blog.csdn.net/zcoutman/article/details/69062422
def translateLine(keywords):
    #print('-------------------')
    print("\t[-] translateLine:\t{}".format(keywords))
    #print("[-] translateLine:")

    global ERR_429
    global ERR_UNKNOWN

    # 1. isnan
    if type(keywords) == float:
        if np.isnan(keywords):
            #print("[-] keywords is nan:", keywords)
            return ''
           
    # 2. strip blank space
    keywords = str(keywords).strip()

    # 3. skip empty keywords
    if keywords == None:
        #print("[-] keywords is None:", keywords)
        return ''
    elif len(keywords) == 0:
        #print("[-] keywords len is 0:", keywords)
        return ''
    elif keywords.isspace():
        #print("[-] keywords is blank space:", keywords)
        return ''

    # 4. translate
    source_language = 'en'
    target_language = 'zh-CN'

    content = 'http://translate.google.cn/translate_a/single?client=gtx&sl=' + source_language + '&tl=' + target_language + '&dt=t&q=' + keywords
    response = requests.post(content)
    #print("[-] response: ", response)
    #print("[-] response type: ", type(response))
    #print("[-] response status: ", response.status_code)
    #exit()

    if response.status_code == 200:
        res = response.json()
        #print("[-] res: ", res)

        search = res[0][0][1]
        result = res[0][0][0]
        #print("{} -> {}".format(search, result))
        print("\t\t\t\t---> {}".format(result))

    elif response.status_code == 429:
        result = ERR_429
    else:
        result = ERR_UNKNOWN

    return result
    
def selectExcelFile():
    print('----------------------------------------------------------------------------------------')
    print("[+] selectExcelFile")

    #os.system("ls -l | grep xlsx | awk '{print $9}'")
    
    #files = os.popen("ls -l | grep xlsx | awk '{print $9}'").read()
    #print('files:\n{}'.format(files))
    #print(files)
    
    files = []
    for i, file in enumerate(os.listdir('./')):
        files.append(file)
        if file.endswith('.xlsx'):
            #print('files {}\t{}'.format(i, files[i]))
            print('\t[-] file {}:\t{}'.format(i, file))

    #print()
    chooice = int(input('Please select a xlsx file [file number] '))
    return files[chooice]


def translateOneWords(keywords):
    translateLine(keywords)


def translateExcel():
    file_path  = selectExcelFile()
    print('[*] file_path:\t{}'.format(file_path))

    sheet_name = selectExcelSheet(file_path)
    print('[*] sheet_name:\t{}'.format(sheet_name))

    keywords_title, data_frame = selectExcelTitle(file_path, sheet_name)
    print('[*] keywords_title:\t{}'.format(keywords_title))

    #file_path = sys.argv[1]
    #column   = int(sys.argv[2])
    #print("[+] Target file\t\t{}".format(file_path))
    #print("[-] Column\t\t{}".format(column))

    pd.set_option('mode.chained_assignment', None)
    #translateExcelColumns(file_path, column_index)
    translateExcelColumns(data_frame, keywords_title)
    

# start from here
if __name__ == '__main__':

    #translateOneWords('pancake')
    #translateOneWords('Preorder traversal')
    #translateOneWords('Engineering Graphics and Computer Graphics Drawings')

    translateExcel()

    print('----------------------------------------------------------------------------------------')

