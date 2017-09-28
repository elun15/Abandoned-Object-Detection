#ifndef DRAWDETECTIONS_H
#define DRAWDETECTIONS_H


#include <opencv2/opencv.hpp>
#include <iostream>
#include "BasicBlob.h"
#include "solape.h"
#include "../../../src/settings.h"

using namespace cv;
using namespace std;

Mat DrawDetections(cv::Mat frame, vector<Rect> found, std::vector<cvBlob> *BlobList, settings Video);//size_t i, size_t j, bool ShowResults);

// Input/Output Variables:
//
// frame:            original video frame
// found:            vector of rectangles where each rectangle contains the detected person
// found_filtered:   vector of rectangles without duplicated rectangles
// BlobList:         list of blobs extracted from the static foreground mask
// i, j:             counter loop variables
// ShowResults:      boolean variable

#endif // DRAWDETECTIONS_H
