import random
import sys
from sets import Set
from datetime import datetime

D1 = {}
D1['S'] = [ ['NP','VP'] ]
D1['NP'] = [ ['John'],['Det','N'],['NP','PP'] ]
D1['Det'] = [ ['the'] ]
D1['N'] = [ ['man'],['telescope'],['house'],['tree'] ]
D1['VP'] = [ ['V','NP'],['VP','PP'] ]
D1['V'] = [ ['saw'] ]
D1['PP'] = [ ['P','NP'] ]
D1['P'] = [ ['with'],['in'],['under'] ]

D2 = {}
D2[ str(['NP','VP']) ] = ['S']
D2[ str(['John']) ] = ['NP']
D2[ str(['Det','N']) ] = ['NP']
D2[ str(['NP','PP']) ] = ['NP']
D2[ str(['the']) ] = ['Det']
D2[ str(['man']) ] = ['N']
D2[ str(['telescope']) ] = ['N']
D2[ str(['house']) ] = ['N']
D2[ str(['tree']) ] = ['N']
D2[ str(['V','NP']) ] = ['VP']
D2[ str(['VP','PP']) ] = ['VP']
D2[ str(['saw']) ] = ['V']
D2[ str(['P','NP']) ] = ['PP']
D2[ str(['with']) ] = ['P']
D2[ str(['in']) ] = ['P']
D2[ str(['under']) ] = ['P']

words = {}
words['1'] = ['John']
words['2'] = ['the']
words['3'] = ['man']
words['4'] = ['telescope']
words['5'] = ['house']
words['6'] = ['tree']
words['7'] = ['saw']
words['8'] = ['with']
words['9'] = ['in']
words['10'] = ['under']

agenda = []

sentence1 = ['John','saw','the','man']
sentence2 = ['John','saw','the','man','with','the','telescope']
sentence3 = ['John','saw','the','man','with','the','telescope','in','the','house']
sentence4 = ['John','saw','the','man','with','the','telescope','in','the','house','under','the','tree']
sentence = ['John','saw','the','man','with','the','telescope','in','the','house','under','the','tree','with','the','telescope']

class State(object) :
    def __init__(self) :
        # value of i
        self.i = None
        # value of j
        self.j = None
        # Alpha value
        self.alpha = []
        # Beta value
        self.beta = []
        # the bpset
        self.bpset = Set()
        # the index in chart
        self.idx = 0
        
    def __str__(self):
        return str(self.i)+" "+str(self.alpha)+" * "+str(self.beta)+" " +str(self.j)

    def equal(s1,s2):
        return s1.i==s2.i and s1.j==s2.j and s1.alpha==s2.alpha and s1.beta==s2.beta

# chart
class Chart(object):
    """
    indexes implemented using string(id1+id2)->index in states array
    we keep pointers(indices) to the actual states array. This way we don't make a copy
    """

    def __init__(self):
        # the list of states
        self.states = []

        # index1 search for all i,a*Xy,j given X,j
        self.index1 = {}

        # index2 search for all j,b*,k given b,j
        self.index2 = {}

    # search in index1
    def find1(self,X,j):
        ret_list = []
        key = str(X)+str(j)
        #print 'find1 ', key
        if not self.index1.has_key(key):
            return ret_list
        idxs = self.index1[str(X)+str(j)]
        for idx in idxs:
            ret_list.append(self.states[idx])
        return ret_list

    # search in index2
    def find2(self,b,j):
        ret_list = []
        key = str(b) + str(j)
        #print 'find2 ', key
        if not self.index2.has_key(key):
            return ret_list
        idxs = self.index2[key]
        for idx in idxs:
            ret_list.append(self.states[idx])
        return ret_list

    # dumb find
    def find(self,s):
        for state in self.states:
            if equal(state,s):
                return True
        return False

    # index of next add
    def index(self,s):
        # old state
        for i in range(0,len(self.states)):
            if equal(s,self.states[i]):
                return i

        # new state
        return len(self.states)

    # add new state
    def add(self,s):
        if self.find(s):
            return False
        idx = len(self.states)
        s.idx = idx
        self.states.append(s)
        if len(s.beta)>0:
            key = str(s.beta[0]) + str(s.j)
            #print 'added ', key, ' ' , s
            if not self.index1.has_key(key):
                self.index1[key] = []
            self.index1[key].append(idx)

        if 0==len(s.beta):
            key = str(s.alpha) + str(s.i)
            #print 'added ', key, ' ' , s
            if not self.index2.has_key(key):
                self.index2[key] = []
            self.index2[key].append(idx)
        return True

chart = Chart()

def add_state(s):
    for state in agenda:
        if equal(s,state):
            # merge bpsets
            state.bpset.update(s.bpset)
            return False
    if chart.find(s):
        return False

    #print 'adding to agenda: ',s
    agenda.append(s)
    return True

def predict(S1) :
    if len(S1.beta)==0:
        return

    a = S1.beta[0]
    #print "PREDICT ", S1

    if not D1.has_key(a):
        return
    for rule in D1[a] :
        #print 'rule: ',rule
        newState = State()
        newState.beta = rule[:]
        newState.i = S1.j
        newState.j = S1.j
        if add_state(newState):
            print "PREDICT: ",newState

def shift(S1,idx) :
    if len(S1.beta)==0:
        return

    a = S1.beta[0]
    if S1.j>=len(sentence):
        return

    if a == sentence[S1.j] :
        #print "SHIFT ",S1
        newState = State()
        newState.alpha = S1.alpha[:]
        newState.alpha.append(a)
        newState.i = S1.i
        newState.j = S1.j + 1
        newState.bpset = Set([idx])
        if add_state(newState):
            print "SHIFT: ",newState

def complete1(S1,idx) :
    if len(S1.beta) == 0 :
        return
    X = S1.beta[0]
    if not D1.has_key(X):
        return

    # find all possible antecedants
    for beta in D1[X]:
        states = chart.find2(beta,S1.j)
        for state in states:
            #print 'complete1 : ', state
            res = State()
            res.i = S1.i
            res.j = state.j
            res.alpha = S1.alpha[:]
            res.alpha.append(X)
            res.beta = S1.beta[:]
            res.beta.pop(0)
            res.bpset = Set( [(idx,state.idx)] )
            if add_state(res):
                print 'COMPLETE: ',S1,' + ',state,' -> ',res,'\n'

def complete2(S1,idx) :
    if len(S1.alpha) == 0 or len(S1.beta) != 0:
        return

    beta = S1.alpha
    if not D2.has_key(str(beta)):
        return

    # find all possible antecedants
    for X in D2[str(beta)]:
        states = chart.find1(X,S1.i)
        for state in states:
            #print 'complete2 : ', state
            res = State()
            res.i = state.i
            res.j = S1.j
            res.alpha = state.alpha[:]
            res.alpha.append(X)
            res.beta = state.beta[:]
            res.beta.pop(0)
            res.bpset = Set( [(idx,state.idx)] )
            if add_state(res):
                print 'COMPLETE: ', S1,' + ',state,' -> ',res

# start state
S = State()
S.i = 0
S.beta.append('S')
S.j = 0

# goal state
goal = State()
goal.i = 0
goal.j = len(sentence)
goal.alpha = ['S']
goal.beta = []

# initialize
agenda.append(S)
length = len(agenda)
S1 = State()

start_time = datetime.now()

while(length != 0):
    #print 'length: ', length
    S1 = agenda[0]
    print "trigger: ",S1
    # index of next add, used in bpset
    idx = chart.index(S1)
    #print 'idx ',idx

    predict(S1)
    shift(S1,idx)
    complete1(S1,idx)
    complete2(S1,idx)

    chart.add(S1)
    agenda.pop(0)
    length = len(agenda)

end_time = datetime.now()

found = False
for s in chart.states:
    if equal(s,goal):
        print 'FOUND: ',s
        found = True
    #else:
        #print s

if not found:
    sys.exit()

# index of the goal state
iter_idx = chart.index(goal)

# recursive print function. If there are multiple options, chooses a random path
def print_tree(idx):
    #print 'print_tree ',idx
    state = chart.states[idx]
    #print 'bpset ', len(state.bpset)

    set_idx = 0

    if len(state.bpset) > 1:
        set_idx = random.randrange(0,len(state.bpset))
        #print 'random ' , set_idx

    count = 0
    for dep in state.bpset:
        if count != set_idx:
            count = count+1
            continue
        #print dep
        if type(dep)==type((1,2)):
            return print_tree(dep[1]) + ' ( ' + state.alpha[len(state.alpha)-1] + print_tree(dep[0]) + ')'
        else:
            return print_tree(dep) + ' ' + state.alpha[len(state.alpha)-1]

    return ''

# recursive function to count the number of possible trees in a sentence
# idea is to use the bpsets and count the possible paths that can be taken
# if there are 2 possible options at a node, we recursively count the possible subtrees. Each of the
# subtrees can be combined with one another, so the total possible options is a product of the subtree
# counts.

def count_tree(idx):
    state = chart.states[idx]
    #print 'bpset ', len(state.bpset)

    count = 0
    for dep in state.bpset:
        #print dep
        if type(dep)==type((1,2)):
            count = count + count_tree(dep[1])*count_tree(dep[0])
        else:
            count = count + count_tree(dep)
    if count == 0:
        count = 1
    return count

print print_tree(iter_idx) + '\n'
print 'trees count: ', count_tree(iter_idx) , '\n'

print 'time: ' , (end_time.second - start_time.second) , 'sec ', (end_time.microsecond -
start_time.microsecond), 'msec'
