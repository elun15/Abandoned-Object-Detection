#ifndef STATICMASKEXTRACTOR_TRIPLEBKG_H
#define STATICMASKEXTRACTOR_TRIPLEBKG_H

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
#include "StaticMaskExtractor.h"
#include "../bgs/BGSselector.h"

//class definition
class StaticMaskExtractor_TripleBkg : public StaticMaskExtractor
{

public:

    // Default constructor
    StaticMaskExtractor_TripleBkg(Mat sampleFrame, double _framerate = FRAMERATE_DEF, double time_to_static = TIME_STATIC_DEF);

    // Default destructor
    virtual ~StaticMaskExtractor_TripleBkg();

    //Method to process each frame
    void processFrame(Mat fgLong, Mat fgMedium, Mat fgShort);
    Mat Z , Z_prev;

private:

    int k;
    Mat evidenceImage, evidenceImageNorm;
    double static_cont, oc_tolerance, static_th;


};

#endif // STATICMASKEXTRACTOR_TRIPLEBKG_H
