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
    ../../../src/DETECTOR/detectorselector.cpp \
    ../../../lib/bgslibrary/package_bgs/KNN.cpp \
    ../../../lib/bgslibrary/package_bgs/IndependentMultimodal.cpp \
    ../../../lib/bgslibrary/package_bgs/LOBSTER.cpp \
    ../../../lib/bgslibrary/package_bgs/MixtureOfGaussianV2.cpp \
    ../../../lib/bgslibrary/package_bgs/PAWCS.cpp \
    ../../../lib/bgslibrary/package_bgs/SuBSENSE.cpp \
    ../../../lib/bgslibrary/package_bgs/IMBS/IMBS.cpp \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorLOBSTER.cpp \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorPAWCS.cpp \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorSuBSENSE.cpp \
    ../../../lib/bgslibrary/package_bgs/LBSP/LBSP_.cpp \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorLBSP.cpp \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorLBSP_.cpp \
    ../../../lib/bgslibrary/package_bgs/LBSP/LBSP.cpp \
    ../../../lib/ACF/ACFDetector.cpp \
    ../../../lib/ACF/ACFFeaturePyramid.cpp \
    ../../../lib/ACF/Channel.cpp \
    ../../../lib/ACF/ChannelFeatures.cpp \
    ../../../lib/ACF/ColorChannel.cpp \
    ../../../lib/ACF/convConst.cpp \
    ../../../lib/ACF/GradHistChannel.cpp \
    ../../../lib/ACF/gradientMex.cpp \
    ../../../lib/ACF/GradMagChannel.cpp \
    ../../../lib/ACF/wrappers.cpp \
    ../../../lib/Channel/Convert.cpp \
    ../../../lib/Core/detection.cpp \
    ../../../lib/Core/DetectionList.cpp \
    ../../../lib/Core/detector.cpp \
    ../../../lib/Core/DetectorManager.cpp \
    ../../../lib/Core/detectormodel.cpp \
    ../../../lib/Core/feature.cpp \
    ../../../lib/Core/featurelayer.cpp \
    ../../../lib/Core/filelocator.cpp \
    ../../../lib/Core/FileWriter.cpp \
    ../../../lib/Core/FrameFromCamera.cpp \
    ../../../lib/Core/FrameFromDirectory.cpp \
    ../../../lib/Core/FrameFromVideo.cpp \
    ../../../lib/Core/FrameProducer.cpp \
    ../../../lib/Core/nms.cpp \
    ../../../lib/Core/NonMaximumSuppression.cpp \
    ../../../lib/Core/ScaleSpacePyramid.cpp

HEADERS  +=   ../../../src/SFGD/StaticMaskExtractor_subsampling.h \
    ../../../src/SFGD/StaticMaskExtractor.h \
    ../../../src/SFGD/StaticMaskExtractor_AccMask.h \
    ../../../src/SFGD/SFGDSelector.h \
    ../../../src/BGS/BkgSubtractionSelector.h \
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
    ../../../src/DETECTOR/detectorselector.h \
    ../../../lib/bgslibrary/package_bgs/KNN.h \
    ../../../lib/bgslibrary/package_bgs/bgslibrary.h \
    ../../../lib/bgslibrary/package_bgs/IndependentMultimodal.h \
    ../../../lib/bgslibrary/package_bgs/LOBSTER.h \
    ../../../lib/bgslibrary/package_bgs/MixtureOfGaussianV2.h \
    ../../../lib/bgslibrary/package_bgs/PAWCS.h \
    ../../../lib/bgslibrary/package_bgs/SuBSENSE.h \
    ../../../lib/bgslibrary/package_bgs/IBGS.h \
    ../../../lib/bgslibrary/package_bgs/IMBS/IMBS.hpp \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorLOBSTER.h \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorPAWCS.h \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorSuBSENSE.h \
    ../../../lib/bgslibrary/package_bgs/LBSP/DistanceUtils.h \
    ../../../lib/bgslibrary/package_bgs/LBSP/RandUtils.h \
    ../../../lib/bgslibrary/package_bgs/LBSP/LBSP_.h \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorLBSP.h \
    ../../../lib/bgslibrary/package_bgs/LBSP/BackgroundSubtractorLBSP_.h \
    ../../../lib/bgslibrary/package_bgs/LBSP/LBSP.h \
    ../../../lib/ACF/ACFDetector.h \
    ../../../lib/ACF/ACFFeaturePyramid.h \
    ../../../lib/ACF/Channel.h \
    ../../../lib/ACF/ChannelFeatures.h \
    ../../../lib/ACF/ColorChannel.h \
    ../../../lib/ACF/GradHistChannel.h \
    ../../../lib/ACF/GradMagChannel.h \
    ../../../lib/ACF/imResampleMex.hpp \
    ../../../lib/ACF/rgbConvertMex.hpp \
    ../../../lib/ACF/sse.hpp \
    ../../../lib/ACF/wrappers.hpp \
    ../../../lib/Channel/Functions.h \
    ../../../lib/Core/detection.h \
    ../../../lib/Core/DetectionList.h \
    ../../../lib/Core/detector.h \
    ../../../lib/Core/DetectorManager.h \
    ../../../lib/Core/detectormodel.h \
    ../../../lib/Core/dirent.h \
    ../../../lib/Core/feature.h \
    ../../../lib/Core/featurelayer.h \
    ../../../lib/Core/filelocator.h \
    ../../../lib/Core/FileWriter.h \
    ../../../lib/Core/FrameFromCamera.h \
    ../../../lib/Core/FrameFromDirectory.h \
    ../../../lib/Core/FrameFromVideo.h \
    ../../../lib/Core/FrameProducer.h \
    ../../../lib/Core/Image.hpp \
    ../../../lib/Core/nms.h \
    ../../../lib/Core/NonMaximumSuppression.h \
    ../../../lib/Core/ScaleSpacePyramid.h \
    ../../../lib/rapidxml-1.13/rapidxml_iterators.hpp \
    ../../../lib/rapidxml-1.13/rapidxml_print.hpp \
    ../../../lib/rapidxml-1.13/rapidxml_utils.hpp \
    ../../../lib/rapidxml-1.13/rapidxml.hpp

FORMS    += mainwindow.ui
# The following lines tells Qmake to use pkg-config for opencv
QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += opencv
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/
LIBS += -L/usr/lib/x86_64-linux-gnu/

DISTFILES += \
    ../../../lib/ACF/INRIA_ACF.xml \
    ../../../lib/rapidxml-1.13/license.txt
