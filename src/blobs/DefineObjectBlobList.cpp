#include "DefineObjectBlobList.h"

using namespace cv;

void DefineObjectBlobList(std::vector<cvBlob> *inBlobList, vector<cv::Rect>& found, BlobList<ObjectBlob*> *outObjectList,Mat mask, bool flag_minsize, bool flag_nearpeople, bool flag_stillpeople)
{
    // INPUT ARGUMENTS
    // inBlobList =>  all blobs in static foreground
    // found => people detections

    // OUTPUT ARGUMENTS
    // outObjectList  => list with the static objects in the scene

    ObjectBlob *pObjectBlob;
    bool blobIsObject = true;
    bool blobIsAttended = false;

    if (!inBlobList->empty()){

        for (cvBlob staticBlob : *inBlobList) //loop through static foreground blobs
        {
            blobIsObject = true;
            Point2f static_bottom(staticBlob.y+staticBlob.h, staticBlob.x+(staticBlob.w/2));

            for (cv::Rect personBlob : found){ //loop through detected people

                if (flag_stillpeople == true)
                {
                    if (CompareBlobs(staticBlob, personBlob)){
                        blobIsObject = false; //at least 50% of being a person
                        blobIsAttended = true;
                    }
                }

                if (flag_nearpeople == true)
                {
                    //Check if it is near a person
                    Point2f people_bottom(personBlob.y+personBlob.height,personBlob.x+(personBlob.width/2));
                    double distance = cv::norm(cv::Mat(static_bottom),cv::Mat(people_bottom));

                    if (distance  < 2*staticBlob.w){
                        blobIsObject = false;
                        blobIsAttended = true;
                    }
                }
            }

            if (blobIsObject == true && !mask.empty()) //Only if we have context mask
            {
                int offset = 20; //Allow to set an offset, if required

                //Extract the 4 corners points of the static blob plus and offset
                int coord_x;
                int coord_y;

                Point topLeft;
                coord_x=staticBlob.x+offset;
                coord_y= staticBlob.y+offset;

                if (coord_x < 0 )
                    coord_x = 0;
                if (coord_y < 0 )
                    coord_y = 0;
                if (coord_x > mask.cols)
                    coord_x = mask.cols;
                if (coord_y > mask.rows)
                    coord_y = mask.rows;

                topLeft = Point(coord_x, coord_y);

                Point topRight;
                coord_x= staticBlob.x+staticBlob.w-offset;
                coord_y= staticBlob.y+offset;

                if (coord_x < 0 )
                    coord_x = 0;
                if (coord_y < 0 )
                    coord_y = 0;
                if (coord_x > mask.cols)
                    coord_x = mask.cols;
                if (coord_y > mask.rows)
                    coord_y = mask.rows;

                topRight = Point(coord_x, coord_y);

                Point bottomLeft;
                coord_x=staticBlob.x+offset;
                coord_y= staticBlob.y+staticBlob.h-offset;

                if (coord_x < 0 )
                    coord_x = 0;
                if (coord_y < 0 )
                    coord_y = 0;
                if (coord_x > mask.cols)
                    coord_x = mask.cols;
                if (coord_y > mask.rows)
                    coord_y = mask.rows;

                bottomLeft= Point(coord_x, coord_y);

                Point bottomRight;
                coord_x=staticBlob.x+staticBlob.w-offset;
                coord_y= staticBlob.y+staticBlob.h-offset;

                if (coord_x < 0 )
                    coord_x = 0;
                if (coord_y < 0 )
                    coord_y = 0;
                if (coord_x > mask.cols)
                    coord_x = mask.cols;
                if (coord_y > mask.rows)
                    coord_y = mask.rows;

                bottomRight = Point(coord_x,coord_y);

                // Check if the static blob is inside/outside the context mask
                // If it is competely inside, or at least 3 of the 4 corners are inside the mask, it is discarded

                if ((int(mask.at<uchar>(topLeft)) == 255 && int(mask.at<uchar>(topRight)) == 255 && int(mask.at<uchar>(bottomLeft)) == 255 && int(mask.at<uchar>(bottomRight)) == 255) ||
                        (int(mask.at<uchar>(topRight)) == 255 && int(mask.at<uchar>(bottomLeft)) == 255 && int(mask.at<uchar>(bottomRight)) == 255) ||
                        (int(mask.at<uchar>(topLeft)) == 255 && int(mask.at<uchar>(bottomLeft)) == 255 && int(mask.at<uchar>(bottomRight)) == 255) ||
                        (int(mask.at<uchar>(topLeft)) == 255 && int(mask.at<uchar>(topRight)) == 255 && int(mask.at<uchar>(bottomRight)) == 255) ||
                        (int(mask.at<uchar>(topLeft)) == 255 && int(mask.at<uchar>(topRight)) == 255 && int(mask.at<uchar>(bottomLeft)) == 255))

                {
                    blobIsObject = false;
                }
            }

            if (flag_minsize == true)
            {
                int minH = cvCeil((mask.rows * 3) / 100); // 3% size
                int minW = cvCeil((mask.cols * 3) / 100);

                int maxH = cvCeil((mask.rows * 40) / 100); // 3% size
                int maxW = cvCeil((mask.cols * 40) / 100);

                if (staticBlob.h <= minH || staticBlob.w <= minW || staticBlob.h >= maxH || staticBlob.w >= maxW )
                {

                    blobIsObject = false;
                }

            }
            else //FALSE
            {
                int minH = cvCeil((mask.rows * 2) / 100); // 3% size
                int minW = cvCeil((mask.cols * 2) / 100);

                int maxH = cvCeil((mask.rows * 40) / 100); // 3% size
                int maxW = cvCeil((mask.cols * 40) / 100);

                if (staticBlob.h <= minH || staticBlob.w <= minW || staticBlob.h >= maxH || staticBlob.w >= maxW )
                {

                    blobIsObject = false;
                }

            }

            // if (blobIsObject){
            //     pObjectBlob = new ObjectBlob(staticBlob.ID, &staticBlob);
            //     outObjectList->addBlob(pObjectBlob);
            // }

            if (blobIsObject){

                pObjectBlob = new ObjectBlob(staticBlob.ID, &staticBlob);
                if (blobIsAttended == false)
                    pObjectBlob->setAttended(false);
                else
                    pObjectBlob->setAttended(true);

                outObjectList->addBlob(pObjectBlob);
            }
        }
    }

    return;
}


bool CompareBlobs(cvBlob blob, cv::Rect rect){ //blobRect : static , rect: person

    cv::Rect BlobRect(blob.x, blob.y, blob.w, blob.h);
    cv::Rect Blobperson(rect.x -20, rect.y -20, rect.width + 40, rect.height + 40);

    blob.PeopleLikelihood = solape(BlobRect, Blobperson);

    //cv::Rect inters = BlobRect & rect;
    //blob.PeopleLikelihood = 2*inters.area()/(BlobRect.area() + rect.area());

    // std::cout << blob.PeopleLikelihood << std::endl;
    if (blob.PeopleLikelihood > 0.3 && blob.PeopleLikelihood < 2)
        return true;
    else
        return false;


}

double solape(cv::Rect hogRect, cv::Rect bloblistRect){


    int x_solape, y_solape, w_solape, h_solape;

    if(hogRect.contains(bloblistRect.tl()) && hogRect.contains(bloblistRect.br())) //blob completely inside the hog blob
    {
        return (double)1;

    }
    else if(bloblistRect.contains(hogRect.br()) && bloblistRect.contains(hogRect.tl()) )
    {
        return (double)1;
    }
    else if (hogRect.contains(cv::Point(bloblistRect.x, bloblistRect.y)) || hogRect.contains(cv::Point(bloblistRect.x + bloblistRect.width, bloblistRect.y)) || hogRect.contains(cv::Point(bloblistRect.x, bloblistRect.y + bloblistRect.height)) || hogRect.contains(cv::Point(bloblistRect.x + bloblistRect.width, bloblistRect.y + bloblistRect.height))){

        x_solape = std::max(hogRect.x, bloblistRect.x);
        y_solape = std::max(hogRect.y, bloblistRect.y);
        w_solape = abs(std::min(hogRect.x + hogRect.width, bloblistRect.x + bloblistRect.width) - x_solape);
        h_solape = abs(std::min(hogRect.y + hogRect.height, bloblistRect.y + bloblistRect.height) - y_solape);

        cv::Rect rectSolape(x_solape, y_solape, w_solape, h_solape);
        return (double)rectSolape.area() / hogRect.area();

    }



    return 0;
}
