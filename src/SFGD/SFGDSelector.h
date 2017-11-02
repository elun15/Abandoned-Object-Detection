#ifndef SFGDSELECTOR_H
#define SFGDSELECTOR_H

#endif // SFGD_SELECTOR_H


#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "../../../src/SFGD/StaticMaskExtractor.h"
#include "../../../src/SFGD/StaticMaskExtractor_subsampling.h"
#include "../../../src/SFGD/StaticMaskExtractor_AccMask.h"
#include "../../../src/SFGD/StaticMaskExtractor_DualBkg.h"

#include "../../../src/SFGD/StaticMaskExtractor_HistoryImages.h"

#include "../../../src/BGS/BkgSubtractionSelector.h"

using namespace std;
using namespace cv;

//number of stages in subsmapling
#define SFGD_SUBSAMPLING_NUMSTAGES 6

class SFDGSelector
{
public:
    Mat img_input;
    Mat img_fg;
    Mat img_bgmodel;

    int int_SFGDid;
    int int_FROM_BGSid;
    double framerate;
    int time2static;
    double alpha = 0.001;//(log(1-Cf)/log(1-alpha))=(log(0.9)/(1-alpha))==#frames para absorcion
    StaticMaskExtractor_subsampling *psubsamplingSFGD;
    StaticMaskExtractor_AccMask *pAccMaskSFGD;
    StaticMaskExtractor_DualBkg *pDBM;
    StaticMaskExtractor_HistoryImages *pHistoryImagesSFGD;


    SFDGSelector(int SFGDid, double framerate, int time_to_static);
    ~SFDGSelector();

    void init(Mat frame, BkgSubtractionSelector bgs);//, BkgSubtractionSelector bkg_selector);
    void process(Mat frame, std::vector<Mat> foreground_img, Mat bgmodel_img, settings Video);
    Mat GetStaticForeground();
    void finish(void);


};
