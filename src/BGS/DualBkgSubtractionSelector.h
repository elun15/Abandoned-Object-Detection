#ifndef DUALBACKGROUNDSELECTOR_H
#define DUALBACKGROUNDSELECTOR_H

#endif // DUALBACKGROUNDSELECTOR_H

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
#include "../../../src/settings.h"
#include "BkgSubtractionSelector.h"


using namespace std;
using namespace cv;
using namespace bgslibrary::algorithms;


class DualBkgSubtractionSelector : public BkgSubtractionSelector
{


public:

    // Variables declaration

    // int_BGSid;
   // double learningRateOverride;
   // double learningRate;

   // Mat img_input; //Frame
//    Mat img_output;
    Mat img_outputLong; //Long Foreground mask
    Mat img_outputshort; //Short Foreground mask
   // Mat img_bgmodel; // Background image
    Mat img_bgmodelLong; // Background image

    // Subtraction techniques objects declaration

    //IBGS * bgs;
    IBGS *bgsShort;
    IBGS *bgsLong;

    // Constructors
    DualBkgSubtractionSelector();
    DualBkgSubtractionSelector(int BGSid);  // BGSid: technique ID


    //Destructor
    ~DualBkgSubtractionSelector();

    //Other functions

    virtual void init(Mat frame);
    virtual void process(Mat frame, settings Video);


    virtual std::vector<Mat> GetForegroundImage(); // antes era vector
   // Mat GetBGModel();
    //double GetBackgroundRatio();
    //void finish(void);


};



