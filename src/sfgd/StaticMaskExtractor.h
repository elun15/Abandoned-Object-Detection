/**
* \file StaticMaskExtractor.h
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 16-12-10
* \brief Common interface for the static region detection algorithms
* \version 2.0\n
*			Version history:\n
*				- 1.0 (01-09-09): Initial Implementation (jcs)
*				- 2.0 (16-12-10): Modification for being a common interface (jcs)
*
*/

//class description
/**
* \class StaticMaskExtractor
* \brief Class to describe a common interface for the static region detection algorithms
*
*/

#if !defined(STATICMASKEXTRACTOR_H_INCLUDED_)
#define STATICMASKEXTRACTOR_H_INCLUDED_
#include <vector>
#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>

/**
*	\def FRAMERATE_DEF
*	default Default framerate of the input video
*/
///	Default Default framerate of the input video
const double FRAMERATE_DEF = 25;

/**
*	\def TIME_STATIC_DEF
*	default Default time to detect an object as stationary
*/
///	Default Default time to detect an object as stationary
const double TIME_STATIC_DEF = 10;

//class definition
class StaticMaskExtractor
{
public:

	// Default constructor
    StaticMaskExtractor(cv::Mat sampleFrame, double framerate = FRAMERATE_DEF, double _time_static = TIME_STATIC_DEF);

	// Default destructor
    virtual ~StaticMaskExtractor();

	//Method to process each frame

    virtual void processFrame(cv::Mat frame,  cv::Mat mask);

    //Method to get the static motion mask
    cv::Mat getStaticMask();
	//Get the number of frames processed
	int getNumFramesProcessed();

	//Method to set the time to detect an object as stationary
	void setTime(double _time);

	//Method to set the framerate of the input video
	void setFramerate(double _framerate);

	//Method to get the time to detect an object as stationary
	double getTime();

	//Method to get the framerate of the input video
	double getFramerate();

protected:

	/// Counter of images processed
	int counter;

	/// Size of the input images
	cv::Size arrSize;

	/// Number of channels of the input images
	int nChannels;

	/// Type of pixels of the input images (IPL_DEPTH_8U or IPL_DEPTH_32F)
	int elemType;

	/// Pointer to log file
	FILE *_log;

	/// mask of stationary regions
	cv::Mat staticMask;

	///framerate of the input video
	double framerate;

	///Time to detect a stationary region
	double secs2static;

	///Frames to detect a stationary region
	int frames2static;
};

#endif // !defined(AFX_STATICMASKEXTRACTOR_H_INCLUDED_)
