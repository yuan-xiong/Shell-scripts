import csv
import requests
import sys

import xlrd

# Translate Excel columns
#    https://blog.csdn.net/allan2222/article/details/88394214
def translateExcelColumns(filepath, column):
    data  = xlrd.open_workbook(filepath, encoding_override='utf-8')
    table = data.sheets()[0]
    nrows = table.nrows
    ncols = table.ncols

    #for i in range(0, nrows):
    for i in range(1, nrows):
        line = table.row_values(i)
        keywords = line[column]

        # strip blank space
        keywords = keywords.strip()

        # empty keywords
        if keywords == None:
            print("[-] keywords is None:", keywords)
        elif len(keywords) == 0:
            #print("[-] keywords len is 0:", keywords)
            print
        elif keywords.isspace():
            print("[-] keywords is blank space:", keywords)
        else:
            #print("[-] translate keywords:", keywords)
            translateWords(keywords)

# simple way: ok
#    https://blog.csdn.net/zcoutman/article/details/69062422
def translateWords(keywords):
    source_language = 'en'
    target_language = 'zh-CN'

    content = 'http://translate.google.cn/translate_a/single?client=gtx&sl=' + source_language + '&tl=' + target_language + '&dt=t&q=' + keywords
    response = requests.post(content)
    res = response.json()
    #print("[-] res: ", res)

    search = res[0][0][1]
    result = res[0][0][0]
    print("{} -> {}".format(search, result))
    return result
    
if __name__ == '__main__':

    #keywords = "pancake"
    #translateWords(keywords)

    filepath = sys.argv[1]
    column   = int(sys.argv[2])
    print("[+] Target file\t\t{}".format(filepath))
    print("[-] Column\t\t{}".format(column))

    translateExcelColumns(filepath, column)
    
