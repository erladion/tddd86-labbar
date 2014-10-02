<<<<<<< HEAD
/* Labb 2a - Wordchain
 *
 * Namn:    Johan Jansson
 * Liuid:   johja118
 *
 * Namn:    Filip Magnusson
 * Liuid:   filma035
 *
 * Användaren skriver in 2 ord, sedan skriver programmet ut den kortaste vägen mellan de 2 orden
 * när man ändrar 1 bokstav i taget
 */
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <fstream>
#include <unordered_set>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";

/*
 * Tar fram alla ord man får då man ändrar 1 bokstav i det inmatade ordet till något annat
 */
stack<string> findNeighbours(string& word, unordered_set<string>& dictionary){
    stack<string> res;
    for(auto i = 0; i < word.length(); ++i){
        string testWord = word;
        for(auto alphabetIndex = 0; alphabetIndex < alphabet.length(); ++alphabetIndex){
            testWord[i] = alphabet[alphabetIndex];
            if (dictionary.find(testWord) != dictionary.end() && testWord != word){
                res.push(testWord);
            }
        }
    }
    return res;
}


/*
 * Hittar den kortaste vägen mellan 2 ord
 */
stack<string> wordChain(string& startWord, string& endWord, unordered_set<string>& dictionary){
    queue<stack<string> > chainList;
    unordered_set<string> usedWords;
    stack<string> newStack;
    newStack.push(startWord);
    chainList.push(newStack);
    while (!chainList.empty()){
        stack<string> chain = chainList.front();        
        chainList.pop();
        if (chain.top() == endWord){
            return chain;
        }
        else{
            stack<string> neighbours = findNeighbours(chain.top(),dictionary);
            while (!neighbours.empty()) {
                string neighbour = neighbours.top();
                neighbours.pop();
                if (usedWords.find(neighbour) == usedWords.end()){
                    stack<string> copy = chain;
                    copy.push(neighbour);
                    chainList.push(copy);
                    usedWords.insert(neighbour);
                }
            }
        }
    }
    return stack<string>();
}

/*
 * Skriver ut vägen mellan 2 ord i omvänd ordning
 */
void printStack(stack<string>& stack){
    while(!stack.empty()){
        cout << stack.top() << " ";
        stack.pop();
    }
    cout << endl;
}

int main() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    cout << "Please type two words: ";
    string startWord;
    string endWord;

    cin >> startWord >> endWord;

    ifstream file ("dictionary.txt");
    string line;
    unordered_set<string> dictionary;
    while(getline(file, line)){
        dictionary.insert(line);
    }
    stack<string> answer = wordChain(startWord,endWord, dictionary);
    if (answer.empty()){
        cout << "No chain between the words were found..." << endl;
    }
    else {
        cout << "Chain from " << endWord << " back to " << startWord << ":" << endl;
        printStack(answer);
    }
    cout << "Have a nice day!" << endl;

    return 0;
}
=======
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";

int main() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    cout << "Please type two words: ";

    // TODO: Finish the program!

    return 0;
}
>>>>>>> 078604445e0f2bde06ef1be5cf656c176c112971
