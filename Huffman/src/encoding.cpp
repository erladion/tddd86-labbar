// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
// TODO: include any other headers you need

using namespace std;

map<int, int> buildFrequencyTable(istream& input) {
    map<int, int> freqTable;
    // As long as we are not at the end of the file, read 1 byte and check if it's already in the map
    // if so, increment it's value by 1, else add it to the map and set the value to 1
    while(true){
        int inputByte = input.get();
        if(inputByte == -1){
            freqTable.insert(make_pair(inputByte, 1));
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
    for (auto it = freqTable.begin(); it != freqTable.end();it++){
        HuffmanNode node(it->first,it->second);
        prioQ.push(node);
    }
    while (prioQ.size() > 1){
        HuffmanNode n1 = prioQ.top();
        HuffmanNode* p1 = &n1;
        prioQ.pop();
        HuffmanNode n2 = prioQ.top();
        HuffmanNode* p2 = &n2;
        prioQ.pop();
        HuffmanNode n3(NOT_A_CHAR,p1->count+p2->count,p1,p2);
        prioQ.push(n3);
    }
    HuffmanNode node = prioQ.top();
    return &node;
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    map<int, string> encodingMap;
    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    // TODO: implement this function
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    // TODO: implement this function
}

void compress(istream& input, obitstream& output) {
    // TODO: implement this function
}

void decompress(ibitstream& input, ostream& output) {
    // TODO: implement this function
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
}
