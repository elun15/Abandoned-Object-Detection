/**
* \file StaticMaskExtractor_AccMask.h
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 16-12-10
* \brief Interface of the static detection method described in
S. Guler and M.k.k Farrow, "Abandoned Object Detection in Corwded Places",
IEEE International Workshop on PETS, New York, June 18, 2006
* \version 2.0\n
*			Version history:\n
*				- 1.0 (30-03-09):Initial Implementation (jcs)
*				- 2.0 (16-12-10) Introduction in the hierarchy of StaticMaskExtractor
*
*/

//class description
/**
* \class StaticMaskExtractor_AccMask
* \brief Class to describe the static region detection algorithm based on FG mask subsampling
*
*/

#if !defined(AFX_STATICMASKEXTRACTOR_ACC_H_INCLUDED_)
#define AFX_STATICMASKEXTRACTOR_ACC_H_INCLUDED_

// LOS PONGO EN COMENTARIO, AHORA NO SE USAN
//#include "cv.h"
//#include "BlobList.h"
//#include "BasicBlob.h"
//#include "BlobExtractor.h"
#include "StaticMaskExtractor.h"
#include <opencv2/opencv.hpp>
#include "../../../src/settings.h"

/**
*	\def CONFIDENCE_THRESHOLD
*	Threshold to consider as stationary mask (range 0---1 is mapped to 0...255)
*/
///	Threshold to consider as stationary mask (range 0---1 is mapped to 0...255)
const int CONF_TH_ACC = 250;

/**
*	\def WEIGTH_DECAY_FOR_NO_MOTION
*	Weight to penalty non-motion detection
*/
///	Weight to penalty non-motion detection
const double WEIGTH_DECAY_FOR_NO_MOTION_ACC = 2.0f;

//class definition
class StaticMaskExtractor_AccMask : public StaticMaskExtractor
{
public:

	// Default constructor
    StaticMaskExtractor_AccMask(Mat sampleFrame, double _framerate = FRAMERATE_DEF, double time_to_static = TIME_STATIC_DEF, double _wd = WEIGTH_DECAY_FOR_NO_MOTION_ACC, int _conf_th = CONF_TH_ACC);

	// Default destructor
	virtual ~StaticMaskExtractor_AccMask();

	//Method to process each frame
    void processFrame(Mat fgmask, double framerate, int time_to_static);


	void setWeightDecay(double _wd);
	double getWeightDecay();
	void setConfidenceThreshold(int _th);
	int getConfidenceThreshold();
    Mat getConfidenceImage();

private:

	///positive number that generates a fraction of time t for declaring a pixel non-extationary		
	double r;

	///Weight decay for penalizing no-motion
	double wd;

	///Confidence threshold for stationary blob detection
	int confThreshold;

	double Acc_Cont;
	double Acc_Dec;
	double FG_Dec;
	double adapt;
	double static_th;

    Mat fgmask_counter;



};

#endif // !defined(AFX_STATICMASKEXTRACTOR_ACC_H_INCLUDED_)
