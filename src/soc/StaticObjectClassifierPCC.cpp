//file description
/**
 * \file StaticObjectClassifierPCC.cpp
 * \author Juan Carlos San Miguel Avedillo (jcs)
 * \date 27-10-17
 * \brief Implementation of StaticObjectClassifierPCC class
 * \version 1.0\n
 *			Version history:\n
 *				- 1.0(25-03-08): Initial Implementation (jcs)
 *				- 2.0(27-10-17): porting to OpenCV 3.2 (jcs)
 *
 *
 */

#include <opencv2/opencv.hpp>
#include "StaticObjectClassifierPCC.h"
#include <math.h>

using namespace std;
using namespace cv;
/**		 
 * Standard class Constructor
 *
 * \param _debug Flag to activate debug mode
 * \param _writelog Flag to write log file
 */
StaticObjectClassifierPCC::StaticObjectClassifierPCC(int debug, int writelog)
{
	need_to_init = true;

	this->_debug = debug;
	this->_writeLog = writelog;

	//this->tha = CONT_ABANDONED_TH_DEF; //th for abandoned object detection
    this->_tha = 0.001;
	//this->ths = CONT_STOLEN_TH_DEF; //th for stolen object detection
	this->_ths = 0.10;
	this->L = 5;
	this->M = 3;
	this->shift_pixels = 1;
	this->_incFactorBB = 2.0;
}

/**		 
 * Standard class Destructor		 
 */
StaticObjectClassifierPCC::~StaticObjectClassifierPCC()
{
	//delete resources
	//...

}

/**		 
 * Initialization resources
 *
 * /param sampleFrame Image to get properties of the data
 */
void StaticObjectClassifierPCC::init(cv::Mat sampleFrame)
{
	_incFactorBB = 2.0;
	need_to_init = false;
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
int StaticObjectClassifierPCC::checkObject(cv::Mat frame, cv::Mat bkgImage, cv::Mat staticObjMask, cv::Mat fgMask, ObjectBlob *object)
{
	if (need_to_init == true)
		init(frame);

	//get ROI for the object
	Rect bbox = cvRect((int)object->x, (int)object->y, (int)object->w, (int)object->h);
	Rect objROI = ampliarBB(bbox, _incFactorBB,frame);

	//extract & crop images
	Mat	croppedframe 	= frame(objROI); //crop frame data
	Mat	croppedbkg 		= bkgImage(objROI); //crop bkg data
	Mat	croppedsfgmask 	= staticObjMask(objROI); //crop mask data

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

	// Evaluate spatial color contrast for each pixel
	double valueBkg=0, valueImg=0;
	if (largest_contour_index != -1)
	{
		int nSamples = contours_sfgmask[largest_contour_index].size();//numbers of elements to process

		//compute the distance among all contour points
		Mat1f dist(nSamples, nSamples, 0.f);
		for (int p1 = 0; p1 < nSamples;p1++)
			for (int p2 = 0; p2 < nSamples;p2++)
				if(p1==p2)
					dist(p1,p2) = std::numeric_limits<float>::max(); //set the maximum value to avoid selecting the same point as min value
				else
				{
					int x1 = contours_sfgmask[largest_contour_index].at(p1).x;
					int y1 = contours_sfgmask[largest_contour_index].at(p1).y;
					int x2 = contours_sfgmask[largest_contour_index].at(p2).x;
					int y2 = contours_sfgmask[largest_contour_index].at(p2).y;
					dist(p1,p2) = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
				}

		//int count_abando = 0, count_stolen = 0; //counters

		//process each contour pixel
		double accPCCval_img=0,accPCCval_bkg=0;
		int count=0;
		for (int i = 0; i < nSamples;i++)
		{
			//coordinates
			int xx = contours_sfgmask[largest_contour_index].at(i).x;
			int yy = contours_sfgmask[largest_contour_index].at(i).y;

			//get the distances to 'ith' points from the previously computed distance
			cv::Mat dist_to_i = dist.row(i); //get data corresponding to 'ith' row

			//get the 1st closest point to the 'ith'
			double minVal=0,maxVal=0;
			cv::Point min_loc, max_loc;
			cv::minMaxLoc(dist_to_i, &minVal, &maxVal, &min_loc, &max_loc);
			int ind1 = min_loc.x; //index

			//get the 2nd closest point to the 'ith'
			dist_to_i.at<float>(min_loc.y, min_loc.x) = std::numeric_limits<float>::max(); //invalidate the first
			cv::minMaxLoc(dist_to_i, &minVal, &maxVal, &min_loc, &max_loc);
			int ind2 = min_loc.x; //index
			//std::cout << " dist_to_i=" << dist_to_i << std::endl;
			//std::cout << "Min value = " << dist_to_i.at<float>(min_loc.x, min_loc.y) << std::endl;

			//compute internal pI and external pO points for PCC
			cv::Point pI, pO;
			int x1 = contours_sfgmask[largest_contour_index].at(ind1).x; //coordinate X for p1 point
			int y1 = contours_sfgmask[largest_contour_index].at(ind1).y; //coordinate Y for p1 point
			int x2 = contours_sfgmask[largest_contour_index].at(ind2).x; //coordinate X for p2 point
			int y2 = contours_sfgmask[largest_contour_index].at(ind2).y; //coordinate Y for p2 point
			compute_sampling_points(cv::Point(xx,yy),cv::Point(x1,y1),cv::Point(x2,y2),&pI,&pO); //get the points

			//visualize the points
			//if(_debug)
			//	plot_sampling_points(cv::Point(xx,yy),cv::Point(x1,y1),cv::Point(x2,y2),pI,pO,&croppedframe);

			//extract patches for points pI/pO in the current frame/background
			cv::Mat IboxImg, OboxImg, IboxBkg, OboxBkg;

			int s = floor((double)(this->M-1)/2.0);
			int px1 = pI.x - s;
			int py1 = pI.y-s;
			int px2 = pO.x - s;
			int py2 = pO.y-s;

			//check boundaries
			if ((px1 >= 0) && (py1 >=0) && (px1+M <= croppedframe.cols) && (py1+M <= croppedframe.rows)
				&& (px2 >= 0) && (py2 >=0) && (px2+M <= croppedframe.cols) && (py2+M <= croppedframe.rows)) {
					IboxImg = extractImage(croppedframe, cvRect(pI.x - s, pI.y-s, M, M));
					OboxImg = extractImage(croppedframe, cvRect(pO.x - s, pO.y-s, M, M));
					IboxBkg = extractImage(croppedbkg, cvRect(pI.x - s, pI.y-s, M, M));
					OboxBkg = extractImage(croppedbkg, cvRect(pO.x - s, pO.y-s, M, M));

			}

			//compute distance in current and background images
			if(!IboxImg.empty() && !OboxImg.empty())
			{
				double c = 1.0/sqrt(3.0*255*255);
				cv::Scalar mI, mO;
				count = count +1;

				//compute statistic in the current image (spatial color contrast)
				mI = cv::mean(IboxImg);
				mO = cv::mean(OboxImg);
				accPCCval_img = accPCCval_img + c * sqrt( (mI.val[0]-mO.val[0])*(mI.val[0]-mO.val[0]) + (mI.val[1]-mO.val[1])*(mI.val[1]-mO.val[1]) + (mI.val[2]-mO.val[2])*(mI.val[2]-mO.val[2]));

				//compute statistic in the current image (spatial color contrast)
				mI = cv::mean(IboxBkg);
				mO = cv::mean(OboxBkg);
				accPCCval_bkg = accPCCval_bkg + c * sqrt( (mI.val[0]-mO.val[0])*(mI.val[0]-mO.val[0]) + (mI.val[1]-mO.val[1])*(mI.val[1]-mO.val[1]) + (mI.val[2]-mO.val[2])*(mI.val[2]-mO.val[2]));
			}

			//compute the average PCC value
			valueBkg = accPCCval_bkg / count;
			valueImg = accPCCval_img / count;
		}

		//classification between abandoned & stolen
		object->results->S_PCC = valueBkg - valueImg; //score obtained
		object->results->S_PCC = STATIC_OBJ_TYPE_UNKNOWN; //initial decision (unkown by default)

		if ((valueImg > _tha) || (valueBkg> _ths))
		{
			if (valueImg > valueBkg)
					object->results->D_F = STATIC_OBJ_TYPE_ABANDONED;
				else
					object->results->D_F = STATIC_OBJ_TYPE_STOLEN;
		}
	}
	if(_debug ==1)
		std::cout << "object id=" << object->ID << " (threshold="<<this->_tha<<")-> pABA(c="<<STATIC_OBJ_TYPE_ABANDONED<<")=" <<valueImg << " pSTO(c="<<STATIC_OBJ_TYPE_STOLEN<<")="<<valueBkg<< " -> decision c=" <<object->results->D_F <<std::endl;
	
	return object->results->D_F;
}

void StaticObjectClassifierPCC::compute_sampling_points(cv::Point pc,cv::Point p1, cv::Point p2, cv::Point *pI, cv::Point *pO)
{
	if(p1.x == p2.x)//vertical rect --> horizontal rect (orthogonal rect)
	{
		pI->x = pc.x-L;
		pI->y = pc.y;
		pO->x = pc.x+L;
		pO->y = pc.y;
	}

	if(p1.y == p2.y)//horizontal rect --> vertical rect (orthogonal rect)
	{
		pI->x = pc.x;
		pI->y = pc.y-L;
		pO->x = pc.x;
		pO->y = pc.y+L;
	}

	if(p1.x != p2.x && p1.y != p2.y) //arbitrary rect --> compute the orthogonal rect
	{
		//rect that contains p1 and p2 -- y=p(x-x0)+y0
		double p  = (double)(p2.y - p1.y)/(double)(p2.x-p1.x);
		double y0  = (double)pc.y;
		double x0 = (double)pc.x;

		//normal rect --> y=-1/a*(x-x0)+y0
		//...

		//find points at distance L of the point pc
		//Euclidean distance d = sqrt((x1-x2)^2+(y1-y2)^2)
		// Assuming that x2 = (x0,y0) and clearing all the variables,
		// we have the following second-order equation:
		// x1^2-2x0x1+x0^2+d^2/(1+1/p^2)
		// we solve this equation using the general method
		// x1 = 1/2a * (-b +- sqrt(b^2 - 4ac))
		double a = 1;
		double b = -2*x0;
		double c = x0*x0 - (L*L)/(1+1/(p*p));
		double rI = 1/(2*a) * (-b + sqrt(b*b - 4*a*c));
		double rO = 1/(2*a) * (-b - sqrt(b*b - 4*a*c));

		rI>pc.x?pI->x=ceil(rI):pI->x=floor(rI);
		rO>pc.x?pO->x=ceil(rO):pO->x=floor(rO);

		pI->y = -1.0/p * (pI->x-pc.x) + y0;
		pO->y = -1.0/p * (pO->x-pc.x) + y0;
	}
}

void StaticObjectClassifierPCC::plot_sampling_points(cv::Point pc,cv::Point p1, cv::Point p2, cv::Point pI, cv::Point pO,cv::Mat *frame)
{
	//Calculate window around initial and end point of the normal rect
	int s = floor((double)(this->M-1)/2.0);

	cv::Mat frameaux;
	frame->copyTo(frameaux);

	if(_debug == 1)
	{
		namedWindow("Points/windows",0);

		circle(frameaux, pI, 1, cv::Scalar(255,0,0)); 	imshow("Points/windows", frameaux);	cv::waitKey(0);
		circle(frameaux, pO, 1, cv::Scalar(0,255,0));	imshow("Points/windows", frameaux);	cv::waitKey(0);
		circle(frameaux, pc, 1, cv::Scalar(0,0,255));	imshow("Points/windows", frameaux);	cv::waitKey(0);

		rectangle(frameaux, cv::Point(pI.x - s, pI.y - s), cv::Point(pI.x + s, pI.y + s), cv::Scalar(128),2);
		rectangle(frameaux, cv::Point(pO.x - s, pO.y - s), cv::Point(pO.x + s, pO.y + s), cv::Scalar(128),2);
		imshow("Points/windows", frameaux);
		cv::waitKey(0);
	}
}


/**		 
 * Computes the euclidean distance between two contours
 * \param contour1 Sequence of points representing the first contour
 * \param contour2 Sequence of points representing the second contour
 * \param numPoints number of points to check
 *
 * \return Similarity value (between 0 and 1)
 */
double StaticObjectClassifierPCC::EuclideanDistance(std::vector<cv::Point> *contour1, std::vector<cv::Point> *contour2, int numPoints)
{
	/*double distance=0.0;

	for(int i=0;i<numPoints;i++)
	{
		CvPoint *p1 = (CvPoint*)cvGetSeqElem( contour1, i );
		CvPoint *p2 = (CvPoint*)cvGetSeqElem( contour2, i );

		double val1 =pow((double)(p1->x - p2->x),2);
		double val2 =pow((double)(p1->y - p2->y),2);
		distance+=sqrt( val1 + val2);
	}

	return distance/numPoints;
	*/
	return 1.0;
}

/**		 
 * Shape matching technique based on histogram comparison
 * \param shape1 Sequence of points representing the first contour
 * \param shape2 Sequence of points representing the second contour
 *
 * \return Similarity value (between 0 and 1)
 */
/*double StaticObjectClassifierPCC::pghMatchShapes(CvSeq *shape1, CvSeq *shape2) {
	int dims[] = {128,128};
        float range[] = {-180, 180, -100, 100};
        float *ranges[] = {&range[0], &range[2]};
    CvHistogram* hist1 = cvCreateHist(2, dims, CV_HIST_ARRAY, ranges, 1);
    CvHistogram* hist2 = cvCreateHist(2, dims, CV_HIST_ARRAY, ranges, 1);
    cvCalcPGH(shape1, hist1);
    cvCalcPGH(shape2, hist2);
    cvNormalizeHist(hist1, 100.0f);
    cvNormalizeHist(hist2, 100.0f);
    double corr = cvCompareHist(hist1, hist2, CV_COMP_BHATTACHARYYA);
    cvReleaseHist(&hist1);
    cvReleaseHist(&hist2);
        return corr;
}/**/

