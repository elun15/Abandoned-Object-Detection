//file description
/**
 * \file StaticObjectClassifierPixelColourContrast.cpp 
 * \author Juan Carlos San Miguel Avedillo (jcs)
 * \date 01-12-10	
 * \brief Implementation of StaticObjectClassifierPixelColourContrast class (colour difference for pixels along the contour of the object being analyzed)
 * \version 2.0\n
 *			Version history:\n
 *				- 1.0(01-12-10): Initial Implementation (jcs)
 *				- 2.0(27-10-17): porting to OpenCV 3.2 (jcs)
 *
 *
 */
#ifndef _STATICOBJECTCLASSIFIERPIXELCOLOURCONTRAST_INCLUDED_
#define _STATICOBJECTCLASSIFIERPIXELCOLOURCONTRAST_INCLUDED_

#include "StaticObjectClassifier.h"

///Size of the window in the contour matching procedure
const int WINDOW_SIZE_ = 2;

//class description
/**
 * \class StaticObjectClassifierPixelColourContrast
 * \brief Class to determine the state of a stationary blob using its contour
 * 
 */
class StaticObjectClassifierPCC : public StaticObjectClassifier
{
public:

	///threshold for abandoned object detection
	double _tha;

	///threshold for stolen object detection
	double _ths;

	bool need_to_init;

	/// Pixel distance of the patches to analyze (considering the normal rect with respect to the pixel under analysis)
	int L;

	/// Size of the patch to analyze
	int M;

	/// number of pixels to jump (for faster analysis)
	int shift_pixels;
	
public:

	//Standard class Constructor		 
	StaticObjectClassifierPCC(int debug=DEBUG_DEF, int writelog=WRITE_LOG_DEF);
	
	//Standard class Destructor		 	 
	~StaticObjectClassifierPCC();

	//Inizialization method	 
	void init(cv::Mat sampleFrame);

	// Method to check each object
	int checkObject(cv::Mat frame, cv::Mat bkgImage, cv::Mat staticObjMask, cv::Mat fgMask, ObjectBlob* object);

private:
	void compute_sampling_points(cv::Point pc,cv::Point p1, cv::Point p2, cv::Point *pI, cv::Point *pO);
	void plot_sampling_points(cv::Point pc,cv::Point p1, cv::Point p2, cv::Point pI, cv::Point pO,cv::Mat *frame);


	//Method to calculate the euclidean distance between two contours
	double EuclideanDistance(std::vector<cv::Point> *contour1, std::vector<cv::Point> *contour2, int numPoints);

	//Method to get the closest points in a contour with respect to a certain point p0
	//void getClosestPoints(cv::Point p0, std::vector<cv::Point> *contour, cv::Point *p1, cv::Point *p2);
};

#endif
