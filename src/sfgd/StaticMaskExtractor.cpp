/**
* \file StaticMaskExtractor.cpp
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 16-12-10
* \brief Implementation of the common interface for the static region detection algorithms
* \version 1.0\n
*			Version history:\n
*				- 1.0 (01-09-09): Initial Implementation (jcs)
*				- 1.1 (16-12-10): Modification for being a common interface (jcs)
*/

#include "StaticMaskExtractor.h"
/**
*	Class Constructor with initial data
*
*	\param sampleFrame Sample of images to process in the future
*	\param log Pointer to log file
*
*/

using namespace cv;

StaticMaskExtractor::StaticMaskExtractor(Mat sampleFrame, double framerate, double _secs2static)
{
    Mat img;
    sampleFrame.copyTo(img);

	this->secs2static = _secs2static;
	this->framerate = framerate;

    nChannels = img.channels();
    elemType = img.type();

    this->staticMask = Mat::zeros(img.rows,img.cols,CV_8UC1);

	this->counter = 0;
}



/**
*	Default class destructor
*
*/


StaticMaskExtractor::~StaticMaskExtractor(void)
{
    if (!staticMask.empty())
        staticMask.release();
}


/**
*	Interface to compute the stationary region mask.
*	This method has to be implemented in derived classes
*
*	\param frame Current image (Color or gray)
*	\param fg_mask Mask indicating the presence of moving objects (Gray)
*
*/
void StaticMaskExtractor::processFrame(Mat frame, Mat fg_mask)
{
}

/**
*	Method to get the static Foreground mask
*
*	\returns An IplImage containing the motion mask (1 channel and IPL_DEPTH_8U)
*
*/
Mat StaticMaskExtractor::getStaticMask()
{
	return this->staticMask;
}

/**
*	Gets the number of frames processed
*	\return the current number of frames processed
*
*/
int StaticMaskExtractor::getNumFramesProcessed()
{
	return this->counter;
}


/**
*	Method to set the time to detect an object as stationary
*
*	\param _time New value
*
*/
void StaticMaskExtractor::setTime(double _time)
{
	this->secs2static = _time;
	this->frames2static = (int)(secs2static*framerate);
}
/**
*	Method to set the framerate of the input video
*
*	\param _framerate New value
*
*/
void StaticMaskExtractor::setFramerate(double _framerate)
{
	this->framerate = _framerate;
	this->frames2static = (int)(secs2static*framerate);
}

/**
*	Method to get the time to detect an object as stationary
*
*	\return currently used value
*
*/
double StaticMaskExtractor::getTime()
{
	return this->secs2static;
}

/**
*	Method to get the framerate of the input video
*
*	\return currently used value
*
*/
double StaticMaskExtractor::getFramerate()
{
	return this->framerate;
}
