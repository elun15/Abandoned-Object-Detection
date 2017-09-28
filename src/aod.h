#ifndef AOD_H
#define AOD_H

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>


#include "../../../src/BGS/BkgSubtractionSelector.h"
#include "../../../src/SFGD/SFGDSelector.h"
#include "../../../src/CLASSIFIER/ClassifierSelector.h"
#include "../../../src/DETECTOR/detectorselector.h"
#include "../../../src/Event/EventController.h"

using namespace cv;

class AOD
{

public:

    AOD();
   ~AOD();

    //Global pointers
    BkgSubtractionSelector *bkg_selector;
    SFDGSelector *sfgd_selector;
    DetectorSelector *detec_selector;
    BlobExtractor blob_extractor;
    ClassifierSelector *classifier_selector;

    EventController *evtControl;



    double elapsedTime_bkg;
    double elapsedTime_sfgd;
    double elapsedTime_pd;
    double elapsedTime_class;
    double elapsedTime_write;
    double elapsedTime_frame;

   settings init(settings Video, Mat frame);
   settings processFrame(settings Video, Mat frame);
   void finish(settings Video);



};




#endif // AOD_H
