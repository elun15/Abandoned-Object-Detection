#include "CompareBlobs.h"
#include "../../../src/Utils/solape.h"
bool CompareBlobs(cvBlob blob, cv::Rect rect){

	cv::Rect BlobRect(blob.x, blob.y, blob.w, blob.h);

	blob.PeopleLikelihood = solape(BlobRect, rect);
   // std::cout << blob.PeopleLikelihood << std::endl;
	if (blob.PeopleLikelihood > 0.5 && blob.PeopleLikelihood < 2)
		return true;
	else
		return false;


}
