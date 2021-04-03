#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours = seconds / (60*60);
    int minutes = (seconds % (60*60))/60;
    int second =  seconds % 60 ;
    return (hours   < 10?"0":"")+std::to_string(hours)  + ":"
         + (minutes < 10?"0":"")+std::to_string(minutes)+ ":"
         + (second  < 10?"0":"")+std::to_string(second); 
    
}