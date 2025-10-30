#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>
#include <queue>

using namespace std;

//functions for reading the csv

// reading csv
string processString(string s)
{
    // removes punctuation and sets the string to lowercase
    s.erase(remove_if(s.begin(), s.end(), ::ispunct), s.end());
    transform(s.begin(), s.end(), s.begin(), ::tolower);

    return s;
}

void dataset_info(string path)
{
    // reads csv given path and can be modified to place values into a vector
    ifstream file(path);

    if (!file.is_open())
    {
        cout << "error" << endl;
        return;
    }

    string line;

    //information exported
    int post_count = 0;
    int max_upvotes = 0;
    int min_upvotes = 0;
    int max_downvotes = 0;
    int min_downvotes = 0;

    //getting column titles
    getline(file,line);
    stringstream ss(line);
    string cell;
    vector<string> headers;

    while (getline(ss, cell, ','))
    {
        headers.push_back(cell);
    }

    cout << "Header Titles : ";
    for(int i = 0; i < headers.size(); i++){
        cout << headers[i] << " " ; 
    }
    cout << endl;

    //getting info about the rest of the dataset
    while (getline(file, line))
    {
        stringstream ss(line);
        vector<string> row_data;

        while (getline(ss, cell, ','))
        {
            row_data.push_back(cell);
        }

        post_count += 1;

        //upvotes min/max
        //cout << "upvotes : " << row_data[0] << endl;

        if(stoi(row_data[0]) > max_upvotes){
            max_upvotes = stoi(row_data[0]);
        }
        else if (stoi(row_data[0]) < min_upvotes)
        {
            min_upvotes = stoi(row_data[0]);
        }

        //downvotes min/max
        //cout << "downvotes : " << row_data[1] << endl;

        if (stoi(row_data[1]) > max_downvotes)
        {
            max_downvotes = stoi(row_data[1]);
        }
        else if (stoi(row_data[1]) < min_downvotes)
        {
            min_downvotes = stoi(row_data[1]);
        }

        //cout << "title : " << processString(row_data[2]) << endl;
        //cout << "content : " << processString(row_data[3]) << endl;
        //cout << endl;
    }

    cout << "Most Upvotes per Post : " << max_upvotes << endl;
    cout << "Least Upvotes per Post : " << min_upvotes << endl;
    cout << "Most downvotes per Post : " << max_downvotes << endl;
    cout << "Least downvotes per Post : " << min_downvotes << endl;

    file.close();
    return;
}

