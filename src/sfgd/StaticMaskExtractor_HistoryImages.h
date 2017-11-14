/**
* \file StaticMaskExtractor_HistoryImages.h
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
* \class StaticMaskExtractor_HistoryImages
* \brief Class to describe the static region detection algorithm based on FG mask subsampling
*
*/

#if !defined(STATICMASKEXTRACTOR_HISTORYIMAGES_H)
#define STATICMASKEXTRACTOR_HISTORYIMAGES_H
// LOS PONGO EN COMENTARIO, AHORA NO SE USAN
//#include "cv.h"
//#include "BlobList.h"
//#include "BasicBlob.h"
//#include "BlobExtractor.h"

#include <opencv2/opencv.hpp>
#include "StaticMaskExtractor.h"
//#include "../../../src/settings.h"

/**
*	\def CONFIDENCE_THRESHOLD
*	Threshold to consider as stationary mask (range 0---1 is mapped to 0...255)
*/
///	Threshold to consider as stationary mask (range 0---1 is mapped to 0...255)
const int CONF_TH_ACC2 = 250;

/**
*	\def WEIGTH_DECAY_FOR_NO_MOTION
*	Weight to penalty non-motion detection
*/
///	Weight to penalty non-motion detection
const double WEIGTH_DECAY_FOR_NO_MOTION_ACC2 = 2.0f;

using namespace cv;
using namespace std;

//class definition
class StaticMaskExtractor_HistoryImages : public StaticMaskExtractor
{
public:

	// Default constructor
    StaticMaskExtractor_HistoryImages(Mat sampleFrame, double _framerate = FRAMERATE_DEF, double time_to_static = TIME_STATIC_DEF, double _wd = WEIGTH_DECAY_FOR_NO_MOTION_ACC2, int _conf_th = CONF_TH_ACC2);

	// Default destructor
    virtual ~StaticMaskExtractor_HistoryImages();

	//Method to process each frame
    void processFrame(Mat fgmask, Mat srcMat, Mat bkgMat, double framerate, int time_to_static);


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

    cv::Mat fgmask_counter;
    cv::Mat motion_counter;
    cv::Mat ssim_counter;
    //cv::Mat StaticFGmat;
    int windowSize;
    vector<cv::Mat> Frames4median;



};

#endif // !defined(AFX_STATICMASKEXTRACTOR_ACC_H_INCLUDED_)
