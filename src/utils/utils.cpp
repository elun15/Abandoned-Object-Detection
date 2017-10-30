#include <time.h> 	//time
#include <sstream>  // std::stringstream
#include <iomanip>	// std::setw std::setfill

#include "utils.h"

using namespace std;

///Function to get the current time as string
const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%H-%M", &tstruct);

    return buf;
}

///Function to perform the conversion from int to string
const std::string to_string_(int i,int padding)
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(padding) << i;
    return ss.str();
}
