#ifndef DETECTORSELECTOR_H
#define DETECTORSELECTOR_H

#endif // DETECTORSELECTOR_H
#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "../../../lib/DPM/dpm.hpp"
//ACF
#include "../../../lib/ACF/ACFDetector.h"
#include "../../../lib/Core/DetectionList.h"
#include "../../../lib/Core/NonMaximumSuppression.h"


using namespace std;
using namespace cv;
using namespace cv::dpm;

//Declaracion de la clase MyPEDESTRIANDETECTORSelector
class DetectorSelector
{
public:
    int int_PEDESTRIANDETECTORid;
    vector<Rect> found;

    // Creacion de los objetos de las clases que definen las distintas tecnicas de deteccion de personas.
    HOGDescriptor pHOG;
  
    //ACF
    ACFDetector Dt;
    NonMaximumSuppression NMS;
    DetectionList Dl;
    DetectionList Dl_NMS;

    
    //DPM
    Ptr<DPMDetector> pDPM;
    Mat frameAux; 
    vector<DPMDetector::ObjectDetection> DPMBoundingBoxesAux; // DPM



    //Definicion de las funciones de la clase MyPEDESTRIANDETECTORSelector

    DetectorSelector(int PEDESTRIANDETECTORid);
    ~DetectorSelector();

    void init();
    vector<Rect> process(Mat frame);
    void non_max_suppresion(const vector<Rect> &srcRects, vector<Rect> &resRects, float thresh);
};

