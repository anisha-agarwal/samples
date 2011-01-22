file = open("TRAIN","r")

word_tag = {}
tag_freq = {}
prob = {}
wt_total = 0.0
word_list = []

tag = {}
tag_total = 0.0
tag_prob = {}

tags = []
tags_freq = {}
tags_prob = {}
start = {}
sequence = []
tags_tot = {}
start_prob = {}
start_total = 0.0

line_no = 0

for line in file:
    lines = line.rstrip('\n')
    words = lines.split(" ")
    current = ""
    for word in words:
        word = word.lstrip('"')
        word = word.rstrip('"')
        if current == "":
            current = word
            continue
        else:
            tags.append(word)
            if word not in word_tag:
                word_tag[word] = {}
            if current in word_tag[word]:
                word_tag[word][current] = word_tag[word][current] + 1
            else:
                word_tag[word][current] = 1
        current = word
        if word in tag:
            tag[word]  = tag[word] + 1
            tag_total = tag_total + 1
        else:
            tag[word] = 1
            tag_total = tag_total + 1
file.close()

test = open("TEST","r")
for line in test:
    lines = line.rstrip('\n')
    words = lines.split(" ")
    for word in words:
        word = word.lstrip('"')
        word = word.rstrip('"')
        if word not in word_list:
            word_list.append(word)
test.close()
    

for word in word_list:
    for tag1 in word_tag:
        if word in word_tag[tag1]:
            word_tag[tag1][word] = word_tag[tag1][word] + 1
        else:
            word_tag[tag1][word] = 1

tags_total = {}
for tags1 in word_tag:
    total = 0.0
    for words in word_tag[tags1]:
        total = total + word_tag[tags1][words]
    tags_total[tags1] = total

for tags1 in word_tag:
    if tags1 not in prob:
        prob[tags1] = {}
    for words in word_tag[tags1]:
        prob[tags1][words] = word_tag[tags1][words]/float(tags_total[tags1])

for word in tag:
    tag_prob[word] = tag[word]/tag_total


current = ""

for tag1 in tags:
    if current == "":
        current = tag1
        if tag1 in start:
            start[tag1] = start[tag1] + 1
        else:
            start[tag1] = 1
        start_total = start_total + 1
        continue
    else:
        if current not in tags_freq:
            tags_freq[current] = {}
                    
    if current == ".":
        if tag1 in start:
            start[tag1] = start[tag1] + 1
        else:
            start[tag1] = 1
        start_total = start_total + 1
        
    if tag1 in tags_freq[current]:
        tags_freq[current][tag1] = tags_freq[current][tag1] + 1
    else:
        tags_freq[current][tag1] = 1
    current = tag1

for words in tags_freq:
    total = 0.0
    for word in tags_freq[words]:
        total = total + tags_freq[words][word]
    tags_tot[words] = total

for words in tags_freq:
    tags_prob[words] = {}
    for word in tags_freq[words]:
        tags_prob[words][word] = tags_freq[words][word]/float(tags_tot[words])

for words in start:
    start_prob[words] = start[words]/float(start_total)

begin = open("alt_test","r")

for lines in begin:
    line_no = 0
    best_path = {}
    best_score = -1
    Q = {}
    line = lines.rstrip('\n')
    words = lines.split(" ")
    current = ""
    for word in words:
        line_no = line_no + 1
        Q[line_no] = {}
        if line_no != 1:
            best_path[line_no] = {}
        word = word.lstrip('"')
        word = word.rstrip('"')
        if current == "":
            current = word
            for tag1 in word_tag:
                if tag1 in start_prob:
                    Q[1][tag1] = start_prob[tag1]*prob[tag1][current]
                else:
                    Q[1][tag1] = 0
            continue
    
        for tag1 in word_tag:
            if line_no != 1:
                Q[line_no][tag1] = 0
                best_path[line_no] = {}
                for tag2 in word_tag:
                    if tag2 in tags_prob[tag1]:
                        if tag1 in prob:
                            if word in prob[tag1]:
                                if tag1 in Q[line_no - 1]:
                                    r = tags_prob[tag1][tag2]*prob[tag1][word]*Q[line_no - 1][tag2]
                                    if r > best_score:
                                        best_score = r
                                        best_path[line_no][line_no - 1] = tag2
                                        Q[line_no][tag1] = r
                                else:
                                    r = -1
    print Q
        
            
              
                
        
    

                    
            



    



