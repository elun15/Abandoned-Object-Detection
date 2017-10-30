
#ifndef _STATICOBJECTCLASSIFIERHIGHGRADIENT_INCLUDED_

#define _STATICOBJECTCLASSIFIERHIGHGRADIENT_INCLUDED_

#include "soc/StaticObjectClassifier.h"

//class description
/**
* \class StaticObjectClassifierGradient
* \brief Class to determine the state of a stationary blob using gradient information
*
*/
class StaticObjectClassifierHighGradient : public StaticObjectClassifier
{
public:

     StaticObjectClassifierHighGradient();
     ~StaticObjectClassifierHighGradient();
     int _margin;
     double _minThreshold;

     /// Classifies a static object between abandoned and stolen
     int checkObject(cv::Mat frame, cv::Mat bkgImage, cv::Mat staticObjMask, cv::Mat fgMask, ObjectBlob* object);
};


#endif // STATICOBJECTCLASSIFIERHIGHGRADIENT_H
