/************************************************************************************/
/* Archivo de la clase MyCLSSFRSelector:                                            */
/* Esta clase nos permite seleccionar la tecnica a utilizar en la etapa de          */
/* clasificacion robo/abandono del sistema.                                         */
/************************************************************************************/

#include "../../../src/CLASSIFIER/ClassifierSelector.h"
#include "../../../src/CLASSIFIER/StaticObjectClassifierHighGradient.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "video.h"


//Constructor: Identifica la tecnica de clasificacion robo/abandono seleccionada desde el programa principal
// e inicializa las variables necesarias.

ClassifierSelector::ClassifierSelector(int CLSSFRid)
{
    std::cout << "ClassifierSelector()" << std::endl;
    this->int_CLSSFRid = (CLSSFRid);
}

// Destructor.
ClassifierSelector::~ClassifierSelector()
{
    std::cout << "~ClassifierSelector()" << std::endl;
}

void ClassifierSelector::init()
{
    switch (this->int_CLSSFRid)
    {

    case 1:
        this->pSOCHighGradient = new StaticObjectClassifierHighGradient();
        cout << "HIGH GRAD" << endl;
        break;
    case 2:
        this->pSOCHist = new StaticObjectClassifierHist();
        cout << "HIST CLASS" << endl;
        break;


    default:
        break;
    }

}

Mat ClassifierSelector::process(cv::Mat frame, cv::Mat bkgImage, cv::Mat StaticFGImage2, cv::Mat ForegroundImage, BlobList<ObjectBlob*>* objects,video Video)
{


    Mat result;

    Mat StaticFGImage=StaticFGImage2.clone();
    //remove noise
    Mat element = getStructuringElement(MORPH_RECT, Size(3,3));


    erode(StaticFGImage, StaticFGImage,element);

    dilate(StaticFGImage, StaticFGImage,element);


    //fill holes
    dilate(StaticFGImage, StaticFGImage,element);
    dilate(StaticFGImage, StaticFGImage,element);
    erode(StaticFGImage, StaticFGImage,element);
    dilate(StaticFGImage, StaticFGImage,element);

    //binarization
    threshold(StaticFGImage, StaticFGImage, 250, 255, CV_THRESH_BINARY);


    switch (this->int_CLSSFRid)
    {

    // HIGH GRADIENT METHOD
    case 1:

        this->pSOCHighGradient->classifyBlobStationary(frame,bkgImage,StaticFGImage,objects);

        result = (this->pSOCHighGradient->printBlobs(frame, objects)).clone();


        if (Video.ShowResults)
        {
            imshow("HG Classifier", result);
            waitKey(1);

        }


        if (Video.SaveImages)
        {

            String d = Video.DirImages + "class" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,result);

        }

        break;

    // COLOR HISTOGRAM METHOD

    case 2:

        this->pSOCHist->processFrame(frame, bkgImage, StaticFGImage, ForegroundImage, objects);
        result = (this->pSOCHist->printBlobs(frame, objects)).clone();

        if (Video.ShowResults)
        {  imshow("Hist Classifier", result);
            waitKey(1);}


        if (Video.SaveImages)
        {

            String d = Video.DirImages + "class" + to_string(Video.numFrame) + ".jpg";
            imwrite(d,result);

        }

        break;



    default:
        break;

    }



    return result;






}
