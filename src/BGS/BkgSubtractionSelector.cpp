/************************************************************************************/
/* Source file of BkgSubtractionSelector class                                      */
/* This class allows us to choose a background subtraction method                   */
/*                                       */
/************************************************************************************/
#include "BkgSubtractionSelector.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "../../../lib/bgslibrary/package_bgs/FrameDifferenceBGS.h"

#include "../../../lib/bgslibrary/package_bgs/pl/BackgroundSubtractorLOBSTER.h"
#include "../../../lib/bgslibrary/package_bgs/pl/BackgroundSubtractorLBSP.h"
#include "../../../lib/bgslibrary/package_bgs/db/imbs.hpp"
#include "../../../lib/bgslibrary/package_bgs/db/IndependentMultimodalBGS.h"
#include "../../../lib/BackgroundSubtractorPAWCS.h"

#include "video.h"


using namespace std;
using namespace cv;


//Constructor: Identify selected subtraction technique from the main programa and initialize the needed variables

BkgSubtractionSelector::BkgSubtractionSelector(int BGSid)
{

    this->int_BGSid = BGSid;
    this->learningRateOverride = 0;
    this->learningRate = 16;
    cout << "BkgSubtractionSelector()" << endl;
    cout << "Method: " << BGSid << endl;


}

// Destructor.
BkgSubtractionSelector::~BkgSubtractionSelector()
{
    std::cout << "~BkgSubtractionSelector()" << std::endl;
}


//Init function: Determines the variables size in order to suit them to the video sequence resolution
//and create the subtraction technique object
void BkgSubtractionSelector::init(Mat frame)
{


    this->img_output = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    this->img_bgmodel = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
    this->SecuenceROI = Mat(frame.size(), CV_8UC1, cv::Scalar_<uchar>(255));

    switch (this->int_BGSid)
    {



    case 1:
        cout << "LOBSTER Method selected." << endl;
        this->pLOBSTERBGS = new BackgroundSubtractorLOBSTER();
        this->pLOBSTERBGS->initialize(frame, this->SecuenceROI);

        break;

    case 2:
        cout << "PAWCS Method selected." << endl;
        this->pPAWCSBGS = new BackgroundSubtractorPAWCS();
        this->pPAWCSBGS->initialize(frame, this->SecuenceROI);

        break;


    case 3:

        cout << "MOG2 Method selected." << endl;
        this->pMOG2  = createBackgroundSubtractorMOG2(400);
        //(int history=200, int nmixtures=5, double backgroundRatio=0.7, double noiseSigma=0
        this->pMOG2->setBackgroundRatio(0.7);
        //this->pMOG2 = createBackgroundSubtractorMOG2();

        this->pMOG2->setNMixtures(5);

        break;

    case 4:

        cout << "KNN Method selected." << endl;
        this->pKNN = createBackgroundSubtractorKNN();
        break;

    case 5:

        cout << "Independent Multimodal Method selected." << endl;
        this->pIMBSBGS = new IndependentMultimodalBGS();
        break;


    default:
        break;

    }


}




//Funcion process: realiza el procesamiento de la secuencia de video a analizar segun la tecnica de substraccion de fondo
//seleccionada desde el programa principal y muestra los resultados si asi se desea.
void BkgSubtractionSelector::process(Mat frame,video Video)
{
    img_input = frame.clone();
    img_output = Mat::zeros(frame.rows,frame.cols,frame.type());

    switch (this->int_BGSid)
    {

    case 1:

        this->pLOBSTERBGS->apply(this->img_input, this->img_output, this->learningRate);
        if (this->learningRate < 100){
            this->learningRate++;
        }
        this->pLOBSTERBGS->getBackgroundImage(this->img_bgmodel);
        if (Video.ShowResults){

            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

            imshow("Frame",img_input);
            imshow("LOBSTER FG", this->img_output);
            imshow("LOBSTER Reconstructed BG", this->img_bgmodel);
            waitKey(1);
        }

        if (Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
        {

            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            String d = Video.DirImages + "frame" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,img_input);

        }





    case 2:

        this->pPAWCSBGS->apply(this->img_input, this->img_output, this->learningRateOverride);
        if (this->learningRateOverride < 100){
            this->learningRateOverride++;
        }
        this->pPAWCSBGS->getBackgroundImage(this->img_bgmodel);

        if (Video.ShowResults){
            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            imshow("Frame",img_input);
            imshow("PAWCS FG", this->img_output);
            imshow("PAWCS Reconstructed BG", this->img_bgmodel);
            waitKey(1);
        }

        if (Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
        {

            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            String d = Video.DirImages + "frame" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,img_input);

        }



        break;


    case 3:

        //this->pMOG2->apply(this->img_input, this->img_output,0.001/15);
        this->pMOG2->apply(this->img_input, this->img_output);
        this->pMOG2->getBackgroundImage(this->img_bgmodel);
        if (Video.ShowResults){

            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            imshow("Frame",img_input);
            waitKey(1);
            imshow("MoG2 FG", this->img_output);
            waitKey(1);
            imshow("MOG2 BG Model", this->img_bgmodel);
            waitKey(1);
        }


        if (Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
        {

            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            String d = Video.DirImages + "frame" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,img_input);

        }
        break;

    case 4:

        double learn;
        // learn = 0.001; //0 not updated. 1 totally updated
        cout << "learning rate " << learn << endl;
        this->pKNN->apply(this->img_input, this->img_output,learn);
        this->pKNN->getBackgroundImage(this->img_bgmodel);
        if (Video.ShowResults){
            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            imshow("Frame",img_input);
            imshow("KNN FG", this->img_output);
            imshow("KNN Reconstructed BG", this->img_bgmodel);
            waitKey(40);
        }

        if (Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
        {

            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            String d = Video.DirImages + "frame" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,img_input);

        }

        break;

    case 5:
        this->pIMBSBGS->process(this->img_input, this->img_output, this->img_bgmodel);
        if (Video.ShowResults){

            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            imshow("Frame",img_input);
            imshow("IMBS FG", this->img_output);
            imshow("IMBS BG Model", this->img_bgmodel);
            waitKey(1);
        }

        if (Video.SaveImages && (Video.numFrame % 20 == 0)) //Save 1 frame out of every 20
        {

            putText(img_input,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
            String d = Video.DirImages + "frame" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,img_input);

        }
        break;



    default:

        break;

    }
}




//Funcion GetForegroundImage: Captura y devuelve el foreground procesado del frame actual bajo analisis.
Mat BkgSubtractionSelector::GetForegroundImage()
{
    vector<Mat> vector;
    vector.push_back(this->img_output);
    //return vector;
    return this->img_output;
}


//Funcion GetBGModel: Captura y devuelve el modelo de fondo procesado del frame actual bajo analisis.
Mat BkgSubtractionSelector::GetBGModel()
{
    return this->img_bgmodel;
}
