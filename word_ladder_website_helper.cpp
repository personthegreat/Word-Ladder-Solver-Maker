#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <queue>
#include<chrono>
#include <algorithm>

using namespace std;
class graph
{
    //stores from ambiguous word to words. h*t to hat and hot and hut
    unordered_map<string, unordered_set<string>> findWords;
    int numOfWords;
public:
    graph(unordered_set<string> words, int size)
    {
        numOfWords = size;
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
    unordered_set<string> getList(string access) const
    {
        if (findWords.find(access) == findWords.end())
            return unordered_set<string>();
        return findWords.at(access);
    }
};
string BFS(const graph& graph, string begin, string end, int& visited)
{
    vector<string> path;
    if (begin == end)
    {
        return begin + ", " + end;
    }
    int visitedCount = 1;
    unordered_map<string, string> paths;
    queue<string> words;
    words.push(begin);
    paths.insert({ begin,"" });
    string current = "";
    while (current != end && !words.empty())
    {
        current = words.front();
        words.pop();
        for (unsigned int i = 0; i < current.length(); ++i)
        {
            string temp = current;
            temp[i] = '*';
            unordered_set<string> list = graph.getList(temp);
            for (auto it = list.begin(); it != list.end(); ++it)
            {
                if (paths.find(*it) == paths.end())
                {
                    paths.insert({ *it, current });
                    words.push(*it);
                }
                ++visitedCount;
            }
        }
    }
    string cheese = "";
    if (current != end)
        cout << "There is no word ladder from " << begin << " to " << end << endl;
    else
    {
        while (current != begin) {
            path.push_back(current);
            current = paths[current];
        }
        path.push_back(begin);
        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            if (it + 1 == path.rend())
                cheese += *it;
            else
                cheese += *it + ",";
        }
    }
    visited = visitedCount;
    return cheese;
}
string checkWordValidity(string word)
{
    ifstream in("WordsByLength.txt");
    while (!in.eof())
    {
        string temp;
        in >> temp;
        if (temp == word)
        {
            return "true";
        }
    }
    return "false";



}
void searchStuff(string begin, string end)
{
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
    

    int length = begin.length();
    //gets all the words of a certain length
    unordered_set<string> words = database[length];
    graph test(words, words.size());
    vector<string> tester;
    const auto start = chrono::steady_clock::now();
    int numVisited;
    string answer = BFS(test, begin, end, numVisited);
    const auto difference = chrono::steady_clock::now() - start;
    auto duration = chrono::duration_cast<std::chrono::nanoseconds>(difference);
    cout << "Execution time of solving the word ladder: " << duration.count() << " Microseconds" << "<br>";
    cout << "Shortest size of ladder: " << tester.size() << "<br>";
    cout << "Word Ladder: " << answer <<"<br>";
    cout << "Number of nodes visited: " << numVisited << "<br>";
    return;
}


int main()
{
    string query = getenv("QUERY_STRING");
    string functionName = query.substr(query.find("functionName=")+13,query.find('&')- query.find("functionName=") - 13);
    string parameters = query.substr(query.find("parameters=")+11);
    cout << "Content-type:text/html\r\n\r\n";
    if (functionName == "checkValidity")
    {
        string word1 = parameters.substr(0,parameters.find('&'));
        string word2 = parameters.substr(parameters.find('&')+1);
        cout << checkWordValidity(word1)<< "&"<<checkWordValidity(word2);
    }
    if (functionName == "searchStuff")
    {
        string word1 = parameters.substr(0, parameters.find('&'));
        string word2 = parameters.substr(parameters.find('&') + 1);
        searchStuff(word1, word2);
    }


    return 0;
}