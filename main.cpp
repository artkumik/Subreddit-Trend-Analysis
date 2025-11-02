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
#include <iomanip>
#include "heap.h"
#include "datasets.h"
#include "word_analysis.h"
using namespace std;

//menu functions
void intro()
{
    //printing for title and by

    cout << "****************************************************" << endl;
    cout << "Welcome to the project 'Subreddit Trend Analysis'" << endl;
    cout << "by: Shyamaa Karthik, Artur Kumik, Ahnaf Raihan" << endl;
    cout << "****************************************************" << endl;
    cout << endl;
}

int subreddit_selection()
{
    //menu for subreddit selection

    cout << "Select an available subreddit : " << endl;
    cout << "   1. r/AskReddit" << endl;
    cout << "   2. r/explainlikeimfive" << endl;
    cout << "   3. r/Showerthoughts" << endl;
    cout << "   4. r/techsupport" << endl;
    cout << "   5. Exit Program" << endl;

    vector<int> subreddits = {1, 2, 3, 4, 5};
    string input;
    int input_stoi;

    while (true)
    {
        cout << "Input (1-5):";
        cin >> input;
        cout << endl;
        input_stoi = stoi(input);

        auto it_found = find(subreddits.begin(), subreddits.end(), input_stoi);
        if (it_found != subreddits.end())
        {
            if (input_stoi == 5)
            {
                cout << "Exiting the program, have a nice day!" << endl;
            }
            return input_stoi;
        }
        else
        {
            cout << input_stoi << " is not an available subreddit. Try again." << endl;
        }
    }
}

int submenu_selection()
{
    //submenu after subreddit is chosen
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
        cout << endl;
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
            cout << input_stoi << " is not an available menu option. Try again." << endl;
        }
    }

}

int main()
{
    map<int, string> paths = {
        {1, "processed_data/AskReddit.csv"},
        {2, "processed_data/explainlikeimfive.csv"},
        {3, "processed_data/Showerthoughts.csv"},
        {4, "processed_data/techsupport.csv"}
    };

    map<int, string> names = {
        {1, "r/AskReddit"},
        {2, "r/explainlikeimfive"},
        {3, "r/Showerthoughts"},
        {4, "r/techsupport"}};
    // testing, make sure to remove in final version
    // replace AskReddit with the name of the csv
    int subreddit;
    int option_chose;
    bool submenu;

    // on startup
    intro();

    bool active = true;
    while (active)
    {
        submenu = true;

        // subreddit selection and exiting
        subreddit = subreddit_selection();
        if (subreddit == 5)
        {
            active = false;
            return 1;
        }

        // options once subreddit is chosen
        while (submenu)
        {
            option_chose = submenu_selection();

            // change options in here based on what subreddit and the different data structures
            //  make sure to keep the break or all will break
            switch (option_chose)
            {
            case 1: { //Prints top 15 posts
                auto it = paths.find(subreddit);
                priority_queue<Post,vector<Post>, MinUpvotes> topPosts;
                priority_queue<Post,vector<Post>, MaxUpvotes> bottomPosts;
                createLists(it->second,15,bottomPosts,topPosts);
                vector<Post> posts;
                createtopvector(topPosts, posts);
                cout << "Listing top posts for " << names[subreddit] << endl;
                printvector(posts);
                break;
            }
            case 2: { //Prints bottom 15 posts
                auto it = paths.find(subreddit);
                priority_queue<Post,vector<Post>, MinUpvotes> topPosts;
                priority_queue<Post,vector<Post>, MaxUpvotes> bottomPosts;
                createLists(it->second,15,bottomPosts,topPosts);
                vector<Post> posts;
                createbottomvector(bottomPosts, posts);
                cout << "Listing bottom posts for "  << names[subreddit] << endl;
                printvector(posts);
                break;
            }
            case 3:
            {
                auto it = paths.find(subreddit);
                if (it == paths.end()){
                    cout << "Unable to locate the selected subreddit dataset." << endl;
                    break;
                }

                cout << "Listing top words for subreddit" << endl;
                auto topWords = getTopWordsByAverageUpvotes(it->second, 15);
                if (topWords.empty()){
                    cout << "No word statistics are available yet for this dataset." << endl;
                }
                else{
                    printWordStats(topWords);
                }
                break;
            }
                case 4:
            {
                auto it = paths.find(subreddit);
                if (it == paths.end()){
                    cout << "Unable to locate the selected subreddit dataset." << endl;
                    break;
                }

                cout << "Listing bottom words for subreddit" << endl;
                auto bottomWords = getBottomWordsByAverageUpvotes(it->second, 15);
                if (bottomWords.empty()){
                    cout << "No word statistics are available yet for this dataset." << endl;
                }
                else{
                    printWordStats(bottomWords);
                }
                break;
            }
            case 5:
                cout << "Printing dataset information for " << names[subreddit] << endl;
                dataset_info(paths[subreddit]);
                break;
            case 6:
                // change subreddit case
                cout << "Choosing another subreddit" << endl;
                submenu = false;
                break;
            case 7:
                // exit case
                submenu = false;
                active = false;
                return 1;
            default:
                cout << "How Did We Get Here?" << endl;
            }

            cout << endl;
        }
    }
    return 1;
}