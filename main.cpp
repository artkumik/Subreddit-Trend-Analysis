#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream> 
#include <iostream>
#include <sstream>

using namespace std;

void intro(){
    cout << "****************************************************" << endl;
    cout << "Welcome to the project 'Subreddit Trend Analysis'" << endl;
    cout << "by: Shyamaa Karthik, Artur Kumik, Ahnaf Raihan" << endl;
    cout << "****************************************************" << endl;
}

int subreddit_selection(){
    cout << "Select an available subreddit : " << endl;
    cout << "   1. r/placeholder1" << endl;
    cout << "   2. r/placeholder2" << endl;
    cout << "   3. r/placeholder3" << endl;
    cout << "   4. r/placeholder4" << endl;
    cout << "   5. Exit Program" << endl;

    vector<int> subreddits = {1, 2, 3, 4, 5};
    string input;
    int input_stoi;

    while(true){

        cout << "Input (1-5):";
        cin >> input;
        input_stoi = stoi(input);

        auto it_found = find(subreddits.begin(), subreddits.end(), input_stoi);
        if (it_found != subreddits.end())
        {   
            if(input_stoi == 5){
                cout << "Exiting the program, have a nice day!" << endl;
            }
            return input_stoi;
        }
        else
        {
            cout << input_stoi << " is not an available subreddit. Try again." << std::endl;
        }
    }
}

int submenu_selection(){
    cout << "What would you like to do? " << endl;
    cout << "   1. List top posts for my subreddit" << endl;
    cout << "   2. List bottom posts for my subreddit" << endl;
    cout << "   3. Show top 15 words for my subreddit" << endl;
    cout << "   4. Show bottom 15 words for my subreddit" << endl;
    cout << "   5. Dataset information" << endl;
    cout << "   6. Change subreddit" << endl;
    cout << "   7. Exit program" << endl;

    vector<int> options = {1, 2, 3, 4, 5, 6, 7};
    string input;
    int input_stoi;

    while (true)
    {
        cout << "Input (1-7):";
        cin >> input;
        input_stoi = stoi(input);

        auto it_found = find(options.begin(), options.end(), input_stoi);
        if (it_found != options.end())
        {
            if (input_stoi == 7)
            {
                cout << "Exiting the program, have a nice day!" << endl;
            }
            return input_stoi;
        }
        else
        {
            cout << input_stoi << " is not an available menu option. Try again." << std::endl;
        }
    }
}

void read_csv(string path){
    ifstream file(path);

    if(!file.is_open()){
        cout << "error" << endl;
        return;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string cell;
        vector<string> row_data;
        vector<string> test;

        while (getline(ss, cell, ','))
        {
            row_data.push_back(cell);
        }

        cout << "upvotes : " << row_data[0] << endl;
        cout << "downvotes : " << row_data[1] << endl;
        cout << "title : " << row_data[2] << endl;
        cout << "content : " << row_data[3] << endl;

        cout << endl;
    }

    file.close();
    return;
}

int main(){

    //testing, make sure to remove in final version 
    //replace AskReddit with the name of the csv
    read_csv("processed_data/AskReddit.csv");

    int subreddit;
    int option_chose;
    bool submenu;

    //on startup
    intro();

    bool active = true;
    while(active){
        submenu = true;

        //subreddit selection and exiting
        subreddit = subreddit_selection();
        if(subreddit == 5){
            active = false;
            return 1;
        }

        //options once subreddit is chosen
        while(submenu){
            option_chose = submenu_selection();

            //change options in here based on what subreddit and the different data structures
            // make sure to keep the break or all will break
            switch (option_chose)
            {
            case 1:
                cout << "Listing top posts for subreddit" << endl;
                break;
            case 2:
                cout << "Listing bottom posts for subreddit" << endl;
                break;
            case 3:
                cout << "Listing top words for subreddit" << endl;
                break;
            case 4:
                cout << "Listing bottom words for subreddit" << endl;
                break;
            case 5:
                cout << "Printing dataset information" << endl;
                break;
            case 6:
                //change subreddit case
                cout << "Choosing another subreddit" << endl;
                submenu = false;
                break;
            case 7:
                //exit case
                submenu = false;
                active = false;
                return 1;
            default:
                cout << "How Did We Get Here?" << endl;
            }

            cout << "" << endl;
        }
    }
    return 1;
}