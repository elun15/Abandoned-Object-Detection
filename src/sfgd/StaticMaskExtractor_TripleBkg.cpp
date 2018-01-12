/**
* \file StaticMaskExtractor_TripleBkg.cpp
* \author Elena Luna (elg)
* \date 25-06-13
* \brief Implementation of the triple bkg FSM
* \version 1.0\n
*			Version history:\n
*				- 1.0 (27-12-17	): Initial Implementation (elg)
*
*/

#include "StaticMaskExtractor_TripleBkg.h"
#include "../bgs/BGSselector.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

#define BG 0
#define MFO 255
#define SFO 100
#define UBG 50
#define OSFO 150
//value tags for the FSM


StaticMaskExtractor_TripleBkg::StaticMaskExtractor_TripleBkg(Mat sampleFrame, double _framerate, double time_to_static) :StaticMaskExtractor(sampleFrame, _framerate, time_to_static)
{

    this->evidenceImage = Mat::zeros(sampleFrame.size(),CV_32FC1);
    //this->evidenceImageNorm = Mat::zeros(sampleFrame.size(),CV_32FC1);
    this->oc_tolerance = 0.8;
    this->static_cont = floor(this->framerate*this->secs2static);
    this->static_th = this->static_cont * this->oc_tolerance;
    this->k = 1;

    this->Z = Mat::zeros(sampleFrame.size(),CV_8UC1); // State matrix
    this->Z_prev = Mat::zeros(sampleFrame.size(),CV_8UC1); // State matrix

}


/**s
*	Default destructor
*
*/
StaticMaskExtractor_TripleBkg::~StaticMaskExtractor_TripleBkg()
{

    evidenceImage.release();
   // evidenceImageNorm.release();

}


void StaticMaskExtractor_TripleBkg::processFrame( Mat fgLong, Mat fgMedium, Mat fgShort) //Solo pasamos bgS porque debe ser igual a bgL
{

    //staticMask.setTo(Scalar::all(0));

    Mat long1 = (fgLong == 255);
    Mat long0 = (fgLong == 0);

    Mat med1 = (fgMedium == 255);
    Mat med0 = (fgMedium == 0);

    Mat short1 = (fgShort == 255);
    Mat short0 = (fgShort == 0);

    Mat Z_prev_s0 = (Z_prev == BG);
    Mat Z_prev_s1 = (Z_prev == MFO);
    Mat Z_prev_s2 = (Z_prev == SFO);
    Mat Z_prev_s3 = (Z_prev == UBG);
    Mat Z_prev_s4 = (Z_prev == OSFO);

    // (L, M, S)
    // "Others" conditions

    // s0 from s0  (other)
    Z.setTo(BG,Z_prev_s0);

    // s1 from s1 (other)
    Z.setTo(MFO,Z_prev_s1);

    //s2 from s2 (whatever)
    Z.setTo(SFO,Z_prev_s2);


    //s3 from s3 (other)
    Z.setTo(UBG,Z_prev_s3);

    //s2 from s4 (other)
    Z.setTo(SFO,Z_prev_s4);


    // State 0 BG

    // from s3   (0, 0, 0)
    Mat cond_s0_froms3 = (long0 & med0 & short0 & Z_prev_s3);
    Z.setTo(BG,cond_s0_froms3);

    //from s1 (0, 0, 0)
    Mat cond_s0_froms1 = (long0 & med0 & short0 & Z_prev_s1);
    Z.setTo(BG,cond_s0_froms1);


    // State 1 MFO

    // from  s0 (1, 1, 1)
    Mat cond_s1_froms0 = long1 & med1 & short1 & Z_prev_s0;
    Z.setTo(MFO,cond_s1_froms0);

    // from s3 (1, 1, 1)
    Mat cond_s1_froms3 = long1 & med1 & short1 & Z_prev_s3;
    Z.setTo(MFO,cond_s1_froms3);


    // State 2 SFO

    //from s1 (1, 0, 0) (1, 1, 0)
    Mat cond_s2_froms1 = ((long1 & med0 & short0) | (long1 & med1 & short0)) & Z_prev_s1;
    Z.setTo(SFO,cond_s2_froms1);


    //State 4 OSFO

    //from s4 (1, 1, 1)
    Mat cond_s4_froms4 = (long1 & med1 & short1)  &  Z_prev_s4;
    Z.setTo(OSFO, cond_s4_froms4);

    //from s2
    Mat cond_s4_froms2 = (long1 & med1 & short1)  &  Z_prev_s2;
    Z.setTo(OSFO, cond_s4_froms2);

    // State 3 UBG

    // from s2 (0, 0, 0) (0, 0, 1) (0, 1, 1)
    Mat cond_s3_froms2 = ((long0 & med0 & short0) | (long0 & med0 & short1) | (long0 & med1 & short1)) & Z_prev_s2;
    Z.setTo(UBG,cond_s3_froms2);

    // from s1 (0, 0, 1) (0, 1, 1)
    Mat cond_s3_froms1 = ( (long0 & med0 & short1) | (long0 & med1 & short1)) & Z_prev_s1;
    Z.setTo(UBG,cond_s3_froms1);


    //from s4 (0, 0, 0) (0, 0, 1) (0, 1, 1)
    Mat cond_s3_froms4 = ((long0 & med0 & short0) | (long0 & med0 & short1) | (long0 & med1 & short1)) & Z_prev_s4;
    Z.setTo(UBG,cond_s3_froms4);



    Mat maskSFO = (Z == SFO);
    Mat masknotSFO = (Z != SFO);

    add(evidenceImage,1,evidenceImage,maskSFO);
    subtract(evidenceImage,k,evidenceImage,masknotSFO);

    //Limit to the maximum value
    Mat evidence_greater_mask;
    compare(evidenceImage,this->static_cont,evidence_greater_mask,CMP_GT); //mask = 1 when evidenceImage > static_cont
    evidenceImage.setTo(this->static_cont,evidence_greater_mask); //evidenceImage = static when mask = 1

    //Limit to the minimum value (0)
    Mat evidence_less_mask;//mask = 1 when evidenceImage < 0
    compare(evidenceImage,0.0,evidence_less_mask,CMP_LT);
    evidenceImage.setTo(0,evidence_less_mask);//evidenceImage = 0 when mask = 1


    evidence_greater_mask.setTo(Scalar::all(0));
    compare(evidenceImage,static_th,evidence_greater_mask,CMP_GT); //mask = 1 when evidenceImage > static_th
    staticMask.setTo(255,evidence_greater_mask); //Mask = 255  when evidenceImage > static_th
    staticMask.setTo(0,(255 - evidence_greater_mask)); //Mask = 0  when evidenceImage < static_th

    Z.copyTo(Z_prev);

   // imshow("mat Z ", Z);
   // waitKey(1);

   // imshow("static mask", staticMask);
   //waitKey(10);



}

