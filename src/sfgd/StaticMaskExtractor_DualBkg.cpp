/**
* \file StaticMaskExtractor_DualBkg.cpp
* \author Diego Ortego Hernandez (doh)
* \date 25-06-13
* \brief Implementation of the common interface for the static region detection algorithms
* \version 1.0\n
*			Version history:\n
*				- 1.0 (25-06-13	): Initial Implementation (doh)
*
*/

#include "StaticMaskExtractor_DualBkg.h"
#include "../bgs/BGSselector.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;



StaticMaskExtractor_DualBkg::StaticMaskExtractor_DualBkg(Mat sampleFrame, double _framerate, double time_to_static) :StaticMaskExtractor(sampleFrame, _framerate, time_to_static)
{

    this->evidenceImage = Mat::zeros(sampleFrame.size(),CV_32FC1);
    this->evidenceImageNorm = Mat::zeros(sampleFrame.size(),CV_32FC1);
    this->static_cont = floor(this->framerate*this->secs2static);
    this->k = 1;
    this->oc_tolerance = 0.7;

}


/**
*	Default destructor
*
*/
StaticMaskExtractor_DualBkg::~StaticMaskExtractor_DualBkg()
{

    evidenceImage.release();
    evidenceImageNorm.release();

}


void StaticMaskExtractor_DualBkg::processFrame( Mat fgLong, Mat fgShort) //Solo pasamos bgS porque debe ser igual a bgL
{

    staticMask.setTo(Scalar::all(0));

    //FL=1 & FS=0 --> +1
    //FL!=1 & FS!=0 --> -k
    Mat fgLong_mask = (fgLong == 255); //mask = 1 when fgLong == 250
    Mat fgShort_mask = (fgShort == 0); //mask = 1 when fgShort == 0
    Mat both_mask;//mask = 1 when both conditions
    bitwise_and(fgLong_mask,fgShort_mask,both_mask);
    add(evidenceImage,1,evidenceImage,both_mask); //evidenImage ++ in pixels fullfilling both conditions
    Mat both_mask_not;
    bitwise_not(both_mask,both_mask_not);
    subtract(evidenceImage,k,evidenceImage,both_mask_not); //evidenImage - k in pixels not fullfilling both conditions

    //Limit to the maximum value
    Mat evidence_greater_mask; //mask = 1 when evidenceImage > static_count
    compare(evidenceImage,this->static_cont,evidence_greater_mask,CMP_GT); //mask = 1 when evidenceImage > static_cont
    evidenceImage.setTo(this->static_cont,evidence_greater_mask); //evidenceImage = static when mask = 1

    //Limit to the minimum value (0)
    Mat evidence_less_mask;//mask = 1 when evidenceImage < 0
    compare(evidenceImage,0.0,evidence_less_mask,CMP_LT);
    evidenceImage.setTo(0,evidence_less_mask);//evidenceImage = 0 when mask = 1

    //Check static threshold and fill Mask
    double static_th = this->static_cont * this->oc_tolerance;
   // double static_th = 100;
    evidence_greater_mask.setTo(Scalar::all(0));
    compare(evidenceImage,static_th,evidence_greater_mask,CMP_GT); //mask = 1 when evidenceImage > static_th
    staticMask.setTo(255,evidence_greater_mask); //Mask = 255  when evidenceImage > static_th
    staticMask.setTo(0,(255 - evidence_greater_mask)); //Mask = 0  when evidenceImage < static_th
    //imshow("Evidence image", evidenceImage);
   // waitKey(1);




}

