#include "utilities.hpp"


// simple function which moves a variable into a range
// by a given increment, and then ensures that it keeps 
// moving inside that range. 
// It returns 0 if there were no changes to direction, 
// 1 else, which can be used to count number of changes to direction. 
unsigned int rangedInput(float &var, float &increment, 
    float lowerBound, float upperBound) 
{
    bool changeDir = ((var <= lowerBound) && (increment < 0)) 
                        || ((var >= upperBound) && (increment > 0));
    if (changeDir)
        increment = -increment;

    var += increment;
    return static_cast<unsigned int>(changeDir);
}