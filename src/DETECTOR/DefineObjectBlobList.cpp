#include "DefineObjectBlobList.h"
#include "video.h"
void DefineObjectBlobList(std::vector<cvBlob> *ExtractBlobList, vector<cv::Rect> found, BlobList<ObjectBlob*> *pObjectList,Mat mask){


    // INPUT ARGUMENTS
    // ExtractBlobList =>  all blobs in static foreground
    // found => people detections

    // OUTPUT ARGUMENTS
    // pObjectList  => list with the static objects in the scene


    ObjectBlob *pObjectBlob;
    bool blobIsObject = true;



    if (!ExtractBlobList->empty()){

        for (cvBlob staticBlob : *ExtractBlobList) //loop through static foreground blobs
        {
            blobIsObject = true;


            Point2f static_bottom(staticBlob.y+staticBlob.h, staticBlob.x+(staticBlob.w/2));


            for (cv::Rect personBlob : found){ //loop through detected people

                if (CompareBlobs(staticBlob, personBlob)){
                    blobIsObject = false; //at least 50% of being a person
                }

                //Check if it is near a person
                Point2f people_bottom(personBlob.y+personBlob.height,personBlob.x+(personBlob.width/2));
                double distance = cv::norm(cv::Mat(static_bottom),cv::Mat(people_bottom));

                if (distance  < 2*staticBlob.w)
                {
                    blobIsObject = false;

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

            if (blobIsObject){
                pObjectBlob = new ObjectBlob(staticBlob.ID, &staticBlob);
                pObjectList->addBlob(pObjectBlob);
            }
        }
    }

    return;
}
