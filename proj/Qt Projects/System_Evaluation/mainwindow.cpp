#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTimer>
#include <ctime>
#include <time.h>
#include <QString>
#include <QWidget>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "video.h"
#include "imagewidget.h"
#include <opencv2/highgui/highgui.hpp> //VideoCapture
#include <opencv2/imgproc/imgproc.hpp> //cvtColor
#include <opencv2/opencv.hpp> //opencv libraries

#include "../../../src_interface/Utils/DrawDetections.h"
#include "../../../lib/bgslibrary/package_bgs/FrameDifferenceBGS.h"
#include "../../../src_interface/BGS/BkgSubtractionSelector.h"
#include "../../../src_interface/SFGD/SFGDSelector.h"
#include "../../../src_interface/DETECTOR/detectorselector.h"
#include "../../../src_interface/DETECTOR/DefineObjectBlobList.h"
#include "../../../src_interface/CLASSIFIER/ClassifierSelector.h"
#include "../../../src_interface/Event/EventController.h"

//Global video capture
video Video;

//Global pointers
BkgSubtractionSelector *bkg_selector;
SFDGSelector *sfgd_selector;
DetectorSelector *detec_selector;
BlobExtractor blob_extractor;
ClassifierSelector *classifier_selector;



//GLobal Interface variables
bool interfaceDisplay = true;
using namespace cv;

//ClassifierSelector classifier_selector(classifier_method);

void on_MouseClick(int evt, int x, int y, int, void*)
{
    if(evt == CV_EVENT_LBUTTONDOWN) {
        Point pt = Point(x,y);
        cout << "Camera 1 Frame x = " << pt.x << " y = " << pt.y << endl;
        Video.maskPoints.push_back(pt);
    }
}



MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Mat frame;
    Mat tmp;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainProcess(){

    // EventController* evtControl;
    // evtControl= new EventController(frame,sourceFile, resultsFile, Video.totalNumFrames,Video.framerate, Video.time_to_static, 0);

    // Start the clock for measuring frame consumption time
    clock_t start = clock();
    Mat frame;
    Video.cap >> frame;
    Video.numFrame = Video.cap.get(CAP_PROP_POS_FRAMES);


    if(Video.ResizeFrame == true)
    {
        //    cv::resize(frame, frame, Size(),0.5,0.5);
        Video.ResizeFrame = false;
    }

    if (frame.data)
    {
        if (Video.numFrame  == 1)
        {

            //Initializations
            bkg_selector = new BkgSubtractionSelector(Video.bkg_method);
            bkg_selector->init(frame);
            sfgd_selector = new SFDGSelector(Video.sfgd_method,Video.framerate,Video.time_to_static);

            sfgd_selector->init(frame);
            detec_selector= new DetectorSelector(Video.detector_method);
            detec_selector->init();
            classifier_selector = new ClassifierSelector(Video.classifier_method);
            classifier_selector->init();
            Video.list_objects = new BlobList<ObjectBlob*>();

            /****CONTEXT MASK SELECTION*****/
            if (ui->ContextMaskButton->isChecked())
            {

                Video.contextMask = Mat::zeros(frame.rows,frame.cols,CV_8UC1);
                bool exit = false;

                while(exit == false){

                    String windowName = "Select mask points";
                    namedWindow(windowName);
                    setMouseCallback(windowName, on_MouseClick, 0);
                    imshow(windowName, frame);
                    if (waitKey() == 27)
                    {
                        exit = true;
                        //destroyWindow(windowName);
                        //destroyAllWindows();

                    }

                }
                fillConvexPoly(Video.contextMask,Video.maskPoints,Scalar(255,255,255));

                //threshold(Video.contextMask,Video.contextMask,250,255,CV_THRESH_BINARY);
                imshow("Context Mask", Video.contextMask);
                imwrite( "/Users/Elena/TFM/images/AVSS_EASY_Mask.jpg", Video.contextMask );
                waitKey();



            }

        }

        if (Video.numFrame  == 4500)
        {
            int pr = 1;
        }
        clock_t bkg1 = clock();
        //Execute Background Subtraction
        bkg_selector->process(frame,Video);


        //Execute Static Foreground Detection
        sfgd_selector->process(frame,bkg_selector->GetForegroundImage(),bkg_selector->GetBGModel(),Video.ShowResults);

        // Extract blobs & filter the mask

        Mat tmp = sfgd_selector->GetStaticForeground().clone();
        Mat ch[3];
        split(tmp, ch);
        tmp = ch[0];

        blob_extractor.extractBlobs(tmp,false);

        vector<cvBlob> *BlobList = blob_extractor.getBlobList(); //All blobs in static foreground


        if (Video.DetectPeopleAlways == true || (Video.DetectPeopleAlways == false && BlobList->size()) )
        {

            //Execute People Detection
            detec_selector->process(frame);

            //Draw (and show if required) static people in the frame
            detec_selector->non_max_suppresion(detec_selector->found,Video.found_filtered,1);
            Mat peopleDetection = DrawDetections(frame,Video.found_filtered, BlobList, Video);
            peopleDetection = resizeImageToWidget(peopleDetection,ui->widgetPeopleDetection);
            ui->widgetPeopleDetection->showImage(peopleDetection);
            //Create a blob list with just the objects in the scene
            DefineObjectBlobList(BlobList, Video.found_filtered, Video.list_objects,Video.contextMask);

            cout << "Frame " << Video.numFrame << ". Num objects blobs = " << Video.list_objects->getBlobNum() <<  ".  Number of people detected: " << detec_selector->found.size() << endl;

        }


        string s = "Frame " + to_string(Video.numFrame) + ". Number of static blobs = " + to_string(blob_extractor.getNumBlobs()) + ".   Number of static objects: " + to_string(Video.list_objects->getBlobNum());
        QString Qstr = QString::fromStdString(s);
        ui->textBrowser->append(Qstr);


        Mat clss;
        //Classification (only if some static object is detected)
        if (Video.list_objects->getBlobNum()){

            clss = classifier_selector->process(frame, bkg_selector->GetBGModel(), tmp, bkg_selector->GetForegroundImage(),Video.list_objects, Video.ShowResults);

        }


        if (Video.SaveResults == true)
        {
            //*************** CHECK EVENTS ******************
            Mat tmp2 = sfgd_selector->GetStaticForeground().clone();
            Mat ch2[3];
            split(tmp2, ch2);
            tmp2 = ch2[0];

            Video.evtControl->checkEvents(frame, bkg_selector->GetBGModel(),tmp2,  bkg_selector->GetForegroundImage(), Video.list_objects,Video.ResizeFrame);

            //*************** CHECK EVENTS ******************
        }

        if (interfaceDisplay)
        {
            Mat frameCopy = frame.clone();
            frameCopy = resizeImageToWidget(frameCopy,ui->widgetFrame);
            putText(frameCopy,to_string(Video.numFrame), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0),1.5);

            ui->widgetFrame->showImage(frameCopy);

            Mat bkg = resizeImageToWidget(bkg_selector->GetBGModel(),ui->widgetBackground);
            ui->widgetBackground->showImage(bkg);

            Mat fg = resizeImageToWidget(bkg_selector->GetForegroundImage(),ui->widgetForeground);
            ui->widgetForeground->showImage(fg);

            Mat sfg = resizeImageToWidget(sfgd_selector->GetStaticForeground(),ui->widgetStaticForeground);
            ui->widgetStaticForeground->showImage(sfg);

            /* if (BlobList->size())
            {

            Mat peopleDetection = resizeImageToWidget(peopleDetection,ui->widgetPeopleDetection);
            ui->widgetPeopleDetection->showImage(peopleDetection);
            }

            else
            {
                ui->widgetPeopleDetection->showImage(frame);
            }*/

            if (Video.list_objects->getBlobNum())
            {
                clss = resizeImageToWidget(clss, ui->widgetClassification);
                ui->widgetClassification->showImage(clss);
            }
            /* else
            {
                ui->widgetClassification->showImage(frame);
            }*/



        }
        Video.list_objects->clear();

    }
    else
    {

        //WRITE EVENTS IN FILE
        if (Video.SaveResults == true)
        {
            if( ((int)Video.evtControl->pActiveEvt.size()) !=  0 )
            {

                Video.evtControl->pwriter.writeEvents(&Video.evtControl->pActiveEvt, Video.ResizeFrame);


            }
        }

        Video.cap.release();
        cout << "Video has finished" << endl;
        Timer->blockSignals(true);
        return;

    }

    clock_t finish = clock();
    double elapsedTime = (double)(finish - start)/CLOCKS_PER_SEC;

    double fpsProcessed = 1/elapsedTime;





}

Mat  MainWindow::resizeImageToWidget(Mat image, ImageWidget* widget){
    int height = widget->height();
    int width = widget->width();
    cv::resize(image,image, {width, height});
    return image;
}

void MainWindow::on_PauseButton_clicked()
{
    if (ui->PauseButton->isChecked())
    {
        ui->textBrowser->append("Paused");
        Timer->blockSignals(true);
    }
    else
    {
        ui->textBrowser->append("Resumed");

        Timer->blockSignals(false);
    }

}

void MainWindow::on_actionOpen_triggered()
{


    //Video files directory
    QString file = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/Users/Elena/PruebasTFM/videos/",
                "Video Files (*.mov *.mpg *.avi *.mpeg *.mp4)"

                );

    Video.fileDir = file.toStdString();

    Video.cap.open(Video.fileDir);
    Video.framerate = Video.cap.get(CAP_PROP_FPS);
    Video.totalNumFrames = Video.cap.get(CAP_PROP_FRAME_COUNT);
    Video.rows = Video.cap.get(CAP_PROP_FRAME_HEIGHT);
    Video.cols = Video.cap.get(CAP_PROP_FRAME_WIDTH);

    if (!Video.cap.isOpened()) //Comprobamos si hemos iniciado el Videocapture correctamente.
    {
        cout << "Could not open the video file" << endl;
        return;
    }
    else
    {
        cout << "Video successfully opened" << endl;
        cout << Video.framerate << " fps. Size: " << Video.cap.get(CV_CAP_PROP_FRAME_WIDTH) << " x " << Video.cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    }


    Video.bkg_method = ui->comboBoxBKG->currentIndex() + 1; //Starts in 0
    Video.sfgd_method = ui->comboBoxSFGD->currentIndex() + 1;
    Video.classifier_method = ui->comboBoxClassifier->currentIndex() + 1;
    Video.detector_method = ui->comboBoxPD->currentIndex() + 1;
    Video.time_to_static = ui->spinBoxSecs->value();
    Video.ResizeFrame = false;

    if (Video.cap.get(CV_CAP_PROP_FRAME_WIDTH) > 400){
        Video.ResizeFrame = true;}

    Video.SaveResults = false;

    if (ui->SaveResultsButton->isChecked())
    {
        Video.SaveResults = true;

        QFileInfo finfo(file);
        QString filename = finfo.fileName();

        //Video.fileResults = "/Users/Elena/TFM/proj/Qt Projects/System/Results/" + filename.toStdString() + "_" + currentDateTime() + ".xml";
         Video.fileResults = "/home/vpu/Desktop/TFM_GitHub/TFM/proj/Qt Projects/System/Results/" + filename.toStdString() + "_" + currentDateTime() + ".xml";

        cout << "currentDateTime()=" << currentDateTime() << endl;
        cout << "File results: " << Video.fileResults << endl;

        /******** EVENT FILE INITIALIZATION ********/
        //To const char* conversion
        char *sourceFile = new char[Video.fileDir.length() + 1];
        strcpy(sourceFile, Video.fileDir.c_str());

        //To const char* conversion
        char *resultsFile = new char[ Video.fileResults.length() + 1];
        strcpy(resultsFile,  Video.fileResults.c_str());

        Video.evtControl = new EventController();

        Video.evtControl->init(Video.rows, Video.cols ,sourceFile, resultsFile, Video.totalNumFrames,Video.framerate, Video.time_to_static, 0);
        /******** EVENT FILE INITIALIZATION ********/



    }

    if (ui->PeopleDetectionButton->isChecked()){
        Video.DetectPeopleAlways = true;
        cout << "Detecting people in every frame." << endl;
    }
    else{
        Video.DetectPeopleAlways = false;
        cout << "Detecting people only when something static is detected." << endl;
    }

    // Timer to launch the mainProcess() slot
    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(mainProcess()));
    Timer->start();
}

const string MainWindow::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}



