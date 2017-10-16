#ifndef STATICMASKEXTRACTOR_DUALBKG_H
#define STATICMASKEXTRACTOR_DUALBKG_H

#endif // STATICMASKEXTRACTOR_DUALBKG_H

/**
* \file StaticMaskExtractor_DualBkg.cpp
* \author Diego Ortego Hernandez (doh)
* \date 25-06-13
* \brief Interface of the static Method explained in
* \version 1.0\n
*			Version history:\n
*				- 1.0 (25-06-13	): Initial Implementation (doh)
*               - 2.0 (03/10/17) : Updated (elg)
*
*/

#pragma once;


#include "StaticMaskExtractor.h"

#include "../../../src/BGS/DualBkgSubtractionSelector.h"

//class definition
class StaticMaskExtractor_DualBkg : public StaticMaskExtractor
{
public:

    // Default constructor
    StaticMaskExtractor_DualBkg(Mat sampleFrame, double _framerate = FRAMERATE_DEF, double time_to_static = TIME_STATIC_DEF, BkgSubtractionSelector *bgS = NULL, double alpha = 0.0);

    // Default destructor
    virtual ~StaticMaskExtractor_DualBkg();

    //Method to process each frame
    void processFrame(Mat fgLong, Mat fgShort);


private:

    Mat evidenceImage, evidenceImageNorm;
    int k;
    double static_cont, oc_tolerance;


};
