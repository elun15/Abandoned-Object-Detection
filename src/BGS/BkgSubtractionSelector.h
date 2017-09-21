#ifndef BGS_SELECTOR_H
#define BGS_SELECTOR_H

#endif // BGS_SELECTOR_H

/************************************************************************************/
/* Header file of BGS_selector.cpp                                                  */
/* This class allows us to choose a background subtraction method                   */
/*                                                                                   */
/************************************************************************************/

//Se incluyen los ficheros cabecera de las distintas tecnicas de substraccion de fondo implementadas.
#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include "../../../lib/bgslibrary/package_bgs/bgslibrary.h"
#include <opencv2/video/background_segm.hpp>
#include "video.h"


using namespace std;
using namespace cv;
using namespace bgslibrary::algorithms;


class BkgSubtractionSelector
{


public:

    // Variables declaration

    int int_BGSid;
    double learningRateOverride;
    double learningRate;

    Mat img_input;
    Mat img_output;
    Mat img_bgmodel;


    // Subtraction techniques objects declaration


    IBGS *bgs;

    // Constructors
    BkgSubtractionSelector();
    BkgSubtractionSelector(int BGSid);  // BGSid: technique ID


    //Destructor
    ~BkgSubtractionSelector();

    //Other functions

    virtual void init(Mat frame);
    virtual void process(Mat frame, video Video);


    virtual Mat GetForegroundImage(); // antes era vector
    Mat GetBGModel();
    //double GetBackgroundRatio();
    //void finish(void);


};



