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
#include "../../../src/settings.h"

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

void SFDGSelector::init(Mat frame, BkgSubtractionSelector bgs)
{

    switch (this->int_SFGDid)
    {
    //SUBSAMPLING
    case 1:
        this->psubsamplingSFGD = new StaticMaskExtractor_subsampling(frame, this->framerate, this->time2static, SFGD_SUBSAMPLING_NUMSTAGES);
        break;

        //ACC MASK
    case 2:
        this->pAccMaskSFGD = new StaticMaskExtractor_AccMask(frame, this->framerate, this->time2static, 2.0f, 250);
        break;

        //HistoryImages
    case 3:
        this->pHistoryImagesSFGD = new StaticMaskExtractor_HistoryImages(frame, this->framerate, this->time2static, 2.0f, 250);
        break;



<<<<<<< HEAD
        break;


        //DBM
    case 4:

        this->pDBM = new StaticMaskExtractor_DualBkg(frame,this->framerate,this->time2static,&bgs, this->alpha);

        break;
=======
>>>>>>> doh2
    default:
        break;
    }
}

void SFDGSelector::process(Mat frame, std::vector<cv::Mat> foreground_img, Mat bgmodel_img, settings Video)
{


    Mat foreground;

    // If 3 channels ->  convert to 1 channel
    if (foreground_img[0].channels() == 1){

        foreground = foreground_img[0];
    }
    else{

        Mat tmp;
        cvtColor(foreground_img[0], tmp, CV_BGR2GRAY);
        foreground = tmp;

    }


    Mat foregroundShort = foreground_img[0];
    Mat foregroundLong;

    if (foreground_img.size() == 2)
    {
        foregroundLong = foreground_img[1];
    }



    switch (this->int_SFGDid)
    {


    //Subsampling
    case 1:

        this->psubsamplingSFGD->processFrame(frame, foreground);

        //if (Video.ShowResults){
        //    imshow("STATIC MOTION MASK SUBSAMPLING", this->psubsamplingSFGD->getStaticMask());
        //}

        if (Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
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

        this->pAccMaskSFGD->processFrame(foreground, framerate, time2static);

        //  if (Video.ShowResults){
        //     imshow("STATIC MOTION MASK ACC", this->pAccMaskSFGD->getStaticMask());
        // }


        if (Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
        {

            String d = Video.DirImages + "sfgd" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,this->pAccMaskSFGD->getStaticMask());
            d = Video.DirImages + "fg" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,foreground_img);

            return;


        }
        break;


        //History Images

    case 3:

        this->pHistoryImagesSFGD->processFrame(foreground_img, frame, bgmodel_img, framerate, time2static);

        if (Video.ShowResults){
            imshow("STATIC MOTION MASK HISTORY", this->pHistoryImagesSFGD->getStaticMask());
        }


        if (Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
        {

            String d = Video.DirImages + "sfgd" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,this->pHistoryImagesSFGD->getStaticMask());
            d = Video.DirImages + "fg" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,foreground_img);

            return;

<<<<<<< HEAD
=======

        }
>>>>>>> doh2
        break;

        //DBM
    case 4:

        this->pDBM->processFrame(foregroundLong,foregroundShort);

        break;

    default:
        break;
    }



    frame.release();
    foreground_img.clear();
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

<<<<<<< HEAD

    case 4:
        return(this->pDBM->getStaticMask());

=======
        //HISTORY IMAGES
    case 3:
        return (this->pHistoryImagesSFGD->getStaticMask());
>>>>>>> doh2
        break;

    default:
        break;
    }
}

void SFDGSelector::finish()
{

}
