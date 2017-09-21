
#include <QApplication>

#include <iostream>
#include <QTimer>
#include <ctime>
#include <time.h>
#include <QString>
#include <QWidget>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "video.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <numeric>
#include <sstream>

#include <opencv2/highgui/highgui.hpp> //VideoCapture
#include <opencv2/imgproc/imgproc.hpp> //cvtColor
#include <opencv2/opencv.hpp> //opencv libraries

#include "../../../src/Utils/DrawDetections.h"
#include "../../../lib/bgslibrary/package_bgs/bgslibrary.h"
#include "../../../src/BGS/BkgSubtractionSelector.h"
#include "../../../src/SFGD/SFGDSelector.h"
#include "../../../src/DETECTOR/detectorselector.h"
#include "../../../src/DETECTOR/DefineObjectBlobList.h"
#include "../../../src/CLASSIFIER/ClassifierSelector.h"
#include "../../../src/Event/EventController.h"


//Global video capture


//Global pointers
BkgSubtractionSelector *bkg_selector;
SFDGSelector *sfgd_selector;
DetectorSelector *detec_selector;
BlobExtractor blob_extractor;
ClassifierSelector *classifier_selector;

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


/*********************************/
/*** MAIN PROGRAM FUNCTION     ***/
/*********************************/


int main(int argc, char *argv[])

{
    // Video class
    video Video;


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


    // ↓↓↓↓↓↓ UNCOMMENT IF WORKING IN PROGRAMMING  ENVIRONMENT ↓↓↓↓↓↓



    Video.bkg_method = 1;
    Video.sfgd_method  = 1;
    Video.classifier_method = 1;
    Video.detector_method= 3;

    Video.fileDir = "../datasets/AVSS/AVSS_corto.mov";
    string basename  = Video.fileDir.substr(Video.fileDir.find_last_of("/")+1);
    string::size_type const point_position(basename.find_last_of('.'));
    string videoName = basename.substr(0,point_position);

    if ( videoName.find("AVSS") != string::npos)
    {
        Video.contextMask = imread("../images/AVSS_Mask.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        if (Video.contextMask.empty())
        {
            cout << "Could not open mask image." << endl;
            return -1;
        }
        bitwise_not(Video.contextMask,Video.contextMask);
    }

    String folder_results  = "../results/";

    Video.fileResults = folder_results + videoName + "_PRUEBA_" + to_string(Video.bkg_method) + "_" + to_string( Video.sfgd_method) + "_"+ to_string( Video.classifier_method ) + "_" + to_string( Video.detector_method ) + "_"+ currentDateTime() + ".xml";
    Video.fileTime =  folder_results + videoName + "time_" + to_string(Video.bkg_method) + "_" + to_string( Video.sfgd_method) + "_"+ to_string( Video.classifier_method ) + "_" + to_string( Video.detector_method ) + "_"+ currentDateTime() + ".time";
    Video.DirImages = "../results/images/";

    cout << "File results: " << Video.fileResults << endl;
    cout << "video filedir " << Video.fileDir << endl;

    // ↑↑↑↑↑ UNCOMMENT IF WORKING IN PROGRAMMING  ENVIRONMENT ↑↑↑↑↑↑



    // ↓↓↓↓↓↓ UNCOMMENT IF WORKING IN TERMINAL OR .SH SCRIPT ↓↓↓↓↓↓

    /*
    // Check number of mandatory input arguments

    if (argc < 7 )
    {
        cout << "Not enough arguments" << endl;
        exit(0);

    }
    else
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

        string basename  = Video.fileDir.substr(Video.fileDir.find_last_of("/")+1);
        string::size_type const point_position(basename.find_last_of('.'));
        string videoName = basename.substr(0,point_position);

        // RESULTS FOLDER
        String folder_results = argv[6];

        // XML file with results (.xml)
        Video.fileResults = folder_results + videoName + "_"+ to_string(Video.bkg_method) + "_" + to_string( Video.sfgd_method) + "_"+ to_string( Video.classifier_method ) + "_" + to_string( Video.detector_method ) + "_"+ currentDateTime() + ".xml";
        cout << "Results will be saved in " << Video.fileResults << endl;

        // file with execution times (.time)
        Video.fileTime = folder_results + videoName + "_"+ to_string(Video.bkg_method) + "_" + to_string( Video.sfgd_method) + "_"+ to_string( Video.classifier_method ) + "_" + to_string( Video.detector_method ) + "_"+ currentDateTime() + ".time";



    }

    // CONTEXT MASK
    if(argc == 8)
    {

        Video.contextMask = imread(argv[7]);
        cout << " A context mask will be used " << endl;
    }


    */


    // ↑↑↑↑↑ UNCOMMENT IF WORKING IN TERMINAL OR .SH SCRIPT ↑↑↑↑↑




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


    /********************/
    /*** MAIN PROCESS ***/
    /********************/

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
        return -1;
    }
    else
    {
        cout << "Video successfully opened" << endl;
        cout << Video.framerate << " fps. Size: " << Video.cap.get(CV_CAP_PROP_FRAME_WIDTH) << " x " << Video.cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    }


    /******** VARIABLE SETTING ********/

    // Seconds to consider an object as static
    Video.time_to_static = 10;
    cout << "Time (seconds) to static: " << Video.time_to_static << endl;

    // Show results if true
    Video.ShowResults = true;

    // Save results images if true
    Video.SaveImages = false;

    // Save XML results file if true
    Video.SaveResults = false;

    // Detect people in every frame if true
    Video.DetectPeopleAlways = false;
    cout << "Detecting people only when something static is detected." << endl;


    if ( Video.DetectPeopleAlways )
    {
        cout << "Detecting people in every frame." << endl;
    }

    if (Video.SaveResults == true)
    {

        cout << "Saving results and execution times." << endl;

        /******** EVENT FILE INITIALIZATION ********/

        // XML results file initialization

        //To const char* conversion (video file)
        char *sourceFile = new char[Video.fileDir.length() + 1];
        strcpy(sourceFile, Video.fileDir.c_str());

        //To const char* conversion (results file)
        char *resultsFile = new char[ Video.fileResults.length() + 1];
        strcpy(resultsFile,  Video.fileResults.c_str());

        Video.evtControl = new EventController();

        Video.evtControl->init(Video.rows, Video.cols ,sourceFile, resultsFile, Video.totalNumFrames,Video.framerate, Video.time_to_static, 0);

        // Time file initialization
        char *pfile = (char *)malloc(strlen(Video.fileTime.c_str()) + 1);
        strcpy(pfile, Video.fileTime.c_str());
        Video.file_time = fopen(pfile, "w+");

        fprintf( Video.file_time,"Execution times\n");
        fprintf( Video.file_time,"Frame    Total     BKG     SFGD      PD    CLASS    WRITE\n");

    }



    //Save images directory
    Video.DirImages = "../../../results/images/";

    if (Video.SaveImages == true)
    {
        cout << "Images will be saved in " <<  Video.DirImages << endl;
    }



    /******** LOOP OVER THE VIDEO FILE ********/

    for ( ; ; )
    {

        // Time variables initialization
        double elapsedTime_total  = 0;
        double elapsedTime_bkg  = 0;
        double elapsedTime_sfgd  = 0;
        double elapsedTime_pd  = 0;
        double elapsedTime_class  = 0;
        double elapsedTime_write  = 0;

        // Start the clock for measuring total consumption time
        clock_t start_total = clock();


        // Get frame
        Mat frame;
        Video.cap >> frame;
        Video.numFrame = Video.cap.get(CAP_PROP_POS_FRAMES);



        /******** FRAME PROCESSING ********/

        if (frame.data)
        {

            // imshow("FRAME", frame);
            // waitKey(10);

            cout << "Frame " << Video.numFrame << endl;

            // First frame
            if (Video.numFrame  == 1)
            {

                // Initializations
                bkg_selector = new BkgSubtractionSelector(Video.bkg_method);
                bkg_selector->init(frame);

                sfgd_selector = new SFDGSelector(Video.sfgd_method,Video.framerate,Video.time_to_static);
                sfgd_selector->init(frame);

                detec_selector= new DetectorSelector(Video.detector_method);
                detec_selector->init();

                classifier_selector = new ClassifierSelector(Video.classifier_method);
                classifier_selector->init();

                Video.list_objects = new BlobList<ObjectBlob*>();


            }

            /******** BACKGROUND SUBTRACTION  ********/


            clock_t start_bkg = clock();

            bkg_selector->process(frame, Video);
            waitKey(10);
            clock_t finish_bkg = clock();
            elapsedTime_bkg = (double)(finish_bkg - start_bkg)/CLOCKS_PER_SEC;


            /******** STATIC FOREGROUND DETECTION  ********/

            clock_t start_sfgd = clock();

            sfgd_selector->process(frame,bkg_selector->GetForegroundImage(),bkg_selector->GetBGModel(),Video);


            // Get static foreground mask
            Mat tmp = sfgd_selector->GetStaticForeground().clone();
            Mat ch[3];
            split(tmp, ch);
            tmp = ch[0];

            // Extract all blobs in static foreground
            blob_extractor.extractBlobs(tmp,false);

            // Create BlobList containing all blobs in static foreground
            vector<cvBlob> *BlobList = blob_extractor.getBlobList();


            clock_t finish_sfgd = clock();
            elapsedTime_sfgd = (double)(finish_sfgd - start_sfgd)/CLOCKS_PER_SEC;

            /******** PEOPLE DETECTION  ********/

            // Detecting people in every frame or when something static has been detected (BlobList !empty)
            if (Video.DetectPeopleAlways == true || (Video.DetectPeopleAlways == false && BlobList->size()) )
            {

                clock_t start_pd = clock();

                detec_selector->process(frame);

                // Filter detections with nms
                detec_selector->non_max_suppresion(detec_selector->found,Video.found_filtered,0.5);

                // Show people detections if show results variable is true
                Mat peopleDetection = DrawDetections(frame,Video.found_filtered, BlobList, Video);


                clock_t finish_pd = clock();
                elapsedTime_pd = (double)(finish_pd - start_pd)/CLOCKS_PER_SEC;


                // Create a static objects list (Video.list_objects) by filtering the static foreground blobs (BlobList)
                // by removing static foreground blobs due to people detections (Video.found_filtered) and considering
                // the context mask (if applicable)

                DefineObjectBlobList(BlobList, Video.found_filtered, Video.list_objects,Video.contextMask);

                cout << "Frame " << Video.numFrame << ". Num objects blobs = " << Video.list_objects->getBlobNum() <<  ".  Number of people detected: " << detec_selector->found.size() << endl;

            }


            /******** CLASSIFIER  ********/


            clock_t start_class = clock();


            // Only if some static object is detected

            if (Video.list_objects->getBlobNum()){

                Mat clss;
                clss = classifier_selector->process(frame, bkg_selector->GetBGModel(), tmp, bkg_selector->GetForegroundImage(),Video.list_objects,Video);

            }

            clock_t finish_class = clock();
            elapsedTime_class = (double)(finish_class - start_class)/CLOCKS_PER_SEC;


            /******** WRITE RESULTS   ********/

            clock_t start_write = clock();

            if (Video.SaveResults == true)
            {
                // Check events

                Video.evtControl->checkEvents(Video.list_objects);

            }

            clock_t finish_write = clock();
            elapsedTime_write = (double)(finish_write - start_write)/CLOCKS_PER_SEC;

            Video.list_objects->clear();

        }
        else
            // End of the video
        {
            cout << "Video has finished." << endl;


            // Release video capture
            Video.cap.release();
            destroyAllWindows();
            fclose(Video.file_time);


            if (Video.SaveResults == true)
            {
                // WRITE EVENTS
                Video.evtControl->checkFinalPastEvents();
                Video.evtControl->~EventController();
                Video.evtControl->pwriter.~EventWriter();
            }


            return -1;


        }


        clock_t finish_total = clock();
        elapsedTime_total = (double)(finish_total - start_total)/CLOCKS_PER_SEC;
        cout << elapsedTime_total << endl;
        
        if (Video.SaveResults == true)
        {

            /******** TIME CONSUPTION WRITTING (For each frame) ********/
            fprintf(Video.file_time,"%d     %2.6f %2.6f %2.6f %2.6f %2.6f %2.6f\n",Video.numFrame,elapsedTime_total ,elapsedTime_bkg,elapsedTime_sfgd,elapsedTime_pd,elapsedTime_class,elapsedTime_write);
        }

    }



    return -1;


}













