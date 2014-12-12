// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "bitstream.h"
#include <string>
// TODO: include any other headers you need

using namespace std;

map<int, int> buildFrequencyTable(istream& input) {
    map<int, int> freqTable;
    // As long as we are not at the end of the file, read 1 byte and check if it's already in the map
    // if so, increment it's value by 1, else add it to the map and set the value to 1
    while(true){
        int inputByte = input.get();
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
    for (auto it = freqTable.begin(); it != freqTable.end();it++){
        HuffmanNode node(it->first,it->second);
        prioQ.push(node);
    }
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

void traverse(HuffmanNode* currentNode,string code, map<int, string>& encodingMap){
    if (currentNode->isLeaf()){
        encodingMap.insert(make_pair(currentNode->character,code));
        return;
    }
    traverse(currentNode->zero,code+"0",encodingMap);
    traverse(currentNode->one,code+"1",encodingMap);
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    map<int, string> encodingMap;
    traverse(encodingTree,"",encodingMap);
    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    while(true){
        int inputByte = input.get();
        if(inputByte == -1){
            for (int i = 0; i < encodingMap.at(PSEUDO_EOF).size();++i){
                output.writeBit(encodingMap.at(PSEUDO_EOF)[i] == '1' ? 1 : 0);
            }
            break;
        }
        else{
            for (int i = 0; i < encodingMap.at(inputByte).size();++i){
                output.writeBit(encodingMap.at(inputByte)[i] == '1' ? 1 : 0);
            }
        }
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* currentNode = encodingTree;
    while(true){
        int inputBit = input.readBit();
        cout << inputBit;
        if (inputBit == -1){
            break;
        }
        else if (inputBit == 1){
            currentNode = currentNode->one;
        }
        else {
            currentNode = currentNode->zero;
        }
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
    map<int, string> encodingMap = buildEncodingMap(buildEncodingTree(freqTable));
    output.put('{');
    for (auto it = freqTable.begin(); it != freqTable.end(); ++it){
        if (it != freqTable.begin()){
            output.put(',');
            output.put(' ');
        }
        string str = to_string(it->first);
        for (int i = 0; i < str.size();++i){
            output.put(str[i]);
        }
        output.put(':');
        str = to_string(it->second);
        for (int i = 0; i < str.size();++i){
            output.put(str[i]);
        }
    }
    output.put('}');
    input.clear(); // removes any current eof/failure flags
    input.seekg(0, ios::beg); // tells the stream to seek back to the beginning
    encodeData(input,encodingMap,output);

}

void decompress(ibitstream& input, ostream& output) {
    map<int, int> freqTable;
    input.get(); // gets '{'
    char byte;
    while ((byte = input.get()) != '}'){
        string str;
        while(byte != ':'){
            str+=(string(1,byte));
            byte = input.get();
        }
        string str2;
        while((byte = input.get()) != ',' && byte != '}'){
            str2+=(string(1,byte));
        }
        freqTable.insert(make_pair((char)atoi(str.c_str()),atoi(str2.c_str())));
        if (byte == '}')
            break;
        input.get(); // gets ' '
    }
    HuffmanNode* tree = buildEncodingTree(freqTable);
    for (auto it = freqTable.begin(); it != freqTable.end();++it)
        cout << (char)(it->first) << ":" << it->second << endl;
    decodeData(input,tree,output);

}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
}
