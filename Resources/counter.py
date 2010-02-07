import random
inFile=open("/home/vijay/workplace/Resources/recordList.txt","r")
outFile=open('/home/vijay/workplace/Resources/ranking.txt','w')
count=0
for line in inFile.readlines():
	count=count+1
print count

ranks=range(count)
random.shuffle(ranks)

for i in ranks:
	outFile.write(str(i)+'\n')

