
"""Solution to Justin.tv question 1: spellchecker
"""

__author__ = "Vijay Rajakumar(rvijax[at]gmail[dot]com)"
__date__ = "$Date: 2010/01/17 21:57:19 $"

from itertools import *
import os,sys,re


file_path="/usr/share/dict/words"

class Trie:
	"""Trie to hold characters
	"""
	
	def __init__(self):
		self.root=[None,{}]
			
	def clean(self,word):
		"""remove repeated characters and put 'a' for all vowels
		"""
		foo = lambda ch : 'a' if ch in ['a','e','i','o','u'] else ch
		word=''.join([foo(ch) for ch in word.lower()])
		return ''.join([x for x,y in groupby(word)])
	
	def add(self,word):
		"""adds clean(word) to Trie, leaf node has word for reading back
		"""
		cur_node = self.root
		for ch in self.clean(word):
			cur_node=cur_node[1].setdefault(ch,[None,{}])
		cur_node[0]=word
		
	def find(self,word):
		"""returns 1 correction for word, if its found,
			else returns NO SUGGESTION 
		"""
		cur_node = self.root
		for ch in self.clean(word):
				try:
					cur_node=cur_node[1][ch]
				except KeyError:
					return "NO SUGGESTION"
		return cur_node[0] if cur_node[0]!=None else "NO SUGGESTION"
						
		
if __name__ =="__main__":
	print "building index..."
	t=Trie()
	##read from "/usr/share/dict/words"
	wordlist=open(os.path.realpath(file_path))
	words=wordlist.readlines()
	wordlist.close()
	
	##clean wordlist for Apostrophes and non-english characters
	regex=lambda x:re.match("[\w]*",x.lower()).group(0)
	cleanwords=set(regex(x) for x in words)
	
	##fill Trie with words
	for word in cleanwords:
		t.add(word)	
	del cleanwords
	
	print "kill by KeyboardInterrupt"	
	##reads from stdin and outputs suggestions
	while(True):
		try:
			print t.find(regex(raw_input(">")))
		except EOFError,KeyboardInterrupt:
			print '\n----BYE----'
			sys.exit(0)
		
