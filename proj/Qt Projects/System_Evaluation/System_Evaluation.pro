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
    ../../../src/aod.cpp \
    ../../../src/bgs/BGSselector.cpp \
    ../../../src/blobs/BasicBlob.cpp \
    ../../../src/blobs/BlobExtractor.cpp \
    ../../../src/blobs/BlobList.cpp \
    ../../../src/blobs/DefineObjectBlobList.cpp \
    ../../../src/blobs/ObjectBlob.cpp \
    ../../../src/event/Event.cpp \
    ../../../src/event/EventController.cpp \
    ../../../src/event/EventWriter.cpp \
    ../../../src/pd/PDselector.cpp \
    ../../../src/sfgd/SFGDselector.cpp \
    ../../../src/sfgd/StaticMaskExtractor.cpp \
    ../../../src/sfgd/StaticMaskExtractor_AccMask.cpp \
    ../../../src/sfgd/StaticMaskExtractor_Subsampling.cpp \
    ../../../src/soc/SOCselector.cpp \
    ../../../src/soc/StaticObjectClassifier.cpp \
    ../../../src/soc/StaticObjectClassifierHighGradient.cpp \
    ../../../src/soc/StaticObjectClassifierHist.cpp \
    ../../../src/soc/StaticObjectClassifierPCC.cpp \
    ../../../src/utils/utils.cpp \
    ../../../src/Config.cpp \
    ../../../src/sfgd/StaticMaskExtractor_DualBkg.cpp \
    ../../../src/sfgd/StaticMaskExtractor_HistoryImages.cpp \
    ../../../lib/bgslibrary/package_bgs/KDE/NPBGSubtractor.cpp \
    ../../../lib/bgslibrary/package_bgs/KDE/KernelTable.cpp \
    ../../../lib/bgslibrary/package_bgs/KDE/NPBGmodel.cpp \
    ../../../lib/bgslibrary/package_bgs/KDE.cpp \
       ../../../lib/pdlibrary/acf/ACF/ACFFeaturePyramid.cpp \
    ../../../lib/pdlibrary/acf/ACF/Channel.cpp \
    ../../../lib/pdlibrary/acf/ACF/ChannelFeatures.cpp \
    ../../../lib/pdlibrary/acf/ACF/ColorChannel.cpp \
    ../../../lib/pdlibrary/acf/ACF/convConst.cpp \
    ../../../lib/pdlibrary/acf/ACF/GradHistChannel.cpp \
    ../../../lib/pdlibrary/acf/ACF/gradientMex.cpp \
    ../../../lib/pdlibrary/acf/ACF/GradMagChannel.cpp \
    ../../../lib/pdlibrary/acf/ACF/wrappers.cpp \
    ../../../lib/pdlibrary/acf/Channel/Convert.cpp \
    ../../../lib/pdlibrary/acf/Core/detection.cpp \
    ../../../lib/pdlibrary/acf/Core/DetectionList.cpp \
    ../../../lib/pdlibrary/acf/Core/detector.cpp \
    ../../../lib/pdlibrary/acf/Core/DetectorManager.cpp \
    ../../../lib/pdlibrary/acf/Core/detectormodel.cpp \
    ../../../lib/pdlibrary/acf/Core/feature.cpp \
    ../../../lib/pdlibrary/acf/Core/featurelayer.cpp \
    ../../../lib/pdlibrary/acf/Core/filelocator.cpp \
    ../../../lib/pdlibrary/acf/Core/FileWriter.cpp \
    ../../../lib/pdlibrary/acf/Core/FrameFromCamera.cpp \
    ../../../lib/pdlibrary/acf/Core/FrameFromDirectory.cpp \
    ../../../lib/pdlibrary/acf/Core/FrameFromVideo.cpp \
    ../../../lib/pdlibrary/acf/Core/FrameProducer.cpp \
    ../../../lib/pdlibrary/acf/Core/nms.cpp \
    ../../../lib/pdlibrary/acf/Core/NonMaximumSuppression.cpp \
    ../../../lib/pdlibrary/acf/Core/ScaleSpacePyramid.cpp \
    ../../../lib/pdlibrary/dpm/dpm_cascade.cpp \
    ../../../lib/pdlibrary/dpm/dpm_cascade_detector.cpp \
    ../../../lib/pdlibrary/dpm/dpm_convolution.cpp \
    ../../../lib/pdlibrary/dpm/dpm_feature.cpp \
    ../../../lib/pdlibrary/dpm/dpm_model.cpp \
    ../../../lib/pdlibrary/dpm/dpm_nms.cpp \
    #../../../src/sfgd/StaticMaskExtractor_TripleBkg.cpp
    ../../../src/sfgd/StaticMaskExtractor_TripleBkg.cpp



HEADERS  +=        ../../../lib/bgslibrary/package_bgs/KNN.h \
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
    ../../../src/aod.h \
     ../../../src/bgs/BGSselector.h \
    ../../../src/blobs/BasicBlob.h \
    ../../../src/blobs/BlobExtractor.h \
    ../../../src/blobs/BlobList.h \
    ../../../src/blobs/DefineObjectBlobList.h \
    ../../../src/blobs/ObjectBlob.h \
    ../../../src/event/Event.h \
    ../../../src/event/EventController.h \
    ../../../src/event/EventWriter.h \
    ../../../src/pd/PDselector.h \
    ../../../src/sfgd/SFGDselector.h \
    ../../../src/sfgd/StaticMaskExtractor.h \
    ../../../src/sfgd/StaticMaskExtractor_AccMask.h \
    ../../../src/sfgd/StaticMaskExtractor_subsampling.h \
    ../../../src/soc/SOCselector.h \
    ../../../src/soc/StaticObjectClassifier.h \
    ../../../src/soc/StaticObjectClassifierHighGradient.h \
    ../../../src/soc/StaticObjectClassifierHist.h \
    ../../../src/soc/StaticObjectClassifierPCC.h \
    ../../../src/utils/utils.h \
    ../../../src/Config.h \
    ../../../src/sfgd/StaticMaskExtractor_DualBkg.h \
    ../../../src/sfgd/StaticMaskExtractor_HistoryImages.h \
        ../../../lib/bgslibrary/package_bgs/KDE/KernelTable.h \
    ../../../lib/bgslibrary/package_bgs/KDE/NPBGmodel.h \
    ../../../lib/bgslibrary/package_bgs/KDE/NPBGSubtractor.h \
      ../../../lib/bgslibrary/package_bgs/GMG.h \
    ../../../lib/bgslibrary/package_bgs/KDE.h \
          ../../../lib/bgslibrary/Config.h \
    ../../../lib/bgslibrary/FrameProcessor.h \
    ../../../lib/bgslibrary/IFrameProcessor.h \
    ../../../lib/bgslibrary/PreProcessor.h \
    ../../../lib/bgslibrary/VideoAnalysis.h \
    ../../../lib/bgslibrary/VideoCapture.h \
    ../../../lib/pdlibrary/acf/ACF/ACFDetector.h \
    ../../../lib/pdlibrary/acf/ACF/ACFFeaturePyramid.h \
    ../../../lib/pdlibrary/acf/ACF/Channel.h \
    ../../../lib/pdlibrary/acf/ACF/ChannelFeatures.h \
    ../../../lib/pdlibrary/acf/ACF/ColorChannel.h \
    ../../../lib/pdlibrary/acf/ACF/GradHistChannel.h \
    ../../../lib/pdlibrary/acf/ACF/GradMagChannel.h \
    ../../../lib/pdlibrary/acf/ACF/imResampleMex.hpp \
    ../../../lib/pdlibrary/acf/ACF/rgbConvertMex.hpp \
    ../../../lib/pdlibrary/acf/ACF/sse.hpp \
    ../../../lib/pdlibrary/acf/ACF/wrappers.hpp \
    ../../../lib/pdlibrary/acf/Channel/Functions.h \
    ../../../lib/pdlibrary/acf/Core/detection.h \
    ../../../lib/pdlibrary/acf/Core/DetectionList.h \
    ../../../lib/pdlibrary/acf/Core/detector.h \
    ../../../lib/pdlibrary/acf/Core/DetectorManager.h \
    ../../../lib/pdlibrary/acf/Core/detectormodel.h \
    ../../../lib/pdlibrary/acf/Core/dirent.h \
    ../../../lib/pdlibrary/acf/Core/feature.h \
    ../../../lib/pdlibrary/acf/Core/featurelayer.h \
    ../../../lib/pdlibrary/acf/Core/filelocator.h \
    ../../../lib/pdlibrary/acf/Core/FileWriter.h \
    ../../../lib/pdlibrary/acf/Core/FrameFromCamera.h \
    ../../../lib/pdlibrary/acf/Core/FrameFromDirectory.h \
    ../../../lib/pdlibrary/acf/Core/FrameFromVideo.h \
    ../../../lib/pdlibrary/acf/Core/FrameProducer.h \
    ../../../lib/pdlibrary/acf/Core/Image.hpp \
    ../../../lib/pdlibrary/acf/Core/nms.h \
    ../../../lib/pdlibrary/acf/Core/NonMaximumSuppression.h \
    ../../../lib/pdlibrary/acf/Core/ScaleSpacePyramid.h \
    ../../../lib/pdlibrary/acf/rapidxml-1.13/rapidxml.hpp \
    ../../../lib/pdlibrary/acf/rapidxml-1.13/rapidxml_iterators.hpp \
    ../../../lib/pdlibrary/acf/rapidxml-1.13/rapidxml_print.hpp \
    ../../../lib/pdlibrary/acf/rapidxml-1.13/rapidxml_utils.hpp \
    ../../../lib/pdlibrary/acf/acf.hpp \
    ../../../lib/pdlibrary/dpm/dpm.hpp \
    ../../../lib/pdlibrary/dpm/dpm_cascade.hpp \
    ../../../lib/pdlibrary/dpm/dpm_convolution.hpp \
    ../../../lib/pdlibrary/dpm/dpm_feature.hpp \
    ../../../lib/pdlibrary/dpm/dpm_model.hpp \
    ../../../lib/pdlibrary/dpm/dpm_nms.hpp \
    ../../../lib/pdlibrary/dpm/precomp.hpp \
    #../../../src/sfgd/StaticMaskExtractor_TripleBkg.h
    ../../../src/sfgd/StaticMaskExtractor_TripleBkg.h



# The following lines tells Qmake to use pkg-config for opencv
QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += opencv
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/
LIBS += -L/usr/lib/x86_64-linux-gnu/
# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1

# add the desired -O2 if not present
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE -= -O4
QMAKE_CXXFLAGS_RELEASE *= -O5


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../lib/pdlibrary/build/release/ -lpd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../lib/pdlibrary/build/debug/ -lpd
else:unix: LIBS += -L$$PWD/../../../lib/pdlibrary/build/ -lpd

INCLUDEPATH += $$PWD/../../../lib/pdlibrary/build
DEPENDPATH += $$PWD/../../../lib/pdlibrary/build

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../lib/pdlibrary/build/release/libpd.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../lib/pdlibrary/build/debug/libpd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../lib/pdlibrary/build/release/pd.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../lib/pdlibrary/build/debug/pd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../lib/pdlibrary/build/libpd.a

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../lib/bgslibrary/build/release/ -lbgs
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../lib/bgslibrary/build/debug/ -lbgs
#else:unix: LIBS += -L$$PWD/../../../lib/bgslibrary/build/ -lbgs

#INCLUDEPATH += $$PWD/../../../lib/bgslibrary/build
#DEPENDPATH += $$PWD/../../../lib/bgslibrary/build

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../lib/bgslibrary/build/release/libbgs.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../lib/bgslibrary/build/debug/libbgs.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../lib/bgslibrary/build/release/bgs.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../lib/bgslibrary/build/debug/bgs.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../../lib/bgslibrary/build/libbgs.a
#
#SUBDIRS += \
 #   ../../../lib/bgslibrary/gui_qt/bgslibrary_gui.pro

#RESOURCES += \
 #   ../../../lib/bgslibrary/gui_qt/application.qrc

