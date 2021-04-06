#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <queue>
#include<chrono>
using namespace std;
class graph
{
    //stores from ambiguous word to words. h*t to hat and hot and hut
    unordered_map<string, unordered_set<string>> findWords;
    int numOfWords;
public:
    graph(unordered_set<string> words,int size)
    {
        numOfWords=size;
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
        if(findWords.find(access)==findWords.end())
            return unordered_set<string>();
        return findWords.at(access);
    }
};
int BFS(const graph &graph, string begin, string end,vector<string> &path)
{
    if(begin==end)
    {
        path.push_back(begin);
        path.push_back(end);
        return 0;
    }
    int visitedCount=1;
    unordered_map<string,string> paths;
    queue<string> words;
    words.push(begin);
    paths.insert({begin,""});
    string current="";
    while(current!=end&&!words.empty())
    {
        current=words.front();
        words.pop();
        for(unsigned int i =0; i<current.length();++i)
        {
            string temp=current;
            temp[i]='*';
            unordered_set<string> list = graph.getList(temp);
            for(auto it=list.begin();it!=list.end();++it)
            {
                if(paths.find(*it)==paths.end())
                {
                    paths.insert({*it, current});
                    words.push(*it);
                }
                ++visitedCount;
            }
        }
    }
    if(current!=end)
        cout<<"There is no word ladder from "<<begin<<" to "<<end<<endl;
    else
        while(current!=begin)
        {
            path.push_back(current);
            current=paths[current];
        }
        path.push_back(current);
    return visitedCount;
}
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
    graph test(words,words.size());
    vector<string> tester;
    const auto start = chrono::steady_clock::now();
    int numVisited = BFS(test,begin,end,tester);
    const auto difference = chrono::steady_clock::now()-start;
    auto duration = chrono::duration_cast<std::chrono::microseconds>(difference);
    cout<<"Execution time of solving the word ladder: "<<duration.count()<< " Microseconds"<<endl;
    cout<<"Shortest size of ladder: "<<tester.size()<<endl;
    cout<<"Word Ladder: ";
    for(auto it = tester.rbegin(); it!=tester.rend();it++)
        if(it+1==tester.rend())
            cout<<*it<<endl;
        else
            cout<<*it<<"->";
    cout<<"Number of nodes visited: "<<numVisited<<endl;
    return 0;
}

