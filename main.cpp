#include <iostream>
#include <vector>
#include <fstream>
#include <ostream>

using namespace std;

class token {
  public:
    string word;
    int freq;
    int code;

    //constructor
    token(string w) {
      freq = 1;
      code = 0;
      word = w;
    }
};

struct hashtable {
  vector<string> tokens;
  vector<int> codes;
};

vector<token> reader(string &);  // step 1

void sortDescend(vector<token> &);  // step 2
void InsertionSort(vector<token> &);

void counter(vector<token> &);  // step 3

void encode(hashtable &, vector<token> &); // step 4

void decode(hashtable &, vector<token> &); // step 5

void compressToFile(hashtable &, vector<token> &, string &,string &);  // step 6


int main() {
  string inputFile;
  string outputFile;

  cout << "Enter name of inputfile: ";
  cin >> inputFile;
  cout << endl;

  vector<token> wordlist = reader(inputFile);
  sortDescend(wordlist);
  counter(wordlist);

  hashtable encoder;
  encode(encoder, wordlist);
  hashtable decoder;
  decode(decoder, wordlist);

  cout << "Enter name of outputfile: ";
  cin >> outputFile;
  cout << endl;

  compressToFile(decoder, wordlist, inputFile, outputFile);

  return 0;
}

/*
STEP 1: Pass through the data once, collecting a list of token-frequency counts
*/
vector<token> reader(string &fileName) {
    ifstream inFS;
    inFS.open(fileName);
    if (!inFS.is_open()) {
        cout << "Error opening " << fileName << endl;
        exit(1);
    }

    string curr;
    vector<token> wordlist;
    bool isFound = false;

    while (inFS >> curr) {
        for (unsigned int i = 0; i < wordlist.size(); ++i) {
            if (curr == wordlist.at(i).word) { //sees if two words are matching
                wordlist.at(i).freq ++;
                isFound = true; //bool to check if word is in file
            }
        }

        if (isFound == false) {
            token newWord = token(curr);
            wordlist.push_back(newWord);
        }

        isFound = false;
    }

    inFS.close();

    return wordlist;
}

/*
STEP 2: Sort the token-frequency counts by frequency, in descending order
*/
void sortDescend(vector<token> &tokenList) {
  InsertionSort(tokenList);
}

void InsertionSort(vector<token> &wordlist) {
  int i = 0;
  int j = 0;

  for (i = 1; i < wordlist.size(); ++i) {
    j = i;
    // Insert numbers[i] into sorted part
    // stopping once numbers[i] in correct position
    while (j > 0 && wordlist.at(j).freq > wordlist.at(j - 1).freq) {

      // Swap numbers[j] and numbers[j - 1]
      token temp = wordlist.at(j); // Temporary variable for swap
      wordlist.at(j) = wordlist.at(j - 1);
      wordlist.at(j - 1) = temp;
      --j;
    }
  }
}

/*
STEP 3: Assign codes to tokens using a simple counter, for example by incrementing over the integers; this is just to keep things simple.
*/
void counter(vector<token> &wordlist) {
  for (unsigned int i = 0; i < wordlist.size(); ++i) {
    wordlist.at(i).code = i;
  }
}

/*
STEP 4: Write encoder to hashtable
*/
void encode(hashtable &encoder, vector<token> &wordlist) {
  for (unsigned int i = 0; i < wordlist.size(); ++i) {
    encoder.tokens.push_back(wordlist.at(i).word);
    encoder.codes.push_back(wordlist.at(i).code);
  }
}

/*
STEP 5: Write decoder to hashtable
*/
void decode(hashtable &decoder, vector<token> &wordlist) {
  for (unsigned int i = 0; i < wordlist.size(); ++i) {
    decoder.tokens.push_back(wordlist.at(i).word);
    decoder.codes.push_back(wordlist.at(i).code);
  }
}

/*
STEP 6: Write decoder and encoded data to outputfile
*/
void compressToFile(hashtable &decoder, vector<token> &wordlist, string &inputFile, string &outputFile) {
  ofstream oFS;
  oFS.open(outputFile);
  if (!oFS.is_open()) {
    cout << "Error opening " << outputFile << endl;
    exit(1);
  }

  ifstream inFS;
  inFS.open(inputFile);
  if (!inFS.is_open()) {
    cout << "Error opening " << inputFile << endl;
    exit(1);
  }

  string curr;

  while (inFS >> curr) {
    for (unsigned int i = 0; i < wordlist.size(); ++i) {
      if (curr == wordlist.at(i).word) {
        oFS << wordlist.at(i).code;
        oFS << " ";
      }
    }
  }

  oFS << endl << endl;

  for (unsigned int i = 0; i < decoder.codes.size(); ++i) {
    oFS << decoder.codes.at(i) << " -> " << decoder.tokens.at(i) << endl;
  }

  
  inFS.close();
  oFS.close();
}