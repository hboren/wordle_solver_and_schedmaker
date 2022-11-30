// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<std::string> all_possible;
    set<string> all_english;

    std::string floating_pass = floating;
    std::string in_pass = in;

    //call helper
    wordle_helper(in, floating_pass, all_possible, 0);

    //keep only english words out of all the possible letter combinations
   for(set<string>::iterator it=all_possible.begin(); it != all_possible.end(); ++it){
        if(dict.find(*it) != dict.end()){
            all_english.insert(*it);
        }
    }

    return all_english;

}

// Define any helper functions here
void wordle_helper(
    std::string in,
    std::string float_, 
    std::set<std::string>& result, 
    int pos)
{

    //if the word we are making is complete, add it to our results
    if(pos == (int)in.size()){
        result.insert(in);
        return;
    }

    //if no letter fixed at current position:
    if(in[pos] == '-'){
        //loop through the floating letters
        for(int h = 0; h < (int)float_.size(); h++){
            string float_temp = float_;
            //set current position to current floating letter and remove from temp float
            in[pos] = float_[h];
            float_temp.erase(h,1);
            //recurse to next position
            wordle_helper(in, float_temp, result, pos + 1);
            //reset changes
            in[pos] = '-';
            float_temp = float_;
        }
        //if the current position is empty or we have used all floating letters
        //loop over alphabet, try each letter, if still valid, recurse to next position
        //then reset
        if(in[pos] == '-' || float_.size() == 0){
            for(char j = 'a'; j <= 'z'; j++){
                    in[pos] = j;
                //if it is a valid word, aka it will use all yellows
                    if(is_valid(in, float_)){
                        wordle_helper(in, float_, result, pos + 1);
                    }
                in[pos] = '-';
            }
        }
    }
    //otherwise just add green letter and continue
    else{
        return wordle_helper(in, float_, result, pos + 1);
    }
}

//checks if word in current state is a valid word, AKA will be able to use all floats
bool is_valid(std::string in, std::string float_){
    int num_open = 0;
    for(int i = 0; i < (int)in.size(); i++){
        if(in[i] == '-')
            num_open++;
    }
    //if there are less open letters than floats remaining, not valid, otherwise valid
    if(num_open < (int)float_.size()){
        return false;
    }
    return true;
}
