
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "PDselector.h"
#include "../utils/utils.h"
#include "../Config.h"

using namespace std;
using namespace cv;
using namespace cv::dpm;


/*
 * \brief Default constructor
 */
PDselector::PDselector()
{
    cout << "Default parameters:" << endl;
    initialize(PD_HOG, false, false, NULL, -1,true);
}

/*
 * \brief Constructor with selected BGS algorithm
 * \param PDid Identifier for the selected algorithm
 * \param display flag to enable display of results (foreground and background)
 * \param saveIMG flag to enable saving files in a given directory
 * \param saveParDir directory to save files
 * \param saveCounter frequency to save data (number of frames), -1 for none
 */
PDselector::PDselector(PD_type PDid, bool display, bool saveIMG, const char* savePathDir, int saveCounter, bool QT_execution)
{
    initialize(PDid, display, saveIMG, savePathDir, saveCounter,QT_execution);
}

/*
 * \brief Default destructor
 */
PDselector::~PDselector()
{
    std::cout << "~PDselector()" << std::endl;
}

/*
 * \brief Method to initialize variables
 * \param PDid Identifier for the selected algorithm
 * \param display flag to enable display of results (foreground and background)
 * \param saveIMG flag to enable saving files in a given directory
 * \param saveParDir directory to save files
 * \param saveCounter frequency to save data (number of frames), -1 for none
 */
void PDselector::initialize(PD_type PDid, bool display, bool saveIMG, const char* savePathDir, int saveCounter,bool QT_execution)
{
    this->_PDid = PDid;
    this->_PDtype_str =  create_PDtype_str(); //create strings to display the name of each method
    this->_display = display;
    this->_saveIMG = saveIMG;
    this->_savePathDir = savePathDir;
    this->_saveCounter = saveCounter;
    this->_QT_execution = QT_execution;
    cout << "PDselector(): " << _PDtype_str[_PDid] << " selected" << endl;
}


void PDselector::init(){


    switch (_PDid)
    {
    case PD_HOG:
        _pHOG.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
        break;
    case PD_DPM:

        if (this->_QT_execution == true)
            _pDPM  = DPMDetector::create(vector<string>(1, QT_PATH_PERSON_MODEL_DPM));
        else
            _pDPM  = DPMDetector::create(vector<string>(1, PATH_PERSON_MODEL_DPM));

        break;

    case PD_ACF:

        if (this->_QT_execution == true)
            _pACF.ReadModel(QT_PATH_PERSON_MODEL_ACF);
        else
            _pACF.ReadModel(PATH_PERSON_MODEL_ACF);

        break;
    case PD_HAAR_FULL:

        if (this->_QT_execution == true)
            pFullBodyCascade.load(QT_PATH_PERSON_MODEL_HAAR_FULL);
        else
            pFullBodyCascade.load(PATH_PERSON_MODEL_HAAR_FULL);

        break;

    case PD_HAAR_UPPER:

        if (this->_QT_execution == true)
            pUpperBodyCascade.load(QT_PATH_PERSON_MODEL_HAAR_UPPER);
        else
            pUpperBodyCascade.load(QT_PATH_PERSON_MODEL_HAAR_UPPER);


        break;

    default:
        break;
    }
}

/*
 * \brief Method to process each frame by the selected BGS algorithm
 *
 * \param frame video frame in cv::Mat format
 * \param counter frame counter for display purposes
 */
vector<Rect> PDselector::process(Mat frame,int counter)
{
    _found.clear();		//clear list of previous detections
    _img_input = frame.clone(); //copy input frame
    cv::Mat temp = frame.clone(); //copy input frame

    switch (_PDid)
    {
    //HOG detector
    case PD_HOG:
        _pHOG.detectMultiScale(temp, _found, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2);
        break;

        //DPM detector
    case PD_DPM:
        _DPMBbox.clear(); //clear detected Bounding Boxes
        _pDPM->detect(temp, _DPMBbox); // Apply DPM detector with NMS. The function destroys the Frame

        //copy bounding box from DPM's result
        for (size_t i = 0; i < _DPMBbox.size(); i++)
            _found.push_back(_DPMBbox[i].rect);
        break;

        //ACF detector
    case PD_ACF:
        Dl = _pACF.applyDetector(temp); //Apply ACF

        //Apply NMS
        Dl_NMS = NMS.dollarNMS(Dl);

        for ( int i = 0; i < (int)Dl_NMS.Ds.size(); i++)
        {
            Detection a = Dl_NMS.Ds[i];
            float Score = a.getScore();
            Rect detection = Rect (Dl_NMS.Ds[i]->getX(),Dl_NMS.Ds[i]->getY(),Dl_NMS.Ds[i]->getWidth(),Dl_NMS.Ds[i]->getHeight());
            if (detection.x >0 && detection.y >0 && detection.height >0 && detection.width > 0)
            {
                if (Score > 35)
                    _found.push_back(detection);
                // cout <<   Score << endl;
            }
        }
        break;

        //Haar full body
    case PD_HAAR_FULL:
        this->pFullBodyCascade.detectMultiScale(frame, _found, 1.2, 2, 1, cv::Size(0, 0));
        break;


        //Haar upper body
    case PD_HAAR_UPPER:
        this->pUpperBodyCascade.detectMultiScale(frame, _found, 1.2, 2, 1, cv::Size(0, 0));
        break;

    default:
        break;
    }

    if (_display)
    {
        if (counter>0)
            putText(_img_input,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        string str = "People Detection: " + _PDtype_str[_PDid];
        imshow(str.c_str(),getDetectionsBoundingBoxMask(_img_input));
        waitKey(1);
    }


    if (_saveIMG && (counter >=2000 )) //Define criteria
    {

        // putText(_img_input,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        // std::string d1 = _savePathDir + "frame" + to_string_(counter,5) + ".jpg";
        //imwrite(d1,_img_input);

        cv::Mat img = getDetectionsBoundingBoxMask(_img_input);

        // putText(img,to_string_(counter), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        std::string d2 = _savePathDir + "pd" + to_string_(counter,5) + ".jpg";
        imwrite(d2,img);
    }


    //printf("Number of detected people %d \n", this->found.size());
    return _found;
}


void PDselector::non_max_suppresion(const vector<Rect> &srcRects, vector<Rect> &resRects, float thresh)
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

std::vector<cv::Rect> PDselector::getDetections()
{
    return this->_found;
}


/**
*	Returns an image containing the bounding box mask of the blobs previously extracted.
*
* \param frame Image to paint the bounding boxes
*
* \return Bounding Box blob mask
*/
cv::Mat &PDselector::getDetectionsBoundingBoxMask(cv::Mat &frame)
{
    frame.copyTo(_label_image);

    int id=1;

    for (int i = 0; i < (int)this->_found.size(); i++)
    {
        CvPoint pt1 = cvPoint(_found[i].x, _found[i].y);
        CvPoint pt2 = cvPoint(_found[i].x + _found[i].width, _found[i].y + _found[i].height);
        cv::rectangle(_label_image, cv::Rect(pt1, pt2), cv::Scalar(id), 2);
    }

    return _label_image;
}
