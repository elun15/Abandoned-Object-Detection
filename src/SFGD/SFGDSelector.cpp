/************************************************************************************/
/* Source file of SFGDSelector class                                                */
/* This class allows us to choose a static foreground detection method              */
/*                                                                                  */
/************************************************************************************/
#include "../../../src/SFGD/SFGDSelector.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "video.h"

using namespace std;
using namespace cv;

SFDGSelector::SFDGSelector(int SFGDid, double framerate, int time_to_static)
{
    cout << "SFGDSelector()" << endl;
    this->int_SFGDid = SFGDid;
    this->framerate = framerate;
    this->time2static = time_to_static;
}

SFDGSelector::~SFDGSelector()
{
    cout << "~SFGDSelector()" << endl;
}

void SFDGSelector::init(Mat frame)
{

    switch (this->int_SFGDid)
    {
    case 1:
        this->psubsamplingSFGD = new StaticMaskExtractor_subsampling(frame, this->framerate, this->time2static, SFGD_SUBSAMPLING_NUMSTAGES);
        break;
    case 2:
        this->pAccMaskSFGD = new StaticMaskExtractor_AccMask(frame, this->framerate, this->time2static, 2.0f, 250);
        break;
    default:
        break;
    }
}

void SFDGSelector::process(Mat frame, Mat foreground_img, Mat bgmodel_img,video Video)
{


    switch (this->int_SFGDid)
    {


    //Subsampling
    case 1:

        this->psubsamplingSFGD->processFrame(frame, foreground_img);

        if (Video.ShowResults){
            imshow("STATIC MOTION MASK SUBSAMPLING", this->psubsamplingSFGD->getStaticMask());
        }


        if (Video.SaveImages)
        {

            String d = Video.DirImages + "sfgd" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,this->psubsamplingSFGD->getStaticMask());
            d = Video.DirImages + "fg" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,foreground_img);

            return;


        }

        break;


    //ACC
    case 2:

        this->pAccMaskSFGD->processFrame(foreground_img, framerate, time2static);

        if (Video.ShowResults){
            imshow("STATIC MOTION MASK ACC", this->pAccMaskSFGD->getStaticMask());
        }


        if (Video.SaveImages)
        {

            String d = Video.DirImages + "sfgd" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,this->pAccMaskSFGD->getStaticMask());
            d = Video.DirImages + "fg" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,foreground_img);

            return;


        }
        break;


    default:
        break;
    }



    frame.release();
    foreground_img.release();
    bgmodel_img.release();


}

Mat SFDGSelector::GetStaticForeground(){
    switch (this->int_SFGDid)
    {

    //Subsampling
    case 1:
        return (this->psubsamplingSFGD->getStaticMask());
        break;

        //ACC
    case 2:
        return (this->pAccMaskSFGD->getStaticMask());
        break;

    default:
        break;
    }
}

void SFDGSelector::finish()
{

}
