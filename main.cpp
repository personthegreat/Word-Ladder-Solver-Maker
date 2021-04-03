#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
using namespace std;

int main()
{
    cout << "Loading..." << endl;
    ifstream in("WordsByLength.txt");
    unordered_map<int, unordered_set<string>> database;

    //reads in all the words and inserts them into database, key is length of word
    while (!in.eof())
    {
        string temp;
        in >> temp;
        int length = temp.length();
        if (database.find(length) == database.end())
        {
            unordered_set<string> n;
            n.insert(temp);
            database[length] = n;
        }
        else
        {
            database[length].insert(temp);
        }
    }

    string begin;
    string end;
    do 
    {
        //continues prompting until a valid word is detected
        cout << "enter first word: ";
        cin >> begin;
    } while (database.find(begin.length()) == database.end() || database[begin.length()].find(begin) == database[begin.length()].end());

    do
    {
        //continues prompting until a valid word with the same length as begin is detected
        cout << "enter second word: ";
        cin >> end;
    } while (database.find(end.length()) == database.end() || database[end.length()].find(end) == database[end.length()].end() || begin.length() != end.length());

    int length = begin.length();
    //gets all the words of a certain length
    unordered_set<string> words = database[length];
    
    class graph
    {
        //stores from ambiguous word to words. h*t to hat and hot and hut
        unordered_map<string, unordered_set<string>> findWords;
    public:
        graph(unordered_set<string> words)
        {
            for (auto iter = words.begin(); iter != words.end(); ++iter)
            {
                string cur = *iter;
                for (int i = 0; i < cur.length(); ++i)
                {
                    string insert = cur.substr(0, i) + "*" + cur.substr(1 + i, cur.length() - i - 1);
                    //inserts this particular combo into findWords
                    if (findWords.find(insert) == findWords.end())
                    {
                        unordered_set<string> temp;
                        temp.insert(cur);
                        findWords[insert] = temp;
                    }
                    else
                    {
                        findWords[insert].insert(cur);
                    }
                }
            }
        }
        void test()
        {
            for (auto iter = findWords.begin(); iter != findWords.end(); ++iter)
            {
                cout << iter->first << ": ";
                for (auto jter = iter->second.begin(); jter != iter->second.end(); ++jter)
                {
                    cout << *jter << ", ";
                }
                cout << endl;
            }
        }
    };
    graph test(words);
    test.test();

    return 0;
}

