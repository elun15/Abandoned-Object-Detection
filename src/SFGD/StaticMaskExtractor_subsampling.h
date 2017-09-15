/**
* \file StaticMaskExtractor_subsampling.h
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 16-12-10
* \brief Interface of the static region detection algorithm
Liao,H-H.; Chang,J-Y.; Chen, L-G. 'A localized Approach to abandoned luggage detection with Foreground Mask sampling',
Proc. of AVSS 2008, pp. 132-139.
* \version 1.0\n
*			Version history:\n
*				- 1.0 (16-12-10): Initial Implementation (jcs)
*
*/

//class description
/**
* \class StaticMaskExtractor_subsampling
* \brief Class to describe the static region detection algorithm based on FG mask subsampling
*
*/

#if !defined(AFX_STATICMASKEXTRACTOR_SUBSAMPLING_H_INCLUDED_)
#define AFX_STATICMASKEXTRACTOR_SUBSAMPLING_H_INCLUDED_

#include <opencv2/opencv.hpp>
// LO PONGO EN COMENTARIO NO LO USO AHORA
//#include "cv.h"
//#include "BlobList.h"
//#include "BasicBlob.h"
//#include "BlobExtractor.h"
#include "StaticMaskExtractor.h"

/**
*	\def STAGES_DEF
*	default Default number of processing stages
*/
///	Default Default number of processing stages
const int STAGES_DEF = 6;

/**
*	\def MAX_STAGES
*	default Maximum number of processing stages
*/
///	Default Maximum number of processing stages
const int MAX_STAGES = 260;

using namespace cv;
//class definition
class StaticMaskExtractor_subsampling : public StaticMaskExtractor
{
public:

	// Default constructor
    StaticMaskExtractor_subsampling( Mat sampleFrame, double framerate = FRAMERATE_DEF, double time_to_static = TIME_STATIC_DEF, int num_stages = STAGES_DEF);

	// Default destructor
    virtual ~StaticMaskExtractor_subsampling();


    //Method to process each frame
    void processFrame(Mat frame, Mat mask);

	//Method to get the static motion mask
	//IplImage *getStaticMask();

	//Method to get the subsampled mask
     Mat getSampledMask(int n);

	//Method to set the time to detect an object as stationary
	void setTime(double _time);

	//Method to set the framerate of the input video
	void setFramerate(double _framerate);

	//Method to set the number of stages of the algorithm
	void setNumStages(int _NumStages);

	//Method to get the number of stages of the algorithm
	int getNumStages();


private:

	/// Auxliary masks for the subsampling analysis
    vector<Mat> pSampledMask;
	///Number of selected processing stages
	int num_stages;

	///Time limit to detect an object as stationary
	//double secs;//already defined in StaticMaskExtractor class

	///Framerate of the input video
	//double framerate;//already defined in StaticMaskExtractor class

	///Number of frames to sample
	int numFramesToSample;

	/// Current stage sampled
	int stage_sampled;
	// Pointer to log file
	//FILE *_log;(already defined in StaticMaskExtractor class)

	// mask of stationary regions
	//IplImage *staticMask; (already defined in StaticMaskExtractor class)
};

#endif // !defined(AFX_STATICMASKEXTRACTOR_SUBSAMPLING_H_INCLUDED_)
