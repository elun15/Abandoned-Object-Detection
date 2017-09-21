/************************************************************************************/
/* Source file of BkgSubtractionSelector class                                      */
/* This class allows us to choose a background subtraction method                   */
/*                                       */
/************************************************************************************/

#include <opencv2/opencv.hpp>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "detectorselector.h"

//#include "../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorPAWCS.h"



#include <opencv2/highgui/highgui.hpp> //VideoCapture
#include <opencv2/imgproc/imgproc.hpp> //cvtColor
#include <opencv2/opencv.hpp> //opencv libraries

#include "../../../lib/DPM/dpm.hpp"


using namespace std;
using namespace cv;

DetectorSelector::DetectorSelector(int PEDESTRIANDETECTORid){
    std::cout << "MyPEDESTRIANDETECTORSelector()" << std::endl;
    this->int_PEDESTRIANDETECTORid = (PEDESTRIANDETECTORid);
}


DetectorSelector::~DetectorSelector(){

    std::cout << "~MyPEDESTRIANDETECTORSelector()" << std::endl;
}

void DetectorSelector::init(){

    switch (this->int_PEDESTRIANDETECTORid)
    {
    case 1:
        this->pHOG.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
        break;

    case 2: //DPM
        this->pDPM  = DPMDetector::create(vector<string>(1, "/home/vpu/AOD_System/lib/DPM/inriaperson.xml"));

        break;

    case 3: // ACF

        break;



    default:
        break;

    }

}

vector<Rect> DetectorSelector::process(Mat frame){

    this->found.clear();
    frameAux = frame.clone();

    switch (this->int_PEDESTRIANDETECTORid)
    {

    // HOG
    case 1:
        this->pHOG.detectMultiScale(frame, this->found, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2);

        break;

        // DPM
    case 2:

        // DPM detector with NMS. The function destroys the Frame

        this->pDPM->detect(frameAux, this->DPMBoundingBoxesAux);

        for (size_t i = 0; i < DPMBoundingBoxesAux.size(); i++)
        {
            this->found.push_back(this->DPMBoundingBoxesAux[i].rect);
        }

        DPMBoundingBoxesAux.clear();

        break;

        //ACF
    case 3:
        //Apply ACF detector
        Dl = Dt.applyDetector(frameAux);

        //Apply NMS
        Dl_NMS = NMS.dollarNMS(Dl);
        for ( int i = 0; i < Dl_NMS.Ds.size(); i++)
        {
            Rect detection = Rect (Dl_NMS.Ds[i]->getX(),Dl_NMS.Ds[i]->getY(),Dl_NMS.Ds[i]->getWidth(),Dl_NMS.Ds[i]->getHeight());
             this->found.push_back(detection);
        }




        break;

    default:
        break;
    }
    //printf("Number of detected people %d \n", this->found.size());
    return this->found;
}


void DetectorSelector::non_max_suppresion(const vector<Rect> &srcRects, vector<Rect> &resRects, float thresh)
{
    resRects.clear();

    const size_t size = srcRects.size();
    if (!size)
        return;

    // Sort the bounding boxes by the bottom - right y - coordinate of the bounding box
    multimap<int, size_t> idxs;
    for (size_t i = 0; i < size; ++i) {
        idxs.insert(pair<int, size_t>(srcRects[i].br().y, i));
    }

    // keep looping while some indexes still remain in the indexes list
    while (idxs.size() > 0) {
        // grab the last rectangle
        auto lastElem = --end(idxs);
        const Rect& rect1 = srcRects[lastElem->second];

        resRects.push_back(rect1);

        idxs.erase(lastElem);

        for (auto pos = begin(idxs); pos != end(idxs); ) {
            // grab the current rectangle
            const Rect& rect2 = srcRects[pos->second];

            float intArea = (rect1 & rect2).area();
            float unionArea = rect1.area() + rect2.area() - intArea;
            float overlap = intArea / unionArea;


            // if there is sufficient overlap, suppress the current bounding box
            if (overlap > thresh) {
                pos = idxs.erase(pos);
            }
            else {
                ++pos;
            }
        }
    }
}
