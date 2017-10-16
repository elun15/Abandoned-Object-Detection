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
    ../../../lib/ACFlibrary/ACF/ACFDetector.cpp \
    ../../../lib/ACFlibrary/ACF/ACFFeaturePyramid.cpp \
    ../../../lib/ACFlibrary/ACF/Channel.cpp \
    ../../../lib/ACFlibrary/ACF/ChannelFeatures.cpp \
    ../../../lib/ACFlibrary/ACF/ColorChannel.cpp \
    ../../../lib/ACFlibrary/ACF/convConst.cpp \
    ../../../lib/ACFlibrary/ACF/GradHistChannel.cpp \
    ../../../lib/ACFlibrary/ACF/gradientMex.cpp \
    ../../../lib/ACFlibrary/ACF/GradMagChannel.cpp \
    ../../../lib/ACFlibrary/ACF/wrappers.cpp \
    ../../../lib/ACFlibrary/Channel/Convert.cpp \
    ../../../lib/ACFlibrary/Core/detection.cpp \
    ../../../lib/ACFlibrary/Core/DetectionList.cpp \
    ../../../lib/ACFlibrary/Core/detector.cpp \
    ../../../lib/ACFlibrary/Core/DetectorManager.cpp \
    ../../../lib/ACFlibrary/Core/detectormodel.cpp \
    ../../../lib/ACFlibrary/Core/feature.cpp \
    ../../../lib/ACFlibrary/Core/featurelayer.cpp \
    ../../../lib/ACFlibrary/Core/filelocator.cpp \
    ../../../lib/ACFlibrary/Core/FileWriter.cpp \
    ../../../lib/ACFlibrary/Core/FrameFromCamera.cpp \
    ../../../lib/ACFlibrary/Core/FrameFromDirectory.cpp \
    ../../../lib/ACFlibrary/Core/FrameFromVideo.cpp \
    ../../../lib/ACFlibrary/Core/FrameProducer.cpp \
    ../../../lib/ACFlibrary/Core/nms.cpp \
    ../../../lib/ACFlibrary/Core/NonMaximumSuppression.cpp \
    ../../../lib/ACFlibrary/Core/ScaleSpacePyramid.cpp \
    ../../../src/Utils/CurrentDateTime.cpp \
    ../../../src/aod.cpp \
    ../../../src/settings.cpp \
    ../../../src/BGS/DualBkgSubtractionSelector.cpp \
    ../../../src/SFGD/StaticMaskExtractor_DualBkg.cpp


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
    ../../../lib/ACFlibrary/ACF/ACFDetector.h \
    ../../../lib/ACFlibrary/ACF/ACFFeaturePyramid.h \
    ../../../lib/ACFlibrary/ACF/Channel.h \
    ../../../lib/ACFlibrary/ACF/ChannelFeatures.h \
    ../../../lib/ACFlibrary/ACF/ColorChannel.h \
    ../../../lib/ACFlibrary/ACF/GradHistChannel.h \
    ../../../lib/ACFlibrary/ACF/GradMagChannel.h \
    ../../../lib/ACFlibrary/ACF/imResampleMex.hpp \
    ../../../lib/ACFlibrary/ACF/rgbConvertMex.hpp \
    ../../../lib/ACFlibrary/ACF/sse.hpp \
    ../../../lib/ACFlibrary/ACF/wrappers.hpp \
    ../../../lib/ACFlibrary/Channel/Functions.h \
    ../../../lib/ACFlibrary/Core/detection.h \
    ../../../lib/ACFlibrary/Core/DetectionList.h \
    ../../../lib/ACFlibrary/Core/detector.h \
    ../../../lib/ACFlibrary/Core/DetectorManager.h \
    ../../../lib/ACFlibrary/Core/detectormodel.h \
    ../../../lib/ACFlibrary/Core/dirent.h \
    ../../../lib/ACFlibrary/Core/feature.h \
    ../../../lib/ACFlibrary/Core/featurelayer.h \
    ../../../lib/ACFlibrary/Core/filelocator.h \
    ../../../lib/ACFlibrary/Core/FileWriter.h \
    ../../../lib/ACFlibrary/Core/FrameFromCamera.h \
    ../../../lib/ACFlibrary/Core/FrameFromDirectory.h \
    ../../../lib/ACFlibrary/Core/FrameFromVideo.h \
    ../../../lib/ACFlibrary/Core/FrameProducer.h \
    ../../../lib/ACFlibrary/Core/Image.hpp \
    ../../../lib/ACFlibrary/Core/nms.h \
    ../../../lib/ACFlibrary/Core/NonMaximumSuppression.h \
    ../../../lib/ACFlibrary/Core/ScaleSpacePyramid.h \
    ../../../lib/ACFlibrary/rapidxml-1.13/rapidxml_iterators.hpp \
    ../../../lib/ACFlibrary/rapidxml-1.13/rapidxml_print.hpp \
    ../../../lib/ACFlibrary/rapidxml-1.13/rapidxml_utils.hpp \
    ../../../lib/ACFlibrary/rapidxml-1.13/rapidxml.hpp \
    ../../../src/Utils/CurrentDateTime.h \
    ../../../src/aod.h \
    ../../../src/settings.h \
    ../../../src/BGS/DualBkgSubtractionSelector.h \
    ../../../src/SFGD/StaticMaskExtractor_DualBkg.h


# The following lines tells Qmake to use pkg-config for opencv
QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += opencv
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/
LIBS += -L/usr/lib/x86_64-linux-gnu/

DISTFILES += \
    ../../../lib/ACF/INRIA_ACF.xml \
    ../../../lib/rapidxml-1.13/license.txt
