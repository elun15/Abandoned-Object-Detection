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


#include "../../../lib/bgslibrary/package_bgs/FrameDifferenceBGS.h"
#include "../../../lib/bgslibrary/package_bgs/pl/BackgroundSubtractorLOBSTER.h"
#include "../../../lib/bgslibrary/package_bgs/pl/BackgroundSubtractorLBSP.h"
#include "../../../lib/BackgroundSubtractorPAWCS.h"

#include "../../../lib/bgslibrary/package_bgs/db/imbs.hpp"
#include "../../../lib/bgslibrary/package_bgs/db/IndependentMultimodalBGS.h"
#include "video.h"


using namespace std;
using namespace cv;


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
    Mat SecuenceROI;

    // Subtraction techniques objects declaration

    FrameDifferenceBGS *MethodFrameDifference;
    BackgroundSubtractorLOBSTER *pLOBSTERBGS;
    BackgroundSubtractorPAWCS *pPAWCSBGS;

    Ptr<BackgroundSubtractorMOG2> pMOG2;
    Ptr<BackgroundSubtractorKNN> pKNN;
    IndependentMultimodalBGS *pIMBSBGS;



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



