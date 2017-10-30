#include <opencv2/opencv.hpp> //opencv libraries

#include "pd/PDselector.h" // AOD system class
#include "Config.h" // Structure with program settings

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
	Config cfg("../../../datasets/VISOR_test/visor_Video00.avi","../../../results");//relative paths from workspace "./AOD_System/proj/Eclipse_Projects/")
	cfg.m_pd = PD_HOG;
	cfg.m_pd = PD_DPM;
	cfg.m_pd = PD_ACF;

	//videoreader
	cv::VideoCapture cap(cfg.inputPath);
	if (!cap.isOpened()){
		cout << "Could not open the video file" << endl;
		exit(EXIT_FAILURE);
	}
	else
		cfg.get_video_info(cap);

    // AOD class encapsulating all the processes
    PDselector pD((PD_type)cfg.m_pd,cfg.ShowResults,cfg.SaveImages,cfg.DirImages.c_str(),20);
    pD.init();

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
            pD.process(frame,cfg.numFrame);
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
