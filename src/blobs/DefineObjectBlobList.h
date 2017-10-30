#include <opencv2/opencv.hpp>
#include <iostream>

#include "blobs/BlobList.h"
#include "blobs/ObjectBlob.h"

void DefineObjectBlobList(std::vector<cvBlob> *ExtractBlobList, vector<cv::Rect> &found, BlobList<ObjectBlob*> *pObjectList, cv::Mat mask = cv::Mat());
bool CompareBlobs(cvBlob blob, cv::Rect rect);
double solape(cv::Rect hogRect, cv::Rect bloblistRect);


// Input/Output Variables
//
// ExtractBlobList:   list of blobs extracted from the static foreground mask
// found:             vector of rectangles, representing detected people
// pObjectList
