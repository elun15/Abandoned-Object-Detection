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

#include "../../../src/CLASSIFIER/StaticObjectClassifierHighGradient.h"
#include "../../../src/CLASSIFIER/StaticObjectClassifier.h"
#include <opencv2/opencv.hpp>
//#include "highgui.h"
using namespace std;
using namespace cv;

#include "../../../src/BLOBS/BasicBlob.h"

//Constructor
StaticObjectClassifierHighGradient::StaticObjectClassifierHighGradient()
{

    margin = 10;

}


StaticObjectClassifierHighGradient::~StaticObjectClassifierHighGradient(){}



/**
 * Blob classification into abandoned and stolen objects. All the input arguments
 * must be initialized when using this function.
 *
 * \param frame current frame of the video sequence (1 or 3-channel 8-bit)
 * \param bkg current background of the video sequence (1 or 3-channel 8-bit)
 * \param sfgmask Foreground/Background segmentation mask (1-channel binary
                  image) with the stationary pixels
 * \param sbloblist List with stationary blobs (object)
 *
 * \return Operation code (negative if not successful operation)
 */
int StaticObjectClassifierHighGradient::classifyBlobStationary(Mat frame, Mat bkg, Mat sfgmask, BlobList<ObjectBlob*>* objectList)
{



    cv::Mat croppedImage, croppedframe, croppedbkg, croppedsfgmask;
    std::vector<std::vector<Point>> contours_frame, contours_bck, contours_sfgmask;
    cv::Mat frame_edges, bck_edges;

    for (int j = 0; j <  objectList->getBlobNum(); j++) {

        int prob_abandon = 0, prob_stolen = 0;
        int decision = STATIC_OBJ_TYPE_UNKNOWN;
        // Setup a rectangle to define your region of interest
        // Boundaries check
        //cv::Rect myROI;
        // myROI = check_size(frame, sbloblist, margin, j);

        ObjectBlob *obj;
        obj = objectList->getBlob(j);

        CvRect myROI = cvRect((int)obj->x, (int)obj->y, (int)obj->w, (int)obj->h);


        // Crop the same region in the frame, background and sfgmask
        croppedImage = frame(myROI);
        croppedImage.copyTo(croppedframe);
        //		imshow("frame crop", croppedframe);

        croppedImage = bkg(myROI);
        croppedImage.copyTo(croppedbkg);

        croppedImage = sfgmask(myROI);
        croppedImage.copyTo(croppedsfgmask);

        findContours(croppedsfgmask, contours_sfgmask, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        // Extract the larger contour
        double largest_area = 0;
        int largest_contour_index = -1;
        for (int i = 0; i < contours_sfgmask.size(); i++) // iterate through each contour.
        {
            double a = contourArea(contours_sfgmask[i], false);  //  Find the area of contour
            if (a > largest_area) {
                largest_area = a;
                largest_contour_index = i;                //Store the index of largest contour
            }

        }
        Mat aux, aux2;
        if (largest_contour_index != -1){

            Canny(croppedframe, frame_edges, 100, 300);
            Canny(croppedbkg, bck_edges, 100, 300);
           

            int xx, yy;
            int x_n1, w, y_n1, h;
            int non_zero_frame, non_zero_bkg;
            //cout << "i max " <<contours_sfgmask[largest_contour_index].size() << endl;

            for (int i = 0; i < contours_sfgmask[largest_contour_index].size();i++) {


                xx = contours_sfgmask[largest_contour_index].at(i).x;
                yy = contours_sfgmask[largest_contour_index].at(i).y;

                // Boundaries check
                if (xx - 1 < 0) {
                    x_n1 = 0;
                }
                else {
                    x_n1 = xx - 1;
                }
                if (yy - 1 < 0) {
                    y_n1 = 0;
                }
                else {
                    y_n1 = yy - 1;
                }

                if (x_n1 + 3 > croppedframe.cols) {
                    w = (x_n1 + 3) - croppedframe.cols;
                    if (w < 0)
                        w = 0;
                }
                else {
                    w = 3;
                }
                if (y_n1 + 3 > croppedframe.rows) {
                    h = (y_n1 + 3) - croppedframe.rows;
                    if (h < 0)
                        h = 0;
                }
                else {
                    h = 3;
                }
                cv::Rect myROI2(x_n1, y_n1, w, h);

                aux = frame_edges(myROI2);
                aux.copyTo(aux);
                aux2 = bck_edges(myROI2);
                aux2.copyTo(aux2);


                non_zero_frame = countNonZero(aux);
                non_zero_bkg = countNonZero(aux2);
                if (non_zero_frame > non_zero_bkg)
                    prob_abandon = prob_abandon + 1;
                else if (non_zero_bkg > non_zero_frame)
                    prob_stolen = prob_stolen + 1;

                non_zero_frame = 0;
                non_zero_bkg = 0;


            }
        }




        if (largest_contour_index != -1){
            double prob_abandon_percentage = 100*prob_abandon/contours_sfgmask[largest_contour_index].size();
            double prob_stolen_percentage = 100*prob_stolen/contours_sfgmask[largest_contour_index].size();
         

            if (prob_abandon_percentage < 15 && prob_stolen_percentage < 15) //20%
            {
                obj->results->D_F = STATIC_OBJ_TYPE_LIGHT_CHANGE; //Color Hist decision
                decision = STATIC_OBJ_TYPE_LIGHT_CHANGE;
            }
            else
            {

                if (prob_abandon_percentage > prob_stolen_percentage)
                {
                    obj->results->D_F =  STATIC_OBJ_TYPE_ABANDONED;
                    decision = STATIC_OBJ_TYPE_ABANDONED;}
               
                else {
                    obj->results->D_F = STATIC_OBJ_TYPE_STOLEN;
                    decision = STATIC_OBJ_TYPE_STOLEN;
                }

            }

        }else
        {

            obj->results->D_F = STATIC_OBJ_TYPE_LIGHT_CHANGE; //Color Hist decision
            decision = STATIC_OBJ_TYPE_LIGHT_CHANGE;

        }



    }

    return 1;

}


