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

    Config cfg;

    if (argc == 10)
    {
        cout << " Using terminal input parameters "<< endl;
        cfg.QT_execution = false;
        cfg.init(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),argv[5],argv[6],atoi(argv[7]),atoi(argv[8]),atoi(argv[9]));

    }
    else
    {
        cout << " Using QT input parameters "<< endl;
        cfg.QT_execution = true;
        cfg.init(2,2,3,1, "./../datasets/PETS2006_S5/PETS2006_S5_C3.mpeg","./../results/",30,DEFAULT_FLAG_NEARPEOPLE, DEFAULT_FLAG_MASK);
       // cfg.init(4,2,1,3, "./../datasets/PETS2006_S5/PETS2006_S5_C3.mpeg","./../results/",30,DEFAULT_FLAG_NEARPEOPLE, DEFAULT_FLAG_MASK);
    }



    //videoreader
    cv::VideoCapture cap(cfg.inputPath);
    if (!cap.isOpened())
    {
        cout << "Could not open the video file: " << cfg.inputPath << endl;
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
            //cout << "Frame " << cfg.numFrame << endl;

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
