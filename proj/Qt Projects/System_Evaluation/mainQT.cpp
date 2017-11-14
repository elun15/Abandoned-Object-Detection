#include <iostream>
#include <ctime>
#include <time.h>

#include "../../../src/settings.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <numeric>
#include <sstream>

#include <opencv2/highgui/highgui.hpp> //VideoCapture
#include <opencv2/imgproc/imgproc.hpp> //cvtColor
#include <opencv2/opencv.hpp> //opencv libraries


#include "../../../src/aod.h"




using namespace cv;
using namespace std;


/******************************/
/*** IMPLEMENTED ALGORITHMS ***/
/******************************/

// 1: LOBSTER
// 2: PAWCS
// 3: MOG2
// 4: KNN
// 5: Multimodal
// 6: SUBSENSE

// 1: Subsampling
// 2: Acc Mask

// 1: High Gradient
// 2: Histogram

// 1: HOG
// 2: DPM
// 3: ACF


/*******************************/
/*** INPUT TERMINAL COMMANDS ***/
/*******************************/

// ARGUMENTS ORDER:

// 1: bkg subtraction method
// 2: sfgd method
// 3: classifier method
// 4: people detector method
// 5: video file directory
// 6: video name
// 7: results folder
// 8: video context mask directory (not mandatory)


/*********************************/
/*** MAIN PROGRAM FUNCTION     ***/
/*********************************/


int main(int argc, char *argv[])

{
    // AOD class encapsulating all the processes
    AOD system;

    // Video class
    settings Video;

    // Initialize all video variables
    Video = Video.init(argc,argv, Video);

    // Open video file
    Video = Video.open(Video);


    // Start the clock for measuring total consumption time of the whole video sequence
    double elapsedTime_totalVideo  = 0;
    clock_t start_total = clock();

    /******** LOOP OVER THE VIDEO FILE ********/

    for ( ; ; )
    {

        // Get frame
        Mat frame;
        Video.cap >> frame;
        Video.numFrame = Video.cap.get(CAP_PROP_POS_FRAMES);


        /******** FRAME PROCESSING ********/
        if (frame.data)
        {

            cout << "Frame " << Video.numFrame << endl;

            // First frame
            if (Video.numFrame  == 1)
            {
                Video = system.init(Video,frame);
            }

           Video = system.processFrame(Video, frame);


        }
        else  // End of the video
        {

            system.finish(Video);

            clock_t finish_total = clock();

            elapsedTime_totalVideo = (double)(finish_total - start_total)/CLOCKS_PER_SEC;
            cout << "Total processing time (secs): " << elapsedTime_totalVideo << endl;

            return -1;
        }



    }
    return -1;





}













