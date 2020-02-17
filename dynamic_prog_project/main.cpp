#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace::std;

typedef long long int lint;

//global vars.
vector<string> patterns;
vector<lint> hashPatterns;
vector<lint> hashTXT;
vector<lint> amount;

string txt = "";
int wordCount = 0;
int prime = 31;
int modus = 1e9 + 7;
//calculating the hash
lint _hash(string s) {
    lint h = 0;
    for (int i = 0; i < s.length(); i++) {
        h = (prime*h + s[i] - 'a' ) % modus;
    }

    return h;
}
//read and create data
void readInput(string _path) {

    ifstream infile;
    infile.open(_path);

    infile >> txt;
    infile >> wordCount;

    amount.resize(1002,0);
    hashTXT.resize(txt.length(),0);

    for (int i = 0; i < wordCount; i++) {
        string word;
        infile >> word;
        hashPatterns.push_back(_hash(word));
        patterns.push_back((word));
    }
    for (int i = 0; i < txt.length() ; i++) {
        hashTXT[i] = _hash(txt.substr(0,i+1));
    }
    infile.close();
}
//for mod
lint power( long long int x, unsigned int y) {
    int res = 1;
    while (y > 0) {
        if (y & 1)
            res = (res*x)% modus;
        y = y>>1;
        x = (x*x)% modus;
    }
    return res%modus;
}
//recursive func
lint func(int index) {
    if(amount[index]==0) {
        lint temp = 0;
        if (index == txt.length()) {
            temp = 1;
        }
        else {
            vector<bool> isVisited(txt.length());
            for (int i = 0; i < wordCount; i++) {
                lint valid_hash;
                lint pattern_hash = hashPatterns[i];
                string pattern = patterns[i];
                if(isVisited[pattern.length()-1] || index+pattern.length() > txt.length())
                    continue;

                if(index!=0) {//check modus!
                    valid_hash = ((hashTXT[index + pattern.length() - 1])%modus)-
                                 (hashTXT[index - 1] * (power(prime, pattern.length())) % modus);
                    valid_hash %= modus;
                    valid_hash += modus;
                    valid_hash %= modus;
                }
                else {
                    valid_hash = hashTXT[pattern.length()-1];
                }
                if(valid_hash == pattern_hash) {
                    if(txt.substr(index,pattern.length()).compare(pattern)==0) {
                        isVisited[pattern.length()-1] = true;
                        temp += func(index + pattern.length());
                        temp %= modus;
                    }
                }
            }
        }
        amount[index] = temp;
    }
    return amount[index];

}
//main
int main(int argc, char* argv[]) {

    if(argc != 3) {
        cout << "Run the code with the following command: ./project3 [input_file] [output_file]" << endl;
    }

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    readInput(argv[1]);

    ofstream output;
    output.open(argv[2]);
    output << func(0) << endl;
    output.close();

    return 0;
}
