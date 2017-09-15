
#ifndef _STATICOBJECTCLASSIFIERHIGHGRADIENT_INCLUDED_

#define _STATICOBJECTCLASSIFIERHIGHGRADIENT_INCLUDED_

#include "StaticObjectClassifier.h"

//class description
/**
* \class StaticObjectClassifierHist
* \brief Class to determine the state of a stationary blob using colour information (histogram)
*
*/
class StaticObjectClassifierHighGradient : public StaticObjectClassifier
{
public:

     StaticObjectClassifierHighGradient();
     ~StaticObjectClassifierHighGradient();
     int margin;


    //abandoned/stolen classification
    int classifyBlobStationary(Mat frame, Mat bkg, Mat sfgmask, BlobList<ObjectBlob*>* objects);
   // cv::Rect check_size(Mat frame, std::vector<cvBlob> &sbloblist, int margin, int j);
};


#endif // STATICOBJECTCLASSIFIERHIGHGRADIENT_H
