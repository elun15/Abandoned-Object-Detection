

#include <iostream>


#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "settings.h"
#include "../../../src/Event/EventController.h"
#include "../../../src/Utils/CurrentDateTime.h"



settings::settings(){}
settings::~settings(){}

settings settings::init(int argc, char *argv[], settings Video)
{

    /******** VARIABLE SETTING ********/

    // Seconds to consider an object as static
    Video.time_to_static = 10;
    cout << "Time (seconds) to static: " << Video.time_to_static << endl;

    // Show results if true
    Video.ShowResults = false;

    // Save results images if true
    Video.SaveImages = false;
    if (Video.SaveImages == true)
    {
        cout << "Images will be saved in " <<  Video.DirImages << endl;
    }

    // Save XML results file if true
    Video.SaveResults = true;

    // Detect people in every frame if true
    Video.DetectPeopleAlways = false;
    if ( Video.DetectPeopleAlways )
    {
        cout << "Detecting people in every frame." << endl;
    }


    // Using Default parameters
    if (argc < 7 )
    {
        cout << "Executing with default parameters" << endl;

        // BKG METHOD
        Video.bkg_method = 6;

        // SFGD METHOD
        Video.sfgd_method = 4;

        // CLASSIFIER METHOD
        Video.classifier_method = 1;

        // PEOPLE DETECTOR METHOD
        Video.detector_method = 3;

        // INPUT VIDEO FILE DIRECTORY
        //Video.fileDir = "../datasets/VISOR/visor_Video00.avi";
        Video.fileDir = "../datasets/AVSS/AVSSS07_MEDIUM.mpg";
        //Video.fileDir = "/storage-disk/AOD/datasets/AVSS2007/AVSSS07_EASY.mpg";

        // RESULTS FOLDER
        Video.folder_results  = "../results/";

    }
    else //Using input parameters
    {
        // BKG METHOD
        int bkg;
        istringstream ss(argv[1]);
        ss >> bkg;
        Video.bkg_method = bkg;
        cout << "Selectec bkg method: " << bkg << endl;

        // SFGD METHOD
        int sfgd;
        istringstream ss2(argv[2]);
        ss2 >>sfgd;
        Video.sfgd_method = sfgd;
        cout << "Selected sfgd method: " << sfgd << endl;

        // CLASSIFIER METHOD
        int classif;
        istringstream ss3(argv[3]);
        ss3 >> classif;
        Video.classifier_method= classif;
        cout << "Selcted classifier method: " << classif << endl;

        // PEOPLE DETECTOR METHOD
        int pd;
        istringstream ss4(argv[4]);
        ss4 >> pd;
        Video.detector_method= pd;
        cout << "Selected people detector method:" << pd << endl;

        // INPUT VIDEO FILE DIRECTORY
        Video.fileDir = argv[5];
        cout << "Input video dir: " << argv[5] << endl;

        // RESULTS FOLDER
        Video.folder_results = argv[6];

    }

    // Compute videoName variable, used for saving the results
    string basename  = Video.fileDir.substr(Video.fileDir.find_last_of("/")+1);
    string::size_type const point_position(basename.find_last_of('.'));
    string videoName = basename.substr(0,point_position);

    // Check if it is required to apply context mask
    if ( videoName.find("AVSS") != string::npos)
    {
        Video.contextMask1 = imread("./datasets/AVSS2007/AVSS_Mask_1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        if (Video.contextMask1.empty())
        {
            cout << "Could not open mask image." << endl;
            exit(EXIT_FAILURE);
        }
        bitwise_not(Video.contextMask1,Video.contextMask1);

        Video.contextMask = imread("./datasets/AVSS2007/AVSS_Mask_2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        if (Video.contextMask.empty())
        {
            cout << "Could not open mask image." << endl;
            exit(EXIT_FAILURE);
        }
    }


    // XML file with results (.xml)
    Video.fileResults = Video.folder_results + videoName + "_"+ to_string(Video.bkg_method) + "_" + to_string( Video.sfgd_method) + "_"+ to_string( Video.classifier_method ) + "_" + to_string( Video.detector_method ) + "_"+ currentDateTime() + ".xml";
    cout << "Results will be saved in " << Video.fileResults << endl;

    // file with execution times (.time)
    Video.fileTime = Video.folder_results + videoName + "_"+ to_string(Video.bkg_method) + "_" + to_string( Video.sfgd_method) + "_"+ to_string( Video.classifier_method ) + "_" + to_string( Video.detector_method ) + "_"+ currentDateTime() + ".time";

    //Save images directory
    Video.DirImages = "../../../results/images/";

    return Video;


}

settings settings::open(settings Video){

    // Read video properties
    Video.cap.open(Video.fileDir);
    Video.framerate = Video.cap.get(CAP_PROP_FPS);
    Video.totalNumFrames = Video.cap.get(CAP_PROP_FRAME_COUNT);
    Video.rows = Video.cap.get(CAP_PROP_FRAME_HEIGHT);
    Video.cols = Video.cap.get(CAP_PROP_FRAME_WIDTH);


    //Check if Videocapture variable has been correctly opened
    if (!Video.cap.isOpened())
    {
        cout << "Could not open the video file" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Video successfully opened" << endl;
        cout << Video.framerate << " fps. Size: " << Video.cap.get(CV_CAP_PROP_FRAME_WIDTH) << " x " << Video.cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    }


    // if (Video.SaveResults == true)


    return Video;




}
