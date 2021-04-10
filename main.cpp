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
string BFS(const graph &graph, string begin, string end,int &visited)
{
    vector<string> path;
    if(begin==end)
    {
        return begin+", "+end;
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
    string cheese = "";
    if(current!=end)
        cout<<"There is no word ladder from "<<begin<<" to "<<end<<endl;
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
string biBFS(const graph &graph, string begin, string end,int &visited)
{
    string cheese="";
    unordered_map<string,string> forward_v;
    unordered_map<string,string> backward_v;
    queue<string> forward_q;
    queue<string> backward_q;
    vector<string> f_path;
    vector<string> b_path;
    if(begin==end)
    {
        return begin+", "+end;
    }
    int visitedCount=1;
    forward_q.push(begin);
    backward_q.push(end);
    string intersect="";
    string front="";
    string backward="";
    while(!forward_q.empty()&&!backward_q.empty())
    {
        front = forward_q.front();
        forward_q.pop();
        backward = backward_q.front();
        backward_q.pop();
        for(unsigned int i =0; i<front.length();++i)
        {
            string temp=front;
            temp[i]='*';
            unordered_set<string> list = graph.getList(temp);
            for(auto it=list.begin();it!=list.end();++it)
            {
                if(forward_v.find(*it)==forward_v.end())
                {
                    forward_v.insert({*it, front});
                    forward_q.push(*it);
                }
                ++visitedCount;
            }
        }
        for(unsigned int i =0; i<backward.length();++i)
        {
            string temp=backward;
            temp[i]='*';
            unordered_set<string> list = graph.getList(temp);
            for(auto it=list.begin();it!=list.end();++it)
            {
                if(backward_v.find(*it)==backward_v.end())
                {
                    backward_v.insert({*it, front});
                    backward_q.push(*it);
                }
                ++visitedCount;
            }
        }
        for(auto it = forward_v.begin(); it!=forward_v.end(); ++it)
            for(auto it2 = backward_v.begin(); it2!=backward_v.end();++it2)
                if(it->first==it2->first)
                {
                    intersect = it->first;
                    break;
                }
        if(!intersect.empty())
            break;
    }
    if(intersect.empty())
        cout<<"There is no word ladder from "<<begin<<" to "<<end<<endl;
    else
    {
        front = intersect;
        f_path.push_back(intersect);
        front = forward_v[front];
        while (front != begin) {
            f_path.push_back(front);
            front = forward_v[front];
        }
        f_path.push_back(begin);
        backward = backward_v[intersect];
        b_path.push_back(backward);
        backward = backward_v[intersect];
        while (backward != end) {
            b_path.push_back(backward);
            backward = forward_v[backward];
        }
        b_path.push_back(end);
        for(auto it = f_path.begin(); it!=f_path.end(); ++it)
            cheese+= *it+", ";
        for(auto it = b_path.rbegin(); it!=b_path.rend(); ++it)
        {
            if(it+1==b_path.rend())
                cheese+=*it;
            else
                cheese=*it+", ";
        }
    }
    visited=visitedCount;
    return cheese;
}
int main()
{
    auto start = chrono::steady_clock::now();
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
    auto difference = chrono::steady_clock::now()-start;
    auto duration = chrono::duration_cast<std::chrono::milliseconds>(difference);
    cout<<duration.count()<<endl;
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
    start = chrono::steady_clock::now();
    int numVisited;
    string answer=biBFS(test,begin,end,numVisited);
    difference = chrono::steady_clock::now()-start;
    duration = chrono::duration_cast<std::chrono::milliseconds>(difference);
    cout<<"Execution time of solving the word ladder: "<<duration.count()<< " Milliseconds"<<endl;
    cout<<"Word Ladder: "<<answer<<endl;
    cout<<"Number of nodes visited: "<<numVisited<<endl;
    return 0;
}

