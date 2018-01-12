#include <opencv2/opencv.hpp>
#include "aod.h"
#include "blobs/DefineObjectBlobList.h"

AOD::AOD()
{
    _file_time = NULL;
    _sel_bkg = NULL;
    _sel_bkg_2 = NULL;
    _sel_sfgd = NULL;
    _sel_pd = NULL;
    _sel_soc = NULL;
    _evtCtrl = NULL;
}

AOD::~AOD()
{
    destroyAllWindows();
    cout << "Destroyed all windows." << endl;

    if (_file_time != NULL)
        fclose(_file_time);
    cout << "Closed time file." << endl;

    // check events for last time
    _evtCtrl->checkFinalPastEvents();

    // release modules
    if (this->_sel_bkg!=NULL)		delete _sel_bkg;
    if (this->_sel_bkg_2!=NULL)     delete _sel_bkg_2;
    if (this->_sel_sfgd!=NULL)		delete _sel_sfgd;
    if (this->_sel_pd!=NULL)		delete _sel_pd;
    if (this->_sel_soc!=NULL)		delete _sel_soc;
    if (this->_evtCtrl!=NULL)		delete _evtCtrl;
    cout << "System modules released." << endl;
}

void AOD::init(Mat frame, Config &cfg)
{
    // Initializations
    this->_sel_bkg = new BGSselector((BGS_type)cfg.m_bkg,cfg.ShowResults,cfg.SaveImages,cfg.DirImages.c_str(),cfg.SaveImages_freq);


    if (cfg.m_sfgd == SFGD_DBM)
    {
        double learningRate_s=-1,learningRate_l = -1,learningRate_m = -1;

        switch(cfg.m_bkg)
        {
        case BGS_MOG2:

            learningRate_l = 0.00009;
            break;
        case BGS_KNN:

            learningRate_l = 0.00009;
            break;
                  break;

        }

        this->_sel_bkg->init(frame, learningRate_s, learningRate_l,learningRate_m, cfg.m_sfgd);
    }
    else if (cfg.m_sfgd == SFGD_TBM)
    {
        double learningRate_s=-1,learningRate_l = -1, learningRate_m = -1;

        switch(cfg.m_bkg)
        {

        case BGS_MOG2:
            learningRate_s = 0.002; // only used to compute medium rate, default for execution
            learningRate_l = 0.00009;//0.00005
            learningRate_m = learningRate_s + ((learningRate_l-learningRate_s)/2);
            break;
        case BGS_KNN:
            learningRate_s = 0.002; // only used to compute medium rate
            learningRate_l = 0.00009;
            learningRate_m = learningRate_s + ((learningRate_l-learningRate_s)/2);

            break;


        }

        this->_sel_bkg->init(frame, learningRate_s, learningRate_l,learningRate_m, cfg.m_sfgd);

    }
    else{

        this->_sel_bkg->init(frame);
    }

    this->_sel_sfgd = new SFDGselector((SFGD_type)cfg.m_sfgd,cfg.framerate,cfg.time_to_static,cfg.ShowResults,cfg.SaveImages,cfg.DirImages.c_str(),cfg.SaveImages_freq);
    this->_sel_sfgd->init(frame);

    this->_sel_pd = new PDselector((PD_type)cfg.m_pd,cfg.ShowResults,cfg.SaveImages,cfg.DirImages.c_str(),cfg.SaveImages_freq, cfg.QT_execution);
    this->_sel_pd->init();

    this->_sel_soc = new SOCselector((SOC_type)cfg.m_soc,cfg.ShowResults,cfg.SaveImages,cfg.DirImages.c_str(),cfg.SaveImages_freq);
    this->_sel_soc->init();

    this->_evtCtrl = new EventController();

    //To const char* conversion (video file)
    char *sourceFile = new char[cfg.inputPath.length() + 1];
    strcpy(sourceFile, cfg.inputPath.c_str());

    if (cfg.SaveResults == true)
    {
        /******** EVENT FILE INITIALIZATION ********/

        cout << "Saving results and execution times." << endl;

        //To const char* conversion (results file)
        char *resultsFile = new char[ cfg.fileResults.length() + 1];
        strcpy(resultsFile,  cfg.fileResults.c_str());

        _evtCtrl->init(cfg.rows, cfg.cols ,sourceFile, resultsFile, cfg.totalNumFrames,cfg.framerate, cfg.time_to_static, 0);

        // Time file initialization
        char *pfile = (char *)malloc(strlen(cfg.fileTime.c_str()) + 1);
        strcpy(pfile, cfg.fileTime.c_str());
        _file_time = fopen(pfile, "w+");

        fprintf( _file_time,"#Execution times (secs) \n");
        fprintf( _file_time,"#Frame   Total           BKG             SFGD               PD            CLASS          WRITE\n");


        //settings paratemers file writting

        char *pfile2 = (char *)malloc(strlen(cfg.fileSettingsPath.c_str()) + 1);
        strcpy(pfile2, cfg.fileSettingsPath.c_str());
        _file_settings = fopen(pfile2, "w+");

        if (_file_settings == NULL)
            cout << "WARNING: settings file can't be created: " << cfg.fileSettingsPath << endl;
        else
            cout << "Settings file created" << endl;


        fprintf( _file_settings,"#Configuration settings \n");
        fprintf( _file_settings, "Selected bkg method = %d\n",cfg.m_bkg) ;
        fprintf( _file_settings, "Selected sfgd method= %d\n" ,cfg.m_sfgd);
        fprintf( _file_settings, "Selected people detector method = %d\n",cfg.m_pd);
        fprintf( _file_settings,"Selected static object classifier method = %d\n",cfg.m_soc);
        fprintf( _file_settings,"Time (seconds) to static = %d\n",cfg.time_to_static);
        fprintf( _file_settings,"Flag near people = %d\n",cfg.flag_nearpeople);
        fprintf( _file_settings,"Flag context mask = %d\n",cfg.flag_contextmask);

        fclose(_file_settings);


    }
    else
        _evtCtrl->init(cfg.rows, cfg.cols ,sourceFile, NULL, cfg.totalNumFrames,cfg.framerate, cfg.time_to_static, 0);

    if ( cfg.contextMask.empty() )
        cfg.contextMask = Mat::zeros(frame.rows, frame.cols, 0);



}

void AOD::processFrame(Mat frame,Config cfg)
{

    Mat fgmask, fgmask_2, bgmodel, sfgmask;
    BlobList<ObjectBlob*> pStaticObjectList;


    /******** BACKGROUND SUBTRACTION  ********/
    clock_t tinit = clock();

    _sel_bkg->process(frame,cfg.AVSS_FGMask,cfg.numFrame, cfg.m_sfgd,cfg.numFrame);
    bgmodel = _sel_bkg->GetBGmodel().clone(); // Get background model

    _elapsedTime_bkg = (double)(clock() - tinit)/CLOCKS_PER_SEC;
    cout << "tiempo " <<_elapsedTime_bkg << endl;

    // ******** STATIC FOREGROUND DETECTION  ******
    clock_t start_sfgd = clock();
    _sel_sfgd->process(frame, _sel_bkg->GetFGmask(cfg.m_sfgd), bgmodel,cfg.numFrame);
    sfgmask = _sel_sfgd->GetSFGmask().clone(); // Get static foreground mask
    _elapsedTime_sfgd = (double)(clock() - start_sfgd)/CLOCKS_PER_SEC;

    // ******** BLOB EXTRACTION  ******
    // Extract all blobs in static foreground
    _blob_extractor.extractBlobs(sfgmask,false); //extract blobs from foreground mask

    // Create BlobList containing all blobs in static foreground
    vector<cvBlob> *BlobList = _blob_extractor.getBlobList();
    // imshow("BLOBS",_blob_extractor.getBlobBoundingBoxMask(frame));
    //waitKey(10);

    // ******* PEOPLE DETECTION  *******
    std::vector<cv::Rect> pdBlobs;        //
    // Detecting people in every frame or when something static has been detected (BlobList !empty)
    if (cfg.DetectPeopleAlways == true || (cfg.DetectPeopleAlways == false && BlobList->size()) )
    {
        clock_t start_pd = clock();
        pdBlobs = this->_sel_pd->process(frame); //apply detector

        //this->_sel_pd->non_max_suppresion(_sel_pd->getDetections(),pdBlobs,0.5); // Filter detections with nms
        _elapsedTime_pd = (double)(clock() - start_pd)/CLOCKS_PER_SEC;
    }

    // ******* FILTER BLOBS WITH PEOPLE DETECTION  *******
    // Create a static objects list (pStaticObjectList) by filtering the static foreground blobs (BlobList)
    // by removing static foreground blobs due to people detections (pdBlobs) and considering
    // the context mask (if applicable)
    DefineObjectBlobList(BlobList,pdBlobs,&pStaticObjectList, cfg.contextMask, cfg.flag_minsize, cfg.flag_nearpeople, cfg.flag_stillpeople);
    cout << "Frame " << cfg.numFrame << ". Num blobs=" << BlobList->size() <<  "  people=" << pdBlobs.size() << "  static=" << pStaticObjectList.getBlobNum() << std::endl;

    // ******* STATIONARY OBJECT CLASSIFICATION  *******
    clock_t start_soc = clock();

    if (pStaticObjectList.getBlobNum())
        this->_sel_soc->process(frame, bgmodel, sfgmask, sfgmask,&pStaticObjectList);

    _elapsedTime_soc = (double)(clock()-start_soc)/CLOCKS_PER_SEC;

    // ******** EVENT ANALYSIS/HANDLING   ********
    clock_t start_write = clock();
    // Check events
    if (cfg.SaveResults == true)
        _evtCtrl->checkEvents(&pStaticObjectList);
    _elapsedTime_evt = (double)(clock() - start_write)/CLOCKS_PER_SEC;

    _elapsedTime_frame = (double)(clock() - tinit)/CLOCKS_PER_SEC;

    // ******** TIME CONSUPTION WRITTING (For each frame) ********
    if (cfg.SaveResults == true)
        fprintf(_file_time,"%d\t%2.6f\t%2.6f\t%2.6f\t%2.6f\t%2.6f\t%2.6f\n",cfg.numFrame, _elapsedTime_frame ,_elapsedTime_bkg,_elapsedTime_sfgd,_elapsedTime_pd,_elapsedTime_soc,_elapsedTime_evt);



}
