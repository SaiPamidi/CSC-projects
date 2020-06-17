
from operator import *

class HuffmanNode:
    def __init__(self, char, freq):
        self.char = char   # stored as an integer - the ASCII character code value
        self.freq = freq   # the freqency associated with the node
        self.left = None   # Huffman tree (node) to the left
        self.right = None  # Huffman tree (node) to the right

    def set_left(self, node):
        self.left = node

    def set_right(self, node):
        self.right = node

    def __lt__(self, other):
        """Less-than comparison."""
        if self.freq < other.freq:
            return True
        elif self.freq == other.freq:
            if self.char < other.char:
                return True
            else:
                return False

        else:
            return False


def comes_before(a, b):
    """Returns True if tree rooted at node a comes before tree rooted at node b, False otherwise"""
    if a<b:
        return True
    else:
        return False


def combine(a, b):
    """Creates and returns a new Huffman node with children a and b, with the "lesser node" on the left
    The new node's frequency value will be the sum of the a and b frequencies
    The new node's char value will be the lesser of the a and b char ASCII values"""

    Newchr = min(a.char,b.char)
    if comes_before(a, b):

        newTree = HuffmanNode(Newchr,a.freq+b.freq)
        newTree.set_left(a)
        newTree.set_right(b)

    else:

        newTree = HuffmanNode(Newchr, a.freq + b.freq)
        newTree.set_left(b)
        newTree.set_right(a)

    return newTree






def cnt_freq(filename):
    """Opens a text file with a given file name (passed as a string) and counts the 
    frequency of occurrences of all the characters within that file"""
    FreqList = [0]*256
    try:
        fileObj = open(filename,'r')
    except:
        raise FileNotFoundError
    for line in fileObj:

        for i in line:
            FreqList[ord(i)] = FreqList[ord(i)] + 1
    fileObj.close()


    return FreqList







def create_huff_tree(char_freq):
    """Create a Huffman tree for characters with non-zero frequency
    Returns the root node of the Huffman tree"""
    Nonzero_charFreqTree = []
    for i in range(len(char_freq)):
        if char_freq[i] !=0 :
            Nonzero_charFreqTree.append(HuffmanNode(i,char_freq[i]))

    Nonzero_charFreqTree.sort()

    while len(Nonzero_charFreqTree)>1:
        lengthList = len(Nonzero_charFreqTree)
        NewNode = combine(Nonzero_charFreqTree[0],Nonzero_charFreqTree[1])
        Nonzero_charFreqTree = Nonzero_charFreqTree[2:(lengthList+1)]
        Nonzero_charFreqTree.append(NewNode)
        Nonzero_charFreqTree.sort()

    return Nonzero_charFreqTree[0]








def create_code(node):
    """Returns an array (Python list) of Huffman codes. For each character, use the integer ASCII representation 
    as the index into the array, with the resulting Huffman code for that character stored at that location"""
    listA = []
    str1 =''

    treeRec(node,listA,str1)
    #print(listA)
    #print(chr(100))
    codes =['']*256
    for i in range(0,len(listA),2):
        codes[listA[i]] = listA[i+1]
    return codes


def treeRec(node,listA,str1):
    if node.left == None and node.right == None:
        listA.append(node.char)
        listA.append(str1)
        str1 = ''
    if node.left != None:
        treeRec(node.left,listA,str1+'0')
    if node.right != None:
        treeRec(node.right,listA,str1+'1')





def create_header(freqs):
    """Input is the list of frequencies. Creates and returns a header for the output file
    Example: For the frequency list asscoaied with "aaabbbbcc, would return “97 3 98 4 99 2” """
    HeaderList = []
    for i in range(len(freqs)):
        if freqs[i] != 0:
            HeaderList.append(str(i))
            HeaderList.append(str(freqs[i]))

    headerStr = ' '.join(HeaderList)

    return headerStr







def huffman_encode(in_file, out_file):
    """Takes inout file name and output file name as parameters
    Uses the Huffman coding process on the text from the input file and writes encoded text to output file
    Take not of special cases - empty file and file with only one unique character"""
    frequency = cnt_freq(in_file)
    count = 0
    for i in frequency:
        if i != 0:
            count = count + 1
    if count == 0:
        #print(count)
        fileObj2 = open(out_file, 'w+')
        fileObj2.close()
    elif count == 1:
        fileObj2 = open(out_file, 'w+', newline='')
        header = create_header(frequency)
        fileObj2.write(header)
        fileObj2.close()

    elif count>1:



        RootNode = create_huff_tree(frequency)
        #print(frequency)
        code = create_code(RootNode)
        header = create_header(frequency)
        codeDict ={}
        for i in range(len(code)):
            codeDict.update({i:code[i]})

        fileObj1 = open(in_file,'r')




        fileObj2 = open(out_file,'w+', newline= '')

        fileObj2.write(header)
        fileObj2.write('\n')
        for line in fileObj1:

            for i in line:
                key = ord(i)
                fileObj2.write(codeDict[key])
        fileObj1.close()
        fileObj2.close()



def getHeaderString(filename):
    try:
        fileObj = open(filename,'r')
    except:
        raise FileNotFoundError
    line1 = fileObj.readline()
    fileObj.close()
    #print(line1)
    return line1

def parse_header(header_string):
    FreqList = [0]*256
    headerList = header_string.strip().split()
    #print(headerList)
    for i in range(0,len(headerList),2):
        FreqList[int(headerList[i])] = int(headerList[i+1])
    return FreqList

def treeDecode(rootnode,str1):
    listA = []
    node = rootnode
    for i in str1:

        if i == '0':
            node = node.left
        if i == '1':
            node = node.right
        if node.left == None and node.right == None:
            listA.append(node.char)
            node = rootnode

    return listA





def huffman_decode(encoded_file, decode_file):
    header_string = getHeaderString(encoded_file)
    if header_string == "":
        fileObj2 = open(decode_file, 'w+', newline='')
        fileObj2.close()
    else:

        listCount = header_string.split()
        #print(listCount)
        count = len(listCount)
        #print(count)
        if count == 2:
            str1 = chr(int(listCount[0]))*int(listCount[1])
            fileObj2 = open(decode_file, 'w+', newline='')
            fileObj2.write(str1)
            fileObj2.close()
        else:

            FreqList = parse_header(header_string)

            rootnode = create_huff_tree(FreqList)

            fileObj = open(encoded_file,'r')
            line = fileObj.readline()
            line = fileObj.readline()
            #print(line)
            fileObj.close()
            DecodedList = treeDecode(rootnode,line)
            for i in range(len(DecodedList)):
                DecodedList[i] = chr(DecodedList[i])
            str1 = ''.join(DecodedList)
            #print(str1)
            fileObj2 = open(decode_file, 'w+', newline='')
            fileObj2.write(str1)
            fileObj2.close()















