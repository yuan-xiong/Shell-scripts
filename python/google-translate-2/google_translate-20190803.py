import csv
import requests
import sys

#reload(sys)
#sys.setdefaultencoding('utf-8')

# https://translation.googleapis.com/language/translate/v2?key=YOUR_API_KEY

# simple way: ok
def translate(keywords, target):
    source_language = 'en'
    target_language = target

    content = 'http://translate.google.cn/translate_a/single?client=gtx&sl=' + source_language + '&tl=' + target_language + '&dt=t&q=' + keywords
    response = requests.post(content)
    res = response.json()

    search = res[0][0][1]
    result = res[0][0][0]
    print(search, result)
    return result
    

#def translate(keywords, target):
#    language_type = ""
#    url = "https://translation.googleapis.com/language/translate/v2"
#    data = {
#        'key': '', 
#        'source': language_type,
#        'target': target,
#        'q': keywords,
#        'format': "text"
#    }
#    #response = requests.post(url, data)
#
#    res = response.json()
#
#    result = res
#    #result = res["data"]["translations"][0]["translatedText"]
#    print(result)
#    return result

if __name__ == '__main__':
    keywords = "pancake"
    target = 'zh-CN'
    translate(keywords, target)
