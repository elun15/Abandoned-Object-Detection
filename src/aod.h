#ifndef AOD_H
#define AOD_H

#include "Config.h"
#include "bgs/BGSselector.h"
#include "sfgd/SFGDselector.h"
#include "blobs/BlobExtractor.h"
#include "pd/PDselector.h"
#include "soc/SOCselector.h"
#include "event/EventController.h"

// BGS          --> 1:LOBSTER; 2:PAWCS; 3:MOG2; 4:KNN; 5:Multimodal; 6:SUBSENSE; 7:KDE
// SBGS         --> 1:Subsampling; 2:Acc Mask ; 3:History Images ; 4:Dual BKG
// CLASSIFIER   --> 1:High Gradient; 2:Histogram; 3:PixelColorContrast
// DETECTOR     --> 1:HOG; 2:DPM; 3:ACF; 4:Haar Fullbody; 5:Haar Upperbody

class AOD
{

public:

	AOD();
   ~AOD();

    FILE *_file_time;

    //system modules
    BGSselector *_sel_bkg;
    BGSselector *_sel_bkg_2;

    SFDGselector *_sel_sfgd;
    PDselector   *_sel_pd;
    SOCselector  *_sel_soc;
    BlobExtractor _blob_extractor;
    EventController *_evtCtrl;

    double _elapsedTime_bkg;
    double _elapsedTime_sfgd;
    double _elapsedTime_pd;
    double _elapsedTime_soc;
    double _elapsedTime_frame;
    double _elapsedTime_evt;

    void init(Mat frame, Config &cfg);
    void processFrame(Mat frame, Config cfg);
};
#endif // AOD_H
