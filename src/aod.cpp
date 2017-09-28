
#include <iostream>
#include <ctime>
#include <time.h>

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "aod.h"


#include "../../../src/BGS/BkgSubtractionSelector.h"
#include "../../../src/SFGD/SFGDSelector.h"
#include "../../../src/CLASSIFIER/ClassifierSelector.h"
#include "../../../src/DETECTOR/detectorselector.h"

#include "../../../src/Utils/DrawDetections.h"
#include "../../../lib/bgslibrary/package_bgs/bgslibrary.h"
#include "../../../src/DETECTOR/DefineObjectBlobList.h"
#include "../../../src/Event/EventController.h"
#include "../../../src/Utils/CurrentDateTime.h"


AOD::AOD(){}
AOD::~AOD(){}

settings AOD::init(settings Video, Mat frame){

    // Initializations
    this->bkg_selector = new BkgSubtractionSelector(Video.bkg_method);
    this->bkg_selector->init(frame);

    this->sfgd_selector = new SFDGSelector(Video.sfgd_method,Video.framerate,Video.time_to_static);
    this->sfgd_selector->init(frame);

    this->detec_selector= new DetectorSelector(Video.detector_method);
    this->detec_selector->init();

    this->classifier_selector = new ClassifierSelector(Video.classifier_method);
    this->classifier_selector->init();

    Video.list_objects = new BlobList<ObjectBlob*>();


    //ADD


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
        evtControl = new EventController();

        evtControl->init(Video.rows, Video.cols ,sourceFile, resultsFile, Video.totalNumFrames,Video.framerate, Video.time_to_static, 0);

        // Time file initialization
        char *pfile = (char *)malloc(strlen(Video.fileTime.c_str()) + 1);
        strcpy(pfile, Video.fileTime.c_str());
        Video.file_time = fopen(pfile, "w+");

        fprintf( Video.file_time,"Execution times\n");
        fprintf( Video.file_time,"Frame    Total     BKG     SFGD      PD    CLASS    WRITE\n");

    }

    return Video;
}

settings AOD::processFrame(settings Video, Mat frame){


    // Time variables initialization

    elapsedTime_bkg  = 0;
    elapsedTime_sfgd  = 0;
    elapsedTime_pd  = 0;
    elapsedTime_class  = 0;
    elapsedTime_write  = 0;
    elapsedTime_frame = 0;


    /******** BACKGROUND SUBTRACTION  ********/

    clock_t start_bkg = clock();

    this->bkg_selector->process(frame, Video);
    waitKey(10);
    clock_t finish_bkg = clock();
    elapsedTime_bkg = (double)(finish_bkg - start_bkg)/CLOCKS_PER_SEC;

    /******** STATIC FOREGROUND DETECTION  ********/

    clock_t start_sfgd = clock();

    this->sfgd_selector->process(frame, this->bkg_selector->GetForegroundImage(), this->bkg_selector->GetBGModel(),Video);

    // Get static foreground mask
    Mat tmp =  this->sfgd_selector->GetStaticForeground().clone();

    // Extract all blobs in static foreground
    this->blob_extractor.extractBlobs(tmp,false);

    // Create BlobList containing all blobs in static foreground
    vector<cvBlob> *BlobList =  this->blob_extractor.getBlobList();


    clock_t finish_sfgd = clock();
    elapsedTime_sfgd = (double)(finish_sfgd - start_sfgd)/CLOCKS_PER_SEC;

    /******** PEOPLE DETECTION  ********/

    // Detecting people in every frame or when something static has been detected (BlobList !empty)
    if (Video.DetectPeopleAlways == true || (Video.DetectPeopleAlways == false && BlobList->size()) )
    {

        clock_t start_pd = clock();

        this->detec_selector->process(frame);

        // Filter detections with nms
        this->detec_selector->non_max_suppresion(detec_selector->found,Video.found_filtered,0.5);

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
        clss = this->classifier_selector->process(frame, this->bkg_selector->GetBGModel(), tmp, this->bkg_selector->GetForegroundImage(),Video.list_objects,Video);

    }

    clock_t finish_class = clock();
    elapsedTime_class = (double)(finish_class - start_class)/CLOCKS_PER_SEC;


    /******** WRITE RESULTS   ********/

    clock_t start_write = clock();

    if (Video.SaveResults == true)
    {
        // Check events

        evtControl->checkEvents(Video.list_objects);

    }

    clock_t finish_write = clock();
    elapsedTime_write = (double)(finish_write - start_write)/CLOCKS_PER_SEC;


    elapsedTime_frame = (double)(finish_write - start_bkg)/CLOCKS_PER_SEC;
    Video.list_objects->clear();

    /******** TIME CONSUPTION WRITTING (For each frame) ********/

    if (Video.SaveResults == true)
    {
        fprintf(Video.file_time,"%d     %2.6f %2.6f %2.6f %2.6f %2.6f %2.6f\n",Video.numFrame, elapsedTime_frame ,elapsedTime_bkg,elapsedTime_sfgd,elapsedTime_pd,elapsedTime_class,elapsedTime_write);
    }


return Video;


}


void AOD::finish(settings Video){



    if (Video.SaveResults == true)
    {
        // WRITE EVENTS
        evtControl->checkFinalPastEvents();
        evtControl->~EventController();
        evtControl->pwriter.~EventWriter();
    }

    // Release video capture
    Video.cap.release();
    destroyAllWindows();
    fclose(Video.file_time);



    cout << "Video has finished." << endl;



}








