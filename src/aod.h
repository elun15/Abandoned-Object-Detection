#ifndef AOD_H
#define AOD_H

#include "Config.h"
#include "bgs/BGSselector.h"
#include "sfgd/SFGDselector.h"
#include "blobs/BlobExtractor.h"
#include "pd/PDselector.h"
#include "soc/SOCselector.h"
#include "event/EventController.h"

// BGS          --> 1:LOBSTER; 2:PAWCS; 3:MOG2; 4:KNN; 5:Multimodal; 6:SUBSENSE
// SBGS         --> 1:Subsampling; 2:Acc Mask
// CLASSIFIER   --> 1:High Gradient; 2:Histogram
// DETECTOR     --> 1:HOG; 2:DPM; 3:ACF

class AOD
{

public:

	AOD();
   ~AOD();

    FILE *_file_time;

    //system modules
    BGSselector *_sel_bkg;
    BGSselector *_sel_bkg_aux;

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

    void init(Mat frame,Config cfg);
    void processFrame(Mat frame,Config cfg);
};
#endif // AOD_H
