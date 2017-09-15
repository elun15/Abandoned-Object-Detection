#include <opencv2/opencv.hpp>
#include <iostream>

#include "../../../src/BLOBS/BlobList.h"
#include "../../../src/BLOBS/ObjectBlob.h"
#include "../../../src/BLOBS/CompareBlobs.h"

void DefineObjectBlobList(std::vector<cvBlob> *ExtractBlobList, vector<cv::Rect> found, BlobList<ObjectBlob*> *pObjectList, cv::Mat mask = cv::Mat());

// Input/Output Variables
//
// ExtractBlobList:   list of blobs extracted from the static foreground mask
// found:             vector of rectangles, representing detected people
// pObjectList
