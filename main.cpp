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
        cout << "enter first word: ";
        cin >> begin;
    } while (database.find(begin.length()) == database.end() || database[begin.length()].find(begin) == database[begin.length()].end());

    do
    {
        cout << "enter second word: ";
        cin >> end;
    } while (database.find(end.length()) == database.end() || database[end.length()].find(end) == database[end.length()].end() || begin.length() != end.length());

    int length = begin.length();
    unordered_set<string> words = database[length];
    
    class graph
    {

    };
    return 0;
}

