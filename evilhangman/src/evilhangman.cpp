/* Labb 2b - Evilhangman
 *
 * Namn:  Filip Magnusson
 * Liuid: filma035
 *
 * Namn:  Johan Jansson
 * Liuid: johja118
 *
 * Ett program som användaren kan spela hangman med, men programmet fuskar
 *
 */
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <unordered_set>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";


/*
 * Läser in en fil innehållande en ordlista som programmet ska använda när användaren spelar
 */
void readDictionary(unordered_multimap<int,string>& dictionary){
    ifstream input;
    input.open("dictionary.txt");
    string line;
    while(getline(input, line)){
        dictionary.insert(make_pair(line.length(),line));
    }
}

/*
 * Tar in ett ord samt en bokstav och kollar ifall bokstaven finns i ordet
 * och returnerar en sträng med 1:or och 0:or där 1:or betyder att bokstaven fanns på den positionen
 */
string findLetter(string& word, char& letter){
    string res;
    for (int i = 0; i < word.length(); ++i){
        if (letter == word[i]) res += "1";
        else res+= "0";
    }
    return res;
}

/*
 * Tar in en unordered list av listor och returnerar den längsta.
 */
queue<string> longestList(unordered_map<string,queue<string> > listOfLists){
    queue<string> bestList;
    int length = 0;
    for (auto it = listOfLists.begin(); it != listOfLists.end();++it){
        if ((*it).second.size() > length){
            length = (*it).second.size();
            bestList = (*it).second;
        }
    }
    return bestList;
}

/*
 * Returnerar true
 */
bool True(){
    return true;
}

/*
 * Tar in en lista med alla ord som finns kvar att välja från och en gissning.
 * Den delar upp ordlistan i olika listor beroende på var gissningen fanns i ordet.
 * Sedan returnerar den den längsta av de listorna.*
 *
 */
queue<string> bestWordlist(queue<string>& wordlist, char& guess){
    unordered_map<string,queue<string> > possibleWordGroups;
    for (int i = 0; i < wordlist.size(); ++i){
        string word = wordlist.front();
        wordlist.pop();
        string letterCode = findLetter(word, guess);
        if (possibleWordGroups.find(letterCode) == possibleWordGroups.end()){
            queue<string> wordGroup;
            possibleWordGroups.insert(make_pair(letterCode, wordGroup));
        }
        possibleWordGroups[letterCode].push(word);
    }
    return longestList(possibleWordGroups);
}


void printWord(string& word, unordered_set<char>& letterList, bool& won){
    string res;
    won = true;
    for (int i = 0; i < word.length();++i){
        if (letterList.find(word[i]) == letterList.end()){
            res += "-";
            won = false;
        }
        else
            res += word[i];
    }
    cout << res << endl;
}

bool correctGuess(string& word, char& guess){
    for (int i = 0; i < word.length();++i){
        if (guess == word[i]) return true;
    }
    return false;
}

int main() {
    cout << "Welcome to Hangman." << endl;

    unordered_multimap<int,string> dictionary;
    readDictionary(dictionary);
    while (true){
        int wordLength;
        cout << "What length on the words do you want? ";
        cin >> wordLength;

        int numberOfGuesses;
        cout << "How many guesses do you want? ";
        cin >> numberOfGuesses;

        bool wantToSeeWords;
        string line;
        cout << "Do you want to see the remaining words? (y/n) ";
        cin >> line;
        wantToSeeWords = line == "y";

        bool wonTheGame = false;

        auto iteratorWithRightWords = dictionary.equal_range(wordLength);
        queue<string> usableWords;
        for (auto it = iteratorWithRightWords.first;it != iteratorWithRightWords.second;++it){
            usableWords.push(it->second);
        }

        if (wantToSeeWords){
            for (int i = 0;i < usableWords.size();++i){
                cout << usableWords.front() << endl;
                usableWords.push(usableWords.front());
                usableWords.pop();
            }
        }

        unordered_set<char> guessedLetters;

        while (!wonTheGame) {
            char guessedChar;
            while (true){
                string guess;
                cout << "What is your guess? ";
                cin >> guess;
                if (guess.length() == 1 && alphabet.find(guess[0]) != alphabet.npos && guessedLetters.find(guess[0]) == guessedLetters.end()){
                    guessedChar = guess[0];
                    break;
                }else if (guess.length() > 1){
                    cout << "Your guess have to be 1 letter." << endl;
                }else if (alphabet.find(guess[0]) == alphabet.npos){
                    cout << "That is not a letter in the latin alphabet." << endl;
                } else {
                    cout << "You have already guessed that letter." << endl;
                }
            }
            guessedLetters.insert(guessedChar);

            usableWords = bestWordlist(usableWords,guessedChar);

            if (!correctGuess(usableWords.front(),guessedChar))
                numberOfGuesses--;

            if (wantToSeeWords){
                for (int i = 0;i < usableWords.size();++i){
                    cout << usableWords.front() << endl;
                    usableWords.push(usableWords.front());
                    usableWords.pop();
                }
            }
            printWord(usableWords.front(),guessedLetters, wonTheGame);
            cout << "number of guesses left: " << numberOfGuesses << endl;
            if (numberOfGuesses == 0) {
                wonTheGame = false;
                break;
            }
        }
        cout << (wonTheGame ? "Congratulations, you have defeated me..." : "I win again, mortal.") << endl;
        cout << "Do you want to play again? (y/n) ";
        string answer;
        cin >> answer;
        if (answer != "y") break;
    }
    return 0;
}
