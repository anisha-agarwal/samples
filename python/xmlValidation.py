import os

def xmlvalidation(string):
    
    flag = 0

    tag_temp = "" # To store the tag temporarily

    tags = {} # Contains the tag-names and the number of times they appear

    lines = string

    if lines.startswith("<") and lines.endswith(">"):
        for chars in lines:
            if chars == '<':
                tag_temp = "" 
                continue
            else:
                if chars == '>':
                    if tag_temp in tags:
                        if flag == 1:
                            if tag_temp not in tags:
                                print "invalid"
                                os._exit(1)
                            else:
                                flag = 0      
                                tags[tag_temp] = tags[tag_temp] + 1
                
                    else:
                        if flag == 1:
                            print "invalid"
                            os._exit(1)
                        else:
                            flag = 0
                            tags[tag_temp] = 1

                    tag_temp = ""
                    continue
            
                else:
                    if ord(chars) in range(0,59) or ord(chars) == 61 or ord(chars) in range(63,128):
                        if ord(chars) == 47:
                            flag = 1
                        else:
                            tag_temp= tag_temp + chars
            

    else:
        print "invalid 0"
        os._exit(1)

    for tag in tags:
        if tag == "":
            print "invalid 1"
            os._exit(1)
        else:
            if tags[tag]%2 != 0:
                print "invalid 2"
                print tags
                os._exit(1)
            else:
                print "valid"
                os._exit(1)

    
