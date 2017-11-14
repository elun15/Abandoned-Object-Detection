#include <opencv2/opencv.hpp>
#include <iostream>
#include "solape.h"


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
