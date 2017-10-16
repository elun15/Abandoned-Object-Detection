#include "../../../src/Utils/DrawDetections.h"
#include "../../../src/settings.h"
#include <opencv2/opencv.hpp>


Mat DrawDetections(cv::Mat frame, vector<Rect> found, std::vector<cvBlob> *BlobList,settings Video)
{

    Mat aux ;
    aux = frame.clone();

    size_t i = Video.i;
    size_t j = Video.j;


    for (i = 0; i<found.size(); i++)//i goes through all the filtered detections
    {

        //(x,y) : coordinates of the top-left corner
        Rect r = found[i];
        // rectangle(aux, r.tl(), r.br(), cv::Scalar(0, 0, 255), 2); //Draw moving people in red without make ir bigger
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.06);
        r.height = cvRound(r.height*0.9);
        //tl: top-left corner, br: bottom-right corner
         rectangle(aux, r.tl(), r.br(), cv::Scalar(255, 0, 0), 2); //Draw moving people in blue

        for (cvBlob iter : *BlobList){
         Rect b;//rectangle containing static blob
         b.x = iter.x;
         b.y = iter.y;
         b.height = iter.h;
         b.width = iter.w;
         rectangle(aux, b.tl(), b.br(), cv::Scalar(0, 0, 255), 2);//Draw static blob in red

            if (solape(r, b) > 0.5)
            {
                rectangle(aux, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);//Draw static people in green

            }



        }


    }
    if (Video.ShowResults){

        imshow("Pedestrian Detection", aux);
        //cv::waitKey(1);
    }


    return aux;
    found.clear();
    aux.release();



    //delete aux;

}
