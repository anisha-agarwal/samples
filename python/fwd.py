
from math import *

a = {}
count_tag = {}
count_wtag = {}
count_ttag = {}
tag_total = 0.0

prob_tag = {}
prob_wtag = {}
prob_ttag = {}



file2 = open("TEXT","r")

prob_first = {}
count_first = {}
list_first = []

count_end = {}
prob_end = {}
list_end = []

for line in file2:
    line = line.rstrip('\n')
    words = line.split(" ")
    first_word = words[0]
    last_word = words[len(words) - 1]
    if first_word not in list_first:
        list_first.append(first_word)
    if last_word not in list_end:
        list_end.append(last_word)
        
file2.close()

file = open("DICT","r")

current2 = ""
prv_tags =  []

for line in file:
    line = line.rstrip('\n')
    words = line.split("\t")
    #print words
    word = words[0]
    if current2 != "":
        prv_tags = current_tags
    current_tags = []
    for w1 in words:
        if w1 != words[0]:
            current_tags.append(w1)
    if current2 == "":
        prv_tags = current_tags
    current2 = word
    #print prv_tags
    #print current_tags
    for tag in prv_tags:
        if tag not in count_ttag:
            count_ttag[tag] = {}
        for tags in current_tags:
            if tags in count_ttag[tag]:
                count_ttag[tag][tags] = count_ttag[tag][tags] + 1
            else:
                count_ttag[tag][tags] = 1         
        
    current = ""
    for i in words:
        if current == "":
            current = i
            continue
        if current != "":
            if i not in count_first:
                count_first[i] = {}
                prob_first[i] = {}
            if i not in count_end:
                count_end[i] = {}
                prob_end[i] = {}
        if i in count_tag:
            count_tag[i] = count_tag[i] + 1
            tag_total = tag_total + 1
        if i not in count_tag:
            count_tag[i] = 1
            tag_total = tag_total + 1
        if current in list_first:
            if current in count_first[i]:
                count_first[i][current] = count_first[i][current] + 1
            else:
                count_first[i][current] = 1
        if i not in count_wtag:
            count_wtag[i] = {}
        if i in count_wtag:
            if word in count_wtag[i] and word not in count_first[i]:
                count_wtag[i][word] = count_wtag[i][word] + 1
            if word not in count_wtag[i] and word not in count_first[i]:
                #print word
                count_wtag[i][word] = 1
        if i == "." and current != "":
            if i not in count_end:
                count_end[i] = {}
            else:
                if word in count_end[i]:
                    count_end[i][word] = count_end[i][word] + 1
                else:
                    count_end[i][word] = 1
      
        current = i

for i in count_tag:
    prob_tag[i] = count_tag[i]/tag_total

total = {}

for i in count_wtag:
    total[i] = 0.0
    for word in count_wtag[i]:
        total[i] = total[i] + count_wtag[i][word]

for i in count_wtag:
    prob_wtag[i] = {}
    for word in count_wtag[i]:
        prob_wtag[i][word] = count_wtag[i][word]/float(total[i])

total_first = {}

for i in count_first:
    total_first[i] = 0.0
    for word in count_first[i]:
        total_first[i] = total_first[i] + count_first[i][word]

for i in count_first:
    prob_first[i] = {}
    for word in count_first[i]:
        prob_first[i][word] = count_first[i][word]/total_first[i]

total_end = {}

for i in count_end:
    total_end[i] = 0.0
    for word in count_end[i]:
        total_end[i] = total_end[i] + count_end[i][word]

for i in count_end:
    prob_end[i] = {}
    for word in count_end[i]:
        prob_end[i][word] = count_end[i][word]/total_end[i]

tag_total = {}
for i in count_ttag:
    tag_total[i] = 0
    for tags in count_ttag[i]:
        tag_total[i] = tag_total[i] + count_ttag[i][tags]

for i in count_ttag:
    prob_ttag[i] = {}
    for tags in count_ttag[i]:
        prob_ttag[i][tags] = count_ttag[i][tags]/float(tag_total[i])

#print count_ttag
#print count_wtag
#print prob_tag
#print prob_wtag
#print count_first
#print total_first
#print prob_first
#print prob_ttag
#print count_end
#print prob_end

file.close()

current = ""
for tag in prob_first:
    if current == "":
        current = tag
    a[tag] = {}
    for word in prob_first[tag]:
        a[tag][word] = log(prob_tag[tag])

prv_tag = ""

file3 = open("TEXT","r")

for line in file3:
    prv_word = ""
    line = line.rstrip('\n')
    words = line.split(" ")
    for word in words:
        if prv_word == "":
            prv_word = word
            continue
        if word != words[0]:
            for tag in prob_wtag:
                if tag not in a:
                    a[tag] = {}
                a[tag][word] = 0.0
                if tag in prob_ttag:
                    for tags in prob_wtag:
                        if tags in prob_ttag[tag]:
                            if prv_word in a[tags] and prv_word in prob_wtag[tags]:
                                temp = log(prob_ttag[tag][tags]) + log(prob_wtag[tags][prv_word]) + a[tags][prv_word]
                                if a[tag][word] > temp or a[tag][word] == temp:
                                    a[tag][word] = a[tag][word] + log(1 + exp(temp - a[tag][word]))
                                else:
                                    if temp > a[tag][word]:
                                        a[tag][word] = temp + log(1 + exp(a[tag][word] - temp))
        prv_word = word

#print a

file3.close()

b = {}

current = ""
for tag in prob_end:
    if current =="":
        current = tag
    b[tag] = {}
    for word in prob_end[tag]:
        b[tag][word] = log(prob_tag[tag])
    

file4 = open("TEXT","r")


for line in file4:
    line = line.rstrip('\n')
    words = line.split(" ")
    words.reverse()
    i = 0
    for word in words:
        i = i + 1
        if word == words[0]:
            continue
        if i < (len(words) - 1):
            curr_word = words[i]
            next_word = words[i + 1]
            for tag in prob_wtag:
                if tag in prob_ttag:
                    for tags in prob_wtag:
                        if tags not in b:
                            b[tags] = {}
                        b[tags][next_word] = 0.0
                        if tags in prob_ttag[tag]:
                            if next_word in prob_wtag[tags] and curr_word in b[tag]:
                                temp = log(prob_ttag[tag][tags]) + log(prob_wtag[tags][next_word]) + b[tag][curr_word]
                                if b[tags][next_word] > temp or b[tags][next_word] == temp:
                                    b[tags][next_word] = b[tags][next_word] + log(1 + exp(temp - b[tags][next_word]))
                                else:
                                    if b[tags][next_word] < temp:
                                        b[tags][next_word] = temp + log(1 + exp(b[tags][next_word] - temp))

for i in b:
    for j in b[i]:
        if b[i][j] != 0.0:
            print i,j

print "a starts"

for i in a:
    for j in a[i]:
        if a[i][j] != 0.0:
            print i,j
            
                            
                            
                    
                    
                        
                        
        
    

