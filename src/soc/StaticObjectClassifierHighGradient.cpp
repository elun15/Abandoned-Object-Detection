//file description
/**
* \file StaticObjectClassifierHist.cpp
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 25-03-08
* \brief Implementation of StaticObjectClassifierHist class
* \version 1.0\n
*			Version history:\n
*				- 1.0(25-03-08): Initial Implementation (jcs)
*
*/

#include <opencv2/opencv.hpp> //opencv library

#include "./../../src/blobs/BasicBlob.h"
#include "StaticObjectClassifierHighGradient.h"

using namespace std;
using namespace cv;

//Constructor
StaticObjectClassifierHighGradient::StaticObjectClassifierHighGradient()
{
    _margin = 3;
    _minThreshold = 5; //min percentage of pixel decisions
}


StaticObjectClassifierHighGradient::~StaticObjectClassifierHighGradient()
{

}

/**
* Classifies a static object
* \param frame current frame
* \param bkgImage background model image
* \param staticObjMask static objects mask
* \param fgMask foreground/background mask
* \param object static object to check
* \return Returns a DECISION about the object analyzed
*/
int StaticObjectClassifierHighGradient::checkObject(Mat frame, Mat bkgImage, Mat staticObjMask, Mat fgMask, ObjectBlob* object)
{
	//compute ROI of object
	CvRect bbox = cvRect((int)object->x, (int)object->y, (int)object->w, (int)object->h);
	CvRect objROI = ampliarBB(bbox, _incFactorBB,frame);

	//extract & crop images
	cv::Mat	croppedframe 	= frame(objROI); //crop frame data
	cv::Mat	croppedbkg 		= bkgImage(objROI); //crop bkg data
	cv::Mat	croppedsfgmask 	= staticObjMask(objROI); //crop mask data

	//find contours of object in static mask
	std::vector<std::vector<Point>> contours_sfgmask;
	findContours(croppedsfgmask, contours_sfgmask, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	// Extract the largest contour
	double largest_area = 0;
	int largest_contour_index = -1;
	for (int i = 0; i < (int)contours_sfgmask.size(); i++) 		 // Iterate through each contour.
	{
		double a = contourArea(contours_sfgmask[i], false);  // Find the area of contour
		if (a > largest_area) {
			largest_area = a;
			largest_contour_index = i;                		 // Store the index of largest contour
		}
	}

	//process largest contour (if any)
	double prob_abando_percentage=0,prob_stolen_percentage=0;
	if (largest_contour_index != -1)
	{
		//compute edges for each cropped data
		cv::Mat frame_edges, bkg_edges;
		Canny(croppedframe, frame_edges, 100, 300);
		Canny(croppedbkg, bkg_edges, 100, 300);

		//cout << "i max " <<contours_sfgmask[largest_contour_index].size() << endl;
		//analyze each pixel of the contour
		int nContourSamples = contours_sfgmask[largest_contour_index].size();//numbers of elements to process
		int count_abando = 0, count_stolen = 0; //counters
		for (int i = 0; i < nContourSamples;i++)
		{
			//coordinates
			int xx = contours_sfgmask[largest_contour_index].at(i).x;
			int yy = contours_sfgmask[largest_contour_index].at(i).y;

			// Boundaries check (point location)
			int x_n1 = (xx - 1 < 0) ? 0 : xx - 1;
			int y_n1 = (yy - 1 < 0) ? 0 : yy - 1;

			// Boundaries check (margin around point)
			int w = (x_n1 + _margin > croppedframe.cols) ? (x_n1 + _margin) - croppedframe.cols: _margin;
			if (w < 0) w = 0;

			int h = (y_n1 + _margin > croppedframe.rows) ? (y_n1 + _margin) - croppedframe.rows: _margin;
			if (h < 0) h = 0;

			//create new ROI after check
			CvRect objROI2(x_n1, y_n1, w, h);

			//get cropped versions of edge data
			cv::Mat frame_aux 	= frame_edges(objROI2);
			cv::Mat bkg_aux 	= bkg_edges(objROI2);

			//identify where edges are present
			int non_zero_frame = countNonZero(frame_aux); //edges in frame
			int non_zero_bkg = countNonZero(bkg_aux);	//edges in background

			//increase counters for abandoned or stolen
			if (non_zero_frame > non_zero_bkg)
				count_abando = count_abando + 1;
			else if (non_zero_bkg > non_zero_frame)
				count_stolen = count_stolen + 1;
		}
		//compute percentage for abandoned & stolen
		prob_abando_percentage = 100*count_abando/nContourSamples;
		prob_stolen_percentage = 100*count_stolen/nContourSamples;
	}

	//take decision for abandoned or stolen
	object->results->D_F = STATIC_OBJ_TYPE_UNKNOWN;//initial decision (unkown by default)

	if ((prob_abando_percentage > _minThreshold) || (prob_stolen_percentage> _minThreshold))
	{
		if (prob_abando_percentage > prob_stolen_percentage)
				object->results->D_F = STATIC_OBJ_TYPE_ABANDONED;
			else
				object->results->D_F = STATIC_OBJ_TYPE_STOLEN;
	}

	if (_debug)
		std::cout << "object id=" << object->ID << " (threshold="<<this->_minThreshold<<")-> pABA(c="<<STATIC_OBJ_TYPE_ABANDONED<<")=" <<prob_abando_percentage << " pSTO(c="<<STATIC_OBJ_TYPE_STOLEN<<")="<<prob_stolen_percentage<< " -> decision c=" <<object->results->D_F <<std::endl;

    return object->results->D_F;
}
