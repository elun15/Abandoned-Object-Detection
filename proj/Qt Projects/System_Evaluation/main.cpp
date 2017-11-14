#include <opencv2/opencv.hpp> //opencv libraries

#include "../src/aod.h" // AOD system class
#include "../src/Config.h" // Structure with program settings

using namespace cv;
using namespace std;


/*********************************/
/*** MAIN PROGRAM FUNCTION     ***/
/*********************************/
// To check command usage see Config::print_usage()
int main(int argc, char *argv[])
{
    // Initialize all settings to configure the AOD system
    //Config cfg(argc,argv);
   // Config cfg("./../datasets/AVSS2007/AVSSS07_EASY.mpg","./../results/");
    //Config cfg("./../datasets/CAVIAR/LeftBag_AtChair.mpeg","./../results/");
    //Config cfg("./../datasets/VISOR_test/visor_1212674131705_pacco6.avi", "./../results");
    Config cfg("./../datasets/VISOR/visor_Video00.avi","./../results/");
    cfg.time_to_static = 10; //in seconds

    //videoreader
    cv::VideoCapture cap(cfg.inputPath);
    if (!cap.isOpened()){
        cout << "Could not open the video file" << endl;
        exit(EXIT_FAILURE);
    }
    else
        cfg.get_video_info(cap);

    // AOD class encapsulating all the processes
    AOD system;

    // Start the clock for measuring total time to process the video sequence
    clock_t start_total = clock();

    // ******** LOOP OVER THE VIDEO FILE *******
    Mat frame;
    for ( ; ; )
    {
        cap >> frame; // Get frame
        cfg.numFrame = cap.get(CAP_PROP_POS_FRAMES); // Get frame number

        // ******** FRAME PROCESSING *******
        if (frame.data)
        {
            cout << "Frame " << cfg.numFrame << endl;

            // First frame
            if (cfg.numFrame  == 1)
                system.init(frame,cfg);

            system.processFrame(frame,cfg);
        }
        else  // End of the video
        {
            double elapsedTime_totalVideo  = (double)(clock() - start_total)/CLOCKS_PER_SEC;
            cout << "Total processing time (secs): " << elapsedTime_totalVideo << endl;
            break;
        }
    }

    //Release resources
    cap.release(); // Release video capture

    return 1;
}
