/* Labb 6 - Huffman
 *
 * Namn:    Johan Jansson
 * Id:      johja118
 *
 * Namn:    Filip Magnusson
 * Id:      filma035
 */

#include "encoding.h"
#include "bitstream.h"

using namespace std;

map<int, int> buildFrequencyTable(istream& input) {
    map<int, int> freqTable;
    // As long as we are not at the end of the file, read 1 byte and check if it's already in the map
    // if so, increment it's value by 1, else add it to the map and set the value to 1
    while(true){
        int inputByte = input.get();
        // If inputByte == -1, we are at the end of file.
        if(inputByte == -1){
            freqTable.insert(make_pair(PSEUDO_EOF, 1));
            break;
        }
        if(freqTable.find(inputByte) != freqTable.end()){
            freqTable[inputByte]++;
        }
        else{
            freqTable.insert(make_pair(inputByte, 1));
        }
    };
    return freqTable;
}

HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    priority_queue<HuffmanNode> prioQ;
    // Makes HuffmanNodes out of our data and adds it into a priority queue
    for (auto it = freqTable.begin(); it != freqTable.end();it++){
        HuffmanNode node(it->first,it->second);
        prioQ.push(node);
    }
    // Add the nodes with smallest count together into a new node with their combined count
    while (prioQ.size() > 1){
        HuffmanNode* p1 = new HuffmanNode(prioQ.top().character,prioQ.top().count,prioQ.top().zero,prioQ.top().one);
        prioQ.pop();
        HuffmanNode* p2 = new HuffmanNode(prioQ.top().character,prioQ.top().count,prioQ.top().zero,prioQ.top().one);
        prioQ.pop();
        HuffmanNode n3(NOT_A_CHAR,p1->count+p2->count,p1,p2);
        prioQ.push(n3);
    }
    HuffmanNode* p = new HuffmanNode(prioQ.top().character,prioQ.top().count,prioQ.top().zero,prioQ.top().one);
    return p;
}

/*
 * Looks through the whole tree and gives each node a binary number depending on its position in the tree and adds it to the encoding map
 */
void traverse(HuffmanNode* currentNode,string code, map<int, string>& encodingMap){
    if (currentNode->isLeaf()){
        encodingMap.insert(make_pair(currentNode->character,code));
        return;
    }
    traverse(currentNode->zero,code+"0",encodingMap);
    traverse(currentNode->one,code+"1",encodingMap);
}

// Uses the above function to build an encoding map
map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    map<int, string> encodingMap;
    traverse(encodingTree,"",encodingMap);
    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    int inputByte;
    // As long as we are not at the end of file, check what the input byte's code is and write that to the output
    while((inputByte = input.get()) != -1){
        for (int i = 0; i < encodingMap.at(inputByte).size();++i){
            output.writeBit(encodingMap.at(inputByte)[i] == '1' ? 1 : 0);
        }
    }
    // Write the endOfFile code at the end of file
    for (int i = 0; i < encodingMap.at(PSEUDO_EOF).size();++i){
        output.writeBit(encodingMap.at(PSEUDO_EOF)[i] == '1' ? 1 : 0);
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* currentNode = encodingTree;
    int inputBit;
    // While we are not at the end of file, check the bit and traverse the tree
    while((inputBit = input.readBit()) != -1){
        if (inputBit == 1){
            currentNode = currentNode->one;
        }
        else {
            currentNode = currentNode->zero;
        }
        // When we get to the leaf, write the character and reset currentNode
        if (currentNode->isLeaf()){
            if (currentNode->character == PSEUDO_EOF)
                break;
            output.put(currentNode->character);
            currentNode = encodingTree;
        }
    }
}

void compress(istream& input, obitstream& output) {
    map<int,int> freqTable = buildFrequencyTable(input);
    HuffmanNode* tree = buildEncodingTree(freqTable);
    map<int, string> encodingMap = buildEncodingMap(tree);
    // Writes down the whole frequency table to the header of the file
    output.put('{');
    for (auto it = freqTable.begin(); it != freqTable.end(); ++it){
        if (it != freqTable.begin()){
            output.put(',');
            output.put(' ');
        }
        // Writes down the character
        string str = to_string(it->first);
        for (int i = 0; i < str.size();++i){
            output.put(str[i]);
        }
        output.put(':');
        // Writes down the frequency of the character
        str = to_string(it->second);
        for (int i = 0; i < str.size();++i){
            output.put(str[i]);
        }
    }
    output.put('}');
    input.clear(); // removes any current eof/failure flags
    input.seekg(0, ios::beg); // tells the stream to seek back to the beginning
    // Encode the actual data
    encodeData(input,encodingMap,output);
    freeTree(tree);
}

void decompress(ibitstream& input, ostream& output) {
    map<int, int> freqTable;
    // Reads the header and save it in freqTable
    input.get(); // gets '{'
    char byte;
    while ((byte = input.get()) != '}'){
        string str;
        // Reads the character
        while(byte != ':'){
            str+=(string(1,byte));
            byte = input.get();
        }
        string str2;
        // Reads the frequency of the character
        while((byte = input.get()) != ',' && byte != '}'){
            str2+=(string(1,byte));
        }
        // Adds it to the freqTable
        freqTable.insert(make_pair(atoi(str.c_str()),atoi(str2.c_str())));
        // If we see a '}', we are done
        if (byte == '}')
            break;
        input.get(); // gets ' '
    }
    HuffmanNode* tree = buildEncodingTree(freqTable);
    // Decode the actual data with the help of the tree we got from the freqTable we just built
    decodeData(input,tree,output);
    freeTree(tree);
}

void freeTree(HuffmanNode* node) {
    if (node != nullptr) return;
    // Traverses the tree
    freeTree(node->zero);
    freeTree(node->one);
    // Delete current node after we have traversed its children
    delete node;
}
