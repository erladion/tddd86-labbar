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
#include <math.h>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";

/*
 * Läser in en fil innehållande en ordlista som programmet ska använda när användaren spelar
 * Sparar i olika delar av mapen beroende på ordlängd
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
 * Skriver ut en queue till konsolen
 */
void printQueue(queue<string>& q){
    for (size_t i = 0;i < q.size();++i){
        cout << q.front() << endl;
        q.push(q.front());
        q.pop();
    }
}

/*
 * Tar in ett ord samt en bokstav och kollar ifall bokstaven finns i ordet
 * och gör ett binärt tal där 1 representerar om den finns på den positionen, 0 annars.
 * Returnerar talet i decimalform.
 */
int findLetter(string& word, char& letter){
    int res = 0;
    for (size_t i = 0; i < word.length(); ++i){
        if (letter == word[i]) res += pow(2,i);
    }
    return res;
}

/*
 * Tar in en unordered list av listor och returnerar den längsta.
 */
queue<string> longestList(unordered_map<int,queue<string> > listOfLists){
    queue<string> bestList;
    int length = 0;
    for (auto it = listOfLists.begin(); it != listOfLists.end();++it){
        if (it->second.size() > length){
            length = it->second.size();
            bestList = it->second;
        }
    }
    cout << endl;
    return bestList;
}

/*
 * Tar in en lista med alla ord som finns kvar att välja från och en gissning.
 * Den delar upp ordlistan i olika listor beroende på var gissningen fanns i ordet.
 * Sedan returnerar den den längsta av de listorna.
 */
queue<string> bestWordlist(queue<string>& wordlist, char& guess, int guessesLeft){
    unordered_map<int,queue<string> > possibleWordGroups;
    while (!wordlist.empty()){
        string word = wordlist.front();
        wordlist.pop();
        int letterCode = findLetter(word, guess); // lettercode är unikt för varje ordgrupp
        if (possibleWordGroups.find(letterCode) == possibleWordGroups.end()){ // kollar om ordgruppen inte finns, annars skapas en ny lista som hör till den nya ordgruppen.
            queue<string> wordGroup;
            possibleWordGroups[letterCode] = wordGroup;
        }
        possibleWordGroups[letterCode].push(word);
    }
    if (guessesLeft == 1 && possibleWordGroups.find(0) != possibleWordGroups.end()) // Om man bara har en gissning kvar så väljer den automatiskt ordgruppen som inte innehåller några rätt om den finns.
        return possibleWordGroups[0];
    return longestList(possibleWordGroups);
}

/*
 * Skriver ut ordet efter varje gissning, om bokstaven inte har blivit gissad så skrivs "-" ut annars den rätta bokstaven.
 * Om alla bokstäver är gissade så ändras "won" till true
 */
void printWord(string& word, unordered_set<char>& letterList, bool& won){
    string res;
    won = true;
    for (size_t i = 0; i < word.length();++i){
        if (letterList.find(word[i]) == letterList.end()){
            res += "-";
            won = false;
        }
        else
            res += word[i];
    }
    cout << res << endl;
}

/*
 * Kollar om gissning är rätt
 */
bool correctGuess(string& word, char& guess){
    for (size_t i = 0; i < word.length();++i){
        if (guess == word[i]) return true;
    }
    return false;
}

int main() {
    cout << "Welcome to Hangman." << endl;

    unordered_multimap<int,string> dictionary;
    readDictionary(dictionary);

    while (true){
        string strWordLength;
        int wordLength;
        do {
        cout << "What length on the words do you want? ";
        cin >> strWordLength;
        wordLength = atoi(strWordLength.c_str());
        } while (dictionary.find(wordLength) == dictionary.end());

        int numberOfGuesses;
        string strNumberOfGuesses;
        do{
        cout << "How many guesses do you want? ";
        cin >> strNumberOfGuesses;
        numberOfGuesses = atoi(strNumberOfGuesses.c_str());
        } while(numberOfGuesses < 1);

        bool wantToSeeWords;
        string line;
        cout << "Do you want to see the remaining words? (y/n) ";
        cin >> line;
        wantToSeeWords = line == "y";

        bool wonTheGame = false;

        /*
         * Tar ut alla ord av rätt längd och lägger in dessa i en queue
         */
        auto iteratorWithRightWords = dictionary.equal_range(wordLength);
        queue<string> usableWords;
        for (auto it = iteratorWithRightWords.first;it != iteratorWithRightWords.second;++it){
            usableWords.push(it->second);
        }

        if (wantToSeeWords){
            printQueue(usableWords);
        }

        unordered_set<char> guessedLetters;

        while (!wonTheGame && numberOfGuesses > 0) {
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

            usableWords = bestWordlist(usableWords,guessedChar, numberOfGuesses);

            if (!correctGuess(usableWords.front(),guessedChar))
                numberOfGuesses--;

            if (wantToSeeWords){
                printQueue(usableWords);
            }
            printWord(usableWords.front(),guessedLetters, wonTheGame);
            cout << "Number of guesses left: " << numberOfGuesses << endl;
        }
        cout << (wonTheGame ? "Congratulations, you have defeated me..." : "I win again, mortal.") << endl;
        cout << "Do you want to play again? (y/n) ";
        string answer;
        cin >> answer;
        if (answer != "y") break;
    }
    return 0;
}
