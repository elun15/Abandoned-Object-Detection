#-------------------------------------------------
#
# Project created by QtCreator 2017-02-23T09:32:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = System_Evaluation
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    ../../../src/SFGD/StaticMaskExtractor_Subsampling.cpp \
    ../../../src/SFGD/StaticMaskExtractor.cpp \
    ../../../src/SFGD/StaticMaskExtractor_AccMask.cpp \
    ../../../src/SFGD/SFGDSelector.cpp \
    ../../../src/BGS/BkgSubtractionSelector.cpp \
    ../../../lib/BackgroundSubtractorPAWCS.cpp \
    ../../../lib/bgslibrary/package_bgs/db/imbs.cpp \
    ../../../lib/bgslibrary/package_bgs/db/IndependentMultimodalBGS.cpp \
    ../../../lib/bgslibrary/package_bgs/pl/BackgroundSubtractorLBSP.cpp \
    ../../../lib/bgslibrary/package_bgs/pl/BackgroundSubtractorLOBSTER.cpp \
    ../../../lib/bgslibrary/package_bgs/pl/LBSP.cpp \
    ../../../lib/bgslibrary/package_bgs/pl/LOBSTER.cpp \
    ../../../src/BLOBS/BlobExtractor.cpp \
   ../../../src/BLOBS/BasicBlob.cpp \
    ../../../src/Utils/solape.cpp \
    ../../../src/DETECTOR/DefineObjectBlobList.cpp \
    ../../../src/BLOBS/BlobList.cpp \
    ../../../src/BLOBS/CompareBlobs.cpp \
    ../../../src/BLOBS/ObjectBlob.cpp \
    ../../../src/CLASSIFIER/ClassifierSelector.cpp \
    ../../../src/CLASSIFIER/StaticObjectClassifier.cpp \
    ../../../src/CLASSIFIER/StaticObjectClassifierHist.cpp \
    ../../../src/CLASSIFIER/StaticObjectClassifierHighGradient.cpp \
    ../../../src/Utils/DrawDetections.cpp \
    ../../../src/Event/EventController.cpp \
    ../../../src/Event/EventWriter.cpp \
    ../../../src/Event/Event.cpp \
    ../../../lib/DPM/dpm_cascade_detector.cpp \
    ../../../lib/DPM/dpm_cascade.cpp \
    ../../../lib/DPM/dpm_convolution.cpp \
    ../../../lib/DPM/dpm_feature.cpp \
    ../../../lib/DPM/dpm_model.cpp \
    ../../../lib/DPM/dpm_nms.cpp \
    video.cpp \
    ../../../src/DETECTOR/detectorselector.cpp

HEADERS  +=   ../../../src/SFGD/StaticMaskExtractor_subsampling.h \
    ../../../src/SFGD/StaticMaskExtractor.h \
    ../../../src/SFGD/StaticMaskExtractor_AccMask.h \
    ../../../src/SFGD/SFGDSelector.h \
    ../../../src/BGS/BkgSubtractionSelector.h \
    ../../../lib/BackgroundSubtractorPAWCS.h \
    ../../../lib/bgslibrary/package_bgs/db/imbs.hpp \
    ../../../lib/bgslibrary/package_bgs/db/IndependentMultimodalBGS.h \
    ../../../lib/bgslibrary/package_bgs/FrameDifferenceBGS.h \
    ../../../lib/bgslibrary/package_bgs/pl/BackgroundSubtractorLBSP.h \
    ../../../lib/bgslibrary/package_bgs/pl/BackgroundSubtractorLOBSTER.h \
    ../../../lib/bgslibrary/package_bgs/pl/DistanceUtils.h \
    ../../../lib/bgslibrary/package_bgs/pl/LBSP.h \
    ../../../lib/bgslibrary/package_bgs/pl/LOBSTER.h \
    ../../../lib/bgslibrary/package_bgs/pl/RandUtils.h \
    ../../../src/DETECTOR/DetectorSelector.h \
    ../../../src/BLOBS/BlobExtractor.h \
    ../../../src/BLOBS/BasicBlob.h \
    ../../../src/Utils/solape.h \
    ../../../src/DETECTOR/DefineObjectBlobList.h \
    ../../../src/BLOBS/BlobList.h \
    ../../../src/BLOBS/CompareBlobs.h \
    ../../../src/BLOBS/ObjectBlob.h \
    ../../../src/CLASSIFIER/ClassifierSelector.h \
    ../../../src/CLASSIFIER/StaticObjectClassifier.h \
    ../../../src/CLASSIFIER/StaticObjectClassifierHist.h \
    ../../../src/CLASSIFIER/StaticObjectClassifierHighGradient.h \
    ../../../src/Utils/DrawDetections.h \
    ../../../src/Event/EventController.h \
    ../../../src/Event/Event.h \
    ../../../src/Event/EventWriter.h \
    ../../../lib/DPM/dpm_cascade.hpp \
    ../../../lib/DPM/dpm_convolution.hpp \
    ../../../lib/DPM/dpm_feature.hpp \
    ../../../lib/DPM/dpm_model.hpp \
    ../../../lib/DPM/dpm_nms.hpp \
    ../../../lib/DPM/dpm.hpp \
    ../../../lib/DPM/precomp.hpp \
    video.h \
    ../../../src/DETECTOR/detectorselector.h

FORMS    += mainwindow.ui
# The following lines tells Qmake to use pkg-config for opencv
QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += opencv
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/
LIBS += -L/usr/lib/x86_64-linux-gnu/
