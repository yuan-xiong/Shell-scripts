#!/home/xy18/anaconda3/envs/text2image27/bin/python
# -*- coding: utf-8 -*-

import json
import os
import re
import socket
import time
import urllib
import urllib2

timeout = 5
socket.setdefaulttimeout(timeout)

# user-agent
#	1. about:version
#	2. f11 + click link: look into the request header
#		User-Agent Mozilla/5.0 (X11; Ubuntu; Linu…) Gecko/20100101 Firefox/65.0

class Crawler:
	__time_sleep = 0.1
	__amount = 0
	__start_mount = 0
	__counter = 0

	def __init__(self, t=0.1):
		self.time_sleep = t

	def getFix(self, name):
		m = re.search(r'\.[^\.]*$', name)
		if m.group(0) and len(m.group(0)) <= 5:
			return m.group(0)
		else:
			return '.jpeg'

	def downloadImage(self, info, word):
		time.sleep(self.time_sleep)
		fix = self.getFix(info['objURL'])
		#savepath = '~/tools/crawler/' + word + '/' + str(self.__counter) + str(fix)
		savepath = '/home/xy18/tools/crawler/' + 'word' + '/' + str(self.__counter) + str(fix)
		savepath = savepath.decode('utf-8').encode('gb2312')
		urllib.urlretrieve(info['objURL'], savepath)

	def saveImages(self, json, word):
		cnname = word.decode('utf-8').encode('gb2312')
		savefolder = "/home/xy18/tools/crawler/word"
		#savefolder = "~/tools/crawler/" + word
		savefolder = savefolder.decode('utf-8').encode('gb2312')
		if not os.path.exists(savefolder):
			os.mkdir(savefolder)
		self.__counter = len(os.listdir(savefolder)) + 1

		for info in json['imgs']:
			try:
				if self.downloadImage(info, word) == False:
					self.__counter -= 1

			except urllib2.HTTPError as urllib_err:
				print(urllib_err)
				pass
			except Exception as err:
				time.sleep(1)
				print(err);
				continue
			finally:
				showstr = word + ": picture + 1 already have " + str(self.__counter) + " images"
				print(showstr.decode('utf-8').encode('gb2312'))
				self.__counter += 1
		return

	def getImages(self, word=u'美女'):
		search = urllib.quote(word)
		pn = self.__start_amount

		headers = {'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linu…) Gecko/20100101 Firefox/65.0'}
		while pn < self.__amount:
			url = 'http://image.baidu.com/search/avatarjson?tn=resultjsonavatarnew&ie=utf-8&word=' + search + '&cg=girl&pn=' + str(
                pn) + '&rn=60&itg=0&z=0&fr=&width=&height=&lm=-1&ic=0&s=0&st=-1&gsm=1e0000001e'
			try:
				time.sleep(self.time_sleep)
				req = urllib2.Request(url=url, headers=headers)
				page = urllib2.urlopen(req)
				data = page.read().decode('utf8')
			except UnicodeDecodeError as e:
				print('-----UnicodeDecodeErrorurl:', url)
			except urllib2.URLError as e:
				print("-----urlErrorurl:", url)
			except socket.timeout as e:
				print("-----socket timout:", url)
			else:
				json_data = json.loads(data)
				self.saveImages(json_data, word)
				print(u"Next Page...")
				pn += 60
			finally:
				page.close()
		print('Download finished.')
		return



	def start(self, word, spider_page_num=1, start_page=1):
		print('word', word)
		print('spider_page_num', spider_page_num)
		print('start_page', start_page)
		self.__start_amount = (start_page - 1) * 60
		print('__start_amount', self.__start_amount)
		self.__amount = spider_page_num * 60 + self.__start_amount
		print('__amount', self.__amount)
		self.getImages(word)

people = ['上户彩', '新垣结衣']
crawler = Crawler(0.05)

for person in people:
	crawler.start(person, 1)

