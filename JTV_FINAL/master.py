"""Solution to Justin.tv Question 1: SpellChecker-Extra credits

"""
__author__ = "Vijay Rajakumar(rvijax[at]gmail[dot]com)"
__date__ = "$Date: 2010/01/17 21:57:19 $"

from subprocess import Popen, PIPE, STDOUT
import os,random

file_path="/usr/share/dict/words"

randomCapitalise=lambda word: ''.join([letter.upper() if letter in random.sample(word,random.randint(0,len(word))) else letter.lower() for letter in word])
randomRepeater=lambda word: ''.join([letter*random.randint(1,5) if letter in random.sample(word,random.randint(0,len(word))) else letter for letter in word])
randomFlipVowels=lambda word: ''.join([random.choice('aeiou') if letter in 'aeiou' else letter for letter in word])
typo=lambda word: randomCapitalise(randomRepeater(randomFlipVowels(word))).rstrip('\n')

if __name__ =="__main__":
	print "Creating words with typos..\n"

	dictlist=open(os.path.realpath(file_path))
	dict=dictlist.readlines()
	dictlist.close()

	selectWords=random.sample(dict,25)
	del dict
		
	words=[typo(word) for word in selectWords]		
	print '\n'.join([selectWord+"-->"+word for selectWord,word in zip(selectWords,words)])

	#print words
	print "\nPiping to spellchecker.py\n"
	p = Popen(['python','spellcheker.py'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
	grep_stdout = p.communicate(input='\n'.join(words))[0]

	print(grep_stdout)
	if grep_stdout.find("NO SUGGESTION")==-1:
		print "no\"NO SUGGESTIONS\" found!!!"
	else:
		print "Error!!Puppies thrown off cliff!!"

