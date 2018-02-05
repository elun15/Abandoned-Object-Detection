
/**
* \file StaticMaskExtractor_AccMask.cpp
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 16-12-10
* \brief Implementation of the common interface for the static region detection algorithms
* \version 1.0\n
*			Version history:\n
*				- 1.1 (01-09-09): Initial Implementation (jcs)
*				- 1.2 (16-12-10): Modification for being a common interface (jcs)
*				- 1.3 (07-12-12): El algoritmo lo he cambiado pues el anterior no funcionaba bien,
*								  ahora la imagen S se incrementa de 1 en 1  si es FG y se decrementa
*                                de 2 en 2 si es BG. Hay un umbral, que en este caso esta a 240, que
*                                cuando se supera significa que el pixel es estatico y la mascara
*                                estatica se pone a 1. (doh)
* \version 2.0\n
*			Version history:\n
*				- 2.0(13/09/17): reimplemented and adapted to openCV 3.2 (elg)
*
*/
#include "StaticMaskExtractor_AccMask.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

#define C_COST 1 //increment cost for stationary detection
#define D_COST 10 //penalization cost for stationary detection


/**
*	Class Constructor with initial data
*
*	\param sampleFrame Sample of images to process in the future
*	\param log Pointer to log file
*	\param _framerate Framerate of the input video frame
*	\param _wd Weight decay for penalizing the no-motion time intervals (default 2.0)
*	\param _conf_th Confidence threshold for detecting a stationary object (default 250)
*
*/
StaticMaskExtractor_AccMask::StaticMaskExtractor_AccMask(Mat sampleFrame, double _framerate, double time_to_static, double _wd, int _conf_th) :StaticMaskExtractor(sampleFrame, _framerate, time_to_static)
{
    this->wd = _wd; //Weight to penalty non-motion detection
    this->confThreshold = _conf_th;	//Threshold to consider as stationary mask (range 0---1 is mapped to 0...255)
    this->Acc_Cont = 1;
    this->Acc_Dec= 250;
    this->static_th = 0.85 * 250;
    this->adapt = (255 / (framerate*secs2static));
    this->FG_Dec = 3 * this->adapt;

   this->fgmask_counter= Mat::zeros(sampleFrame.rows,sampleFrame.cols,CV_32FC1);
}


/**
*	Default destructor
*
*/
StaticMaskExtractor_AccMask::~StaticMaskExtractor_AccMask()
{

    if (!fgmask_counter.empty())
        fgmask_counter.release();

}


/**
*	Method to accumulate FG masks (currently is based on a simple counter).
*
*	\param frame Current image (Color or gray)
*	\param fg_mask Mask indicating the presence of moving objects (Gray)
*
*/
void StaticMaskExtractor_AccMask::processFrame(Mat fgmask,double framerate, int time_to_static)
{
    this->counter++;

    //check input validity and return -1 if any is not valid
    Mat bgmask;

    Mat shad_mask;

    Mat negative;
    // Delete shadows of the mask

    compare(fgmask, 127, shad_mask, CMP_EQ);
    fgmask.setTo(0, shad_mask);

    //num frames to static
    int numframes2static = (int)(framerate * time_to_static);

    //Invert fgmask -- 255 if BG pixel and 0 if FG
    bitwise_not(fgmask, bgmask);

    //operate with fgmask to update fgmask_counter
    double a = C_COST;//*adapt;
    add(fgmask_counter, (float)a, fgmask_counter, fgmask); // Add C_COST if FG pixel

    double b = D_COST;//*adapt;
    subtract(fgmask_counter, (float)b, fgmask_counter, bgmask);

    compare(fgmask_counter, 0.0, negative, CMP_LT);
    fgmask_counter.setTo(0.0, negative);

    //operate with fgmask_counter to update sfgmask
    compare(fgmask_counter, (float)numframes2static, this->staticMask, CMP_GE);



}


/**
*	Method to set the weight decay (penalty for no-motion)
*
*	\params _wd The new value
*
*/
void StaticMaskExtractor_AccMask::setWeightDecay(double _wd)
{
    this->wd = _wd;
}

/**
*	Method to get the weight decay (penalty for no-motion)
*
*	\returns the current value
*
*/
double StaticMaskExtractor_AccMask::getWeightDecay()
{
    return this->wd;
}

/**
*	Method to set the confidence threshold
*
*	\params the new value
*
*/
void StaticMaskExtractor_AccMask::setConfidenceThreshold(int _th)
{
    this->confThreshold = _th;
}

/**
*	Method to get the confidence threshold
*
*	\returns the current value
*
*/
int StaticMaskExtractor_AccMask::getConfidenceThreshold()
{
    return confThreshold;
}

Mat StaticMaskExtractor_AccMask::getConfidenceImage()
{
    return fgmask_counter;
}
