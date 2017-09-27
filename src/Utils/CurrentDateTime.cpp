#include <opencv2/opencv.hpp>
#include <iostream>
#include "CurrentDateTime.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sstream>

using namespace cv;
using namespace std;

/*********************************/
/*** GET CURRENT TIME FUNCTION ***/
/*********************************/

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

