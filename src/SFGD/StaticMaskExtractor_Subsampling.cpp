/**
* \file StaticMaskExtractor_subsampling.cpp
* \author Juan Carlos San Miguel Avedillo (jcs)
* \date 16-12-10
* \brief Implementation of the common interface for the static region detection algorithms
* \version 2.0\n
*			Version history:\n
*				- 1.0 (01-09-09): Initial Implementation (jcs)
*				- 2.0 (16-12-10): Modification for being a common interface (jcs)
*				- 3.0 (07-12-12): He corregido el algoritmo, pues el anterior, solo hacia AND
*								  de las mascaras FG muestreadas cuando stage_sampled == num_stages
*								  y eso solo es la primera vez, despues la AND debe ser continua cada
*								  vez que haya muestreo de una nueva mascara FG.
*/

#include "StaticMaskExtractor_subsampling.h"
#include "../../../src/SFGD/StaticMaskExtractor.h"
#include "../../../src/SFGD/StaticMaskExtractor_subsampling.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;


/**
*	Class Constructor with initial data
*
*	\param sampleFrame Sample of images to process in the future
*	\param log Pointer to log file
*	\param _num_stages Number of sampling stages applied to obtain the stationary region mask
*	\param time_to_static Time (in secs) to consider a region as 'static'
*	\param _framerate Framerate of the input video frame
*
*/

StaticMaskExtractor_subsampling::StaticMaskExtractor_subsampling(Mat sampleFrame, double _framerate, double time_to_static, int _num_stages) : StaticMaskExtractor(sampleFrame, _framerate, time_to_static)
{
    num_stages = _num_stages;
    numFramesToSample = (int)((this->secs2static * this->framerate) / this->num_stages);//Frames entre cada muestra tomada (cada cuanto cojo una muestra)

    stage_sampled = 0;


    for (int i = 0; i< MAX_STAGES; i++)
    {

          pSampledMask.push_back(Mat::zeros(arrSize,CV_8UC1));
    }
}

/**
*	Default destructor
*
*/

StaticMaskExtractor_subsampling::~StaticMaskExtractor_subsampling()
{

        pSampledMask.clear();


}



/**
*	Method to compute the stationary region mask using a subsampling scheme of FG masks
*	This method has to be implemented in derived classes
*
*	\param frame Current image (Color or gray)
*	\param fg_mask Mask indicating the presence of moving objects (Gray)
*
*/

void StaticMaskExtractor_subsampling::processFrame(Mat frame, Mat fg_mask)
{
    counter = counter + 1;

    if (counter == 1 || counter%numFramesToSample == 0) //Si es la primera vez o si han pasado numFramesToSample
    {


        fg_mask.copyTo(pSampledMask[stage_sampled]);


        stage_sampled++;//Incremento, cuando sea 6(num_stages) entra en el if siguiente


        if (counter >= ((num_stages*numFramesToSample) - numFramesToSample))
        {

            //fusion of sampled masks using logical AND
            this->pSampledMask[0].copyTo(this->staticMask);

            for (int j = 1; j<num_stages; j++)
                 bitwise_and(this->staticMask, this->pSampledMask[j], this->staticMask);
            if (this->num_stages == stage_sampled)
                stage_sampled = 0;//Reseteo de etapas de muestreo


        }
    }
}



/**
*	Method to get the 'n' subsample FG mask
*
*	\param n Stage number (< MAX_STAGES)
*	\returns An IplImage containing the 'n' sampled FG mask (1 channel and IPL_DEPTH_8U)
*
*/

Mat StaticMaskExtractor_subsampling::getSampledMask(int n)
{
    if (n<MAX_STAGES)
        return this->pSampledMask[n];

}

/**
*	Method to set the time to detect an object as stationary.
*	This class redefines original behaviour of the parent's method
*
*	\param _time New value
*
*/

void StaticMaskExtractor_subsampling::setTime(double _time)
{
    this->secs2static = _time;

    this->numFramesToSample = (int)((this->secs2static * framerate) / num_stages);
}

/**
*	Method to set the number of stages of the algorithm
*
*	\param _NumStages New value
*
*/


void StaticMaskExtractor_subsampling::setNumStages(int _NumStages)
{
    if (_NumStages <= MAX_STAGES)
    {
        this->num_stages = _NumStages;

        this->numFramesToSample = (int)((this->secs2static * framerate) / num_stages);
    }
}

/**
*	Method to set the framerate of the input video
*	This class redefines original behaviour of the parent's method
*
*	\param _framerate New value
*
*/

void StaticMaskExtractor_subsampling::setFramerate(double _framerate)
{
    this->framerate = _framerate;

    this->numFramesToSample = (int)((this->secs2static * framerate) / num_stages);
}

/**
*	Method to get the number of stages of the algorithm
*
*	\return currently used value
*
*/

int StaticMaskExtractor_subsampling::getNumStages()
{
    return this->num_stages;
}

