#ifndef HEAP_H
#define HEAP_H
#include <queue>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

struct Post {
    int upvotes;
    int downvotes;
    string title;
    string body;
};

struct MinUpvotes { // min-heap comparator
    bool operator()(const Post& a, const Post& b) const;
};
struct MaxUpvotes { // max-heap comparator
    bool operator()(const Post& a, const Post& b) const;
};

string getstring(string s); //helper
void createLists(string path, size_t k, priority_queue<Post, vector<Post>, MaxUpvotes>& bottom15,
    priority_queue<Post,vector<Post>, MinUpvotes>& top15); //creates a min-heap and max-heap for the posts

void createtopvector(priority_queue<Post, vector<Post>, MinUpvotes>& top15,
                             vector<Post>& result); //creates vector for top 15 posts

void createbottomvector(priority_queue<Post, vector<Post>, MaxUpvotes>& bottom15,
                                vector<Post>& result); //creates vector for bottom 15 posts
void printvector(vector<Post>& v); // print helper
#endif //HEAP_H

// function declarations
bool MinUpvotes::operator()(const Post &a, const Post &b) const
{
    return a.upvotes > b.upvotes;
}
bool MaxUpvotes::operator()(const Post &a, const Post &b) const
{
    return a.upvotes < b.upvotes;
}

string getstring(string s)
{
    s.erase(remove_if(s.begin(), s.end(), ::ispunct), s.end());
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
void createLists(string path, size_t k, priority_queue<Post, vector<Post>, MaxUpvotes> &bottom15,
                 priority_queue<Post, vector<Post>, MinUpvotes> &top15)
{
    ifstream file(path);

    if (!file.is_open())
    {
        cout << "error opening: " << path << endl;
        return;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string cell;
        vector<string> row_data;

        while (getline(ss, cell, ','))
        {
            row_data.push_back(cell);
        }
        if (row_data.size() < 4)
            continue;

        // Skip header or malformed rows
        if (!isdigit(row_data[0][0]))
            continue;

        Post p;
        p.upvotes = stoi(row_data[0]);
        p.downvotes = stoi(row_data[1]);
        p.title = getstring(row_data[2]);
        p.body = getstring(row_data[3]);
        if (k > 0)
        { // creates min-heap to maintain top 15 posts
            if (top15.size() < k)
            {
                top15.push(p);
            }
            else if (p.upvotes > top15.top().upvotes)
            {
                top15.pop();
                top15.push(p);
            }
        }
        if (k > 0)
        { // creates max-heap to maintain bottom 15 posts
            if (bottom15.size() < k)
            {
                bottom15.push(p);
            }
            else if (p.upvotes < bottom15.top().upvotes)
            {
                bottom15.pop();
                bottom15.push(p);
            }
        }
    }

    file.close();
    return;
}

void createtopvector(priority_queue<Post, vector<Post>, MinUpvotes> &top15,
                     vector<Post> &result)
{
    result.clear();
    result.reserve(top15.size());
    while (!top15.empty())
    {
        result.push_back(top15.top());
        top15.pop();
    }
    sort(result.begin(), result.end(), [](const Post &a, const Post &b)
              { return a.upvotes > b.upvotes; });
}

void createbottomvector(priority_queue<Post, vector<Post>, MaxUpvotes> &bottom15,
                        vector<Post> &result)
{
    result.clear();
    result.reserve(bottom15.size());
    while (!bottom15.empty())
    {
        result.push_back(bottom15.top());
        bottom15.pop();
    }
    sort(result.begin(), result.end(), [](const Post &a, const Post &b)
              { return a.upvotes < b.upvotes; });
}

void printvector(vector<Post> &v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        Post p = v[i];
        cout << (i + 1) << ". Upvotes: " << p.upvotes << ", Downvotes: " << p.downvotes << ", Title: " << p.title << endl;
    }
}
