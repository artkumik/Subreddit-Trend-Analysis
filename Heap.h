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
    std::string title;
    std::string body;
};

struct MinUpvotes { // min-heap comparator
    bool operator()(const Post& a, const Post& b) const;
};
struct MaxUpvotes { // max-heap comparator
    bool operator()(const Post& a, const Post& b) const;
};

string getstring(std::string s); //helper
void createLists(string path, std::size_t k, std::priority_queue<Post, std::vector<Post>, MaxUpvotes>& bottom15,
    std::priority_queue<Post,std::vector<Post>, MinUpvotes>& top15); //creates a min-heap and max-heap for the posts

void createtopvector(std::priority_queue<Post, std::vector<Post>, MinUpvotes>& top15,
                             std::vector<Post>& result); //creates vector for top 15 posts

void createbottomvector(std::priority_queue<Post, std::vector<Post>, MaxUpvotes>& bottom15,
                                std::vector<Post>& result); //creates vector for bottom 15 posts
void printvector(std::vector<Post>& v); // print helper
#endif //HEAP_H
