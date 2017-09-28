#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#include "../../../src/BLOBS/BlobExtractor.h"
#include "../../../src/BLOBS/BlobList.h"
#include "../../../src/BLOBS/ObjectBlob.h"
#include "../../../src/Event/EventController.h"



using namespace cv;

class video
{

public:
    video();

    video init(int argc, char *argv[], video Video);
    video open(video Video);
   ~video();

    VideoCapture cap;

    int cols;
    int rows;


    // PROGRAM OPTIONS
    bool ShowResults = false; // Boolean to show or not the results
    bool DetectPeopleAlways; // Detect people in every frame if true
    bool SaveResults;  // Save XML results file if true
    bool SaveImages; // Save results images if true

    // DIRECTORIES
    String fileDir;   // INPUT VIDEO FILE DIRECTORY
    String DirImages;
    String fileResults; // XML file with results (.xml)
    String fileTime;  // file with execution times (.time)
    FILE *file_time = NULL;
    String folder_results;

    vector<Point> maskPoints;
    Mat contextMask1;
    Mat contextMask;


    int time_to_static; // Seconds
    int numFrame;
    int totalNumFrames;
    double framerate;

    // METHODS

    int bkg_method;
    int sfgd_method ;
    int detector_method ;
    int classifier_method ;


    size_t i = 0;
    size_t j = 0;


    vector<Rect> found_filtered;
    BlobList<ObjectBlob *>*list_objects;

    EventController *evtControl;





};

#endif
