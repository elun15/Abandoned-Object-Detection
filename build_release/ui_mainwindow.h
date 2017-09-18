/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <imagewidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    ImageWidget *widgetFrame;
    QGridLayout *gridLayout_8;
    ImageWidget *widgetClassification;
    QGridLayout *gridLayout_3;
    ImageWidget *widgetStaticForeground;
    QGridLayout *gridLayout_5;
    ImageWidget *widgetForeground;
    QGridLayout *gridLayout_6;
    ImageWidget *widgetBackground;
    QGridLayout *gridLayout_7;
    ImageWidget *widgetPeopleDetection;
    QGridLayout *gridLayout_4;
    QLabel *label_4;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QTextBrowser *textBrowser;
    QPushButton *pushButtonClose;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_5;
    QComboBox *comboBoxBKG;
    QLabel *label_6;
    QComboBox *comboBoxSFGD;
    QLabel *label_7;
    QComboBox *comboBoxPD;
    QLabel *label_8;
    QComboBox *comboBoxClassifier;
    QCheckBox *PauseButton;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout;
    QSpinBox *spinBoxSecs;
    QLabel *label_15;
    QCheckBox *PeopleDetectionButton;
    QCheckBox *SaveResultsButton;
    QCheckBox *ContextMaskButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1219, 940);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 500));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widgetFrame = new ImageWidget(groupBox);
        widgetFrame->setObjectName(QStringLiteral("widgetFrame"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(widgetFrame->sizePolicy().hasHeightForWidth());
        widgetFrame->setSizePolicy(sizePolicy1);
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        widgetFrame->setFont(font);
        gridLayout_8 = new QGridLayout(widgetFrame);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));

        gridLayout_2->addWidget(widgetFrame, 1, 0, 1, 1);

        widgetClassification = new ImageWidget(groupBox);
        widgetClassification->setObjectName(QStringLiteral("widgetClassification"));
        sizePolicy1.setHeightForWidth(widgetClassification->sizePolicy().hasHeightForWidth());
        widgetClassification->setSizePolicy(sizePolicy1);
        widgetClassification->setFont(font);
        gridLayout_3 = new QGridLayout(widgetClassification);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));

        gridLayout_2->addWidget(widgetClassification, 3, 2, 1, 1);

        widgetStaticForeground = new ImageWidget(groupBox);
        widgetStaticForeground->setObjectName(QStringLiteral("widgetStaticForeground"));
        sizePolicy1.setHeightForWidth(widgetStaticForeground->sizePolicy().hasHeightForWidth());
        widgetStaticForeground->setSizePolicy(sizePolicy1);
        widgetStaticForeground->setFont(font);
        gridLayout_5 = new QGridLayout(widgetStaticForeground);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));

        gridLayout_2->addWidget(widgetStaticForeground, 3, 0, 1, 1);

        widgetForeground = new ImageWidget(groupBox);
        widgetForeground->setObjectName(QStringLiteral("widgetForeground"));
        sizePolicy1.setHeightForWidth(widgetForeground->sizePolicy().hasHeightForWidth());
        widgetForeground->setSizePolicy(sizePolicy1);
        gridLayout_6 = new QGridLayout(widgetForeground);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));

        gridLayout_2->addWidget(widgetForeground, 1, 2, 1, 1);

        widgetBackground = new ImageWidget(groupBox);
        widgetBackground->setObjectName(QStringLiteral("widgetBackground"));
        sizePolicy1.setHeightForWidth(widgetBackground->sizePolicy().hasHeightForWidth());
        widgetBackground->setSizePolicy(sizePolicy1);
        gridLayout_7 = new QGridLayout(widgetBackground);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));

        gridLayout_2->addWidget(widgetBackground, 1, 1, 1, 1);

        widgetPeopleDetection = new ImageWidget(groupBox);
        widgetPeopleDetection->setObjectName(QStringLiteral("widgetPeopleDetection"));
        sizePolicy1.setHeightForWidth(widgetPeopleDetection->sizePolicy().hasHeightForWidth());
        widgetPeopleDetection->setSizePolicy(sizePolicy1);
        widgetPeopleDetection->setFont(font);
        gridLayout_4 = new QGridLayout(widgetPeopleDetection);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));

        gridLayout_2->addWidget(widgetPeopleDetection, 3, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        QFont font1;
        font1.setPointSize(18);
        label_4->setFont(font1);

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        sizePolicy2.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy2);
        label_9->setFont(font1);

        gridLayout_2->addWidget(label_9, 2, 1, 1, 1);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));
        sizePolicy2.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy2);
        label_10->setFont(font1);

        gridLayout_2->addWidget(label_10, 2, 2, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setFont(font1);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy3);
        QFont font2;
        font2.setPointSize(18);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(50);
        font2.setStrikeOut(false);
        font2.setKerning(true);
        label_2->setFont(font2);

        gridLayout_2->addWidget(label_2, 0, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy3);
        label_3->setFont(font2);

        gridLayout_2->addWidget(label_3, 0, 2, 1, 1);

        widgetFrame->raise();
        widgetBackground->raise();
        label->raise();
        label_2->raise();
        widgetForeground->raise();
        label_3->raise();
        widgetStaticForeground->raise();
        label_4->raise();
        label_9->raise();
        widgetPeopleDetection->raise();
        label_10->raise();
        widgetClassification->raise();

        gridLayout->addWidget(groupBox, 0, 0, 2, 3);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy4);

        gridLayout->addWidget(textBrowser, 2, 0, 1, 3);

        pushButtonClose = new QPushButton(centralWidget);
        pushButtonClose->setObjectName(QStringLiteral("pushButtonClose"));
        QFont font3;
        font3.setPointSize(15);
        pushButtonClose->setFont(font3);

        gridLayout->addWidget(pushButtonClose, 4, 1, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QFont font4;
        font4.setPointSize(20);
        groupBox_2->setFont(font4);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        QFont font5;
        font5.setPointSize(16);
        label_5->setFont(font5);

        verticalLayout_2->addWidget(label_5);

        comboBoxBKG = new QComboBox(groupBox_2);
        comboBoxBKG->setObjectName(QStringLiteral("comboBoxBKG"));
        comboBoxBKG->setFont(font3);

        verticalLayout_2->addWidget(comboBoxBKG);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font5);

        verticalLayout_2->addWidget(label_6);

        comboBoxSFGD = new QComboBox(groupBox_2);
        comboBoxSFGD->setObjectName(QStringLiteral("comboBoxSFGD"));
        comboBoxSFGD->setFont(font3);

        verticalLayout_2->addWidget(comboBoxSFGD);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font5);

        verticalLayout_2->addWidget(label_7);

        comboBoxPD = new QComboBox(groupBox_2);
        comboBoxPD->setObjectName(QStringLiteral("comboBoxPD"));
        comboBoxPD->setFont(font3);

        verticalLayout_2->addWidget(comboBoxPD);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font5);

        verticalLayout_2->addWidget(label_8);

        comboBoxClassifier = new QComboBox(groupBox_2);
        comboBoxClassifier->setObjectName(QStringLiteral("comboBoxClassifier"));
        comboBoxClassifier->setFont(font3);

        verticalLayout_2->addWidget(comboBoxClassifier);

        comboBoxBKG->raise();
        comboBoxSFGD->raise();
        label_6->raise();
        label_5->raise();
        comboBoxPD->raise();
        comboBoxClassifier->raise();
        label_8->raise();
        label_7->raise();

        gridLayout->addWidget(groupBox_2, 0, 3, 1, 1);

        PauseButton = new QCheckBox(centralWidget);
        PauseButton->setObjectName(QStringLiteral("PauseButton"));
        PauseButton->setEnabled(true);
        PauseButton->setFont(font3);
        PauseButton->setChecked(false);

        gridLayout->addWidget(PauseButton, 4, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        QSizePolicy sizePolicy5(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy5);
        groupBox_3->setFont(font4);
        formLayout = new QFormLayout(groupBox_3);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        spinBoxSecs = new QSpinBox(groupBox_3);
        spinBoxSecs->setObjectName(QStringLiteral("spinBoxSecs"));
        spinBoxSecs->setFont(font5);
        spinBoxSecs->setMaximum(20);
        spinBoxSecs->setValue(10);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinBoxSecs);

        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setFont(font5);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_15);

        PeopleDetectionButton = new QCheckBox(groupBox_3);
        PeopleDetectionButton->setObjectName(QStringLiteral("PeopleDetectionButton"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(PeopleDetectionButton->sizePolicy().hasHeightForWidth());
        PeopleDetectionButton->setSizePolicy(sizePolicy6);
        PeopleDetectionButton->setFont(font5);
        PeopleDetectionButton->setLayoutDirection(Qt::RightToLeft);

        formLayout->setWidget(1, QFormLayout::SpanningRole, PeopleDetectionButton);

        SaveResultsButton = new QCheckBox(groupBox_3);
        SaveResultsButton->setObjectName(QStringLiteral("SaveResultsButton"));
        sizePolicy6.setHeightForWidth(SaveResultsButton->sizePolicy().hasHeightForWidth());
        SaveResultsButton->setSizePolicy(sizePolicy6);
        SaveResultsButton->setFont(font5);
        SaveResultsButton->setLayoutDirection(Qt::RightToLeft);

        formLayout->setWidget(2, QFormLayout::SpanningRole, SaveResultsButton);

        ContextMaskButton = new QCheckBox(groupBox_3);
        ContextMaskButton->setObjectName(QStringLiteral("ContextMaskButton"));
        sizePolicy6.setHeightForWidth(ContextMaskButton->sizePolicy().hasHeightForWidth());
        ContextMaskButton->setSizePolicy(sizePolicy6);
        ContextMaskButton->setFont(font5);
        ContextMaskButton->setLayoutDirection(Qt::RightToLeft);

        formLayout->setWidget(3, QFormLayout::SpanningRole, ContextMaskButton);


        gridLayout->addWidget(groupBox_3, 1, 3, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        groupBox_2->raise();
        PauseButton->raise();
        pushButtonClose->raise();
        textBrowser->raise();
        groupBox->raise();
        groupBox_3->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1219, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);

        retranslateUi(MainWindow);
        QObject::connect(pushButtonClose, SIGNAL(clicked()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "System", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        groupBox->setTitle(QString());
        label_4->setText(QApplication::translate("MainWindow", "Static Foreground", 0));
        label_9->setText(QApplication::translate("MainWindow", "Detections", 0));
        label_10->setText(QApplication::translate("MainWindow", "Classification", 0));
        label->setText(QApplication::translate("MainWindow", "Frame", 0));
        label_2->setText(QApplication::translate("MainWindow", "Background", 0));
        label_3->setText(QApplication::translate("MainWindow", "Foreground", 0));
        pushButtonClose->setText(QApplication::translate("MainWindow", "Close", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Method Selection", 0));
        label_5->setText(QApplication::translate("MainWindow", "Background Subtraction", 0));
        comboBoxBKG->clear();
        comboBoxBKG->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "LOBSTER", 0)
         << QApplication::translate("MainWindow", "PAWCS", 0)
         << QApplication::translate("MainWindow", "MOG2", 0)
         << QApplication::translate("MainWindow", "KNN", 0)
         << QApplication::translate("MainWindow", "IMBS", 0)
        );
        label_6->setText(QApplication::translate("MainWindow", "Static Foreground Detection", 0));
        comboBoxSFGD->clear();
        comboBoxSFGD->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Subsampling", 0)
         << QApplication::translate("MainWindow", "Accumulative Mask", 0)
        );
        label_7->setText(QApplication::translate("MainWindow", "People Detection", 0));
        comboBoxPD->clear();
        comboBoxPD->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "HOG", 0)
         << QApplication::translate("MainWindow", "DPM", 0)
         << QApplication::translate("MainWindow", "Haar Upper Body", 0)
         << QApplication::translate("MainWindow", "Haar Frontal Face", 0)
         << QApplication::translate("MainWindow", "Haar Full Body", 0)
        );
        label_8->setText(QApplication::translate("MainWindow", "Classifier", 0));
        comboBoxClassifier->clear();
        comboBoxClassifier->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "High Gradient", 0)
         << QApplication::translate("MainWindow", "Histogram", 0)
        );
        PauseButton->setText(QApplication::translate("MainWindow", "Pause execution", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Parameters Selection", 0));
        label_15->setText(QApplication::translate("MainWindow", "Seconds to static", 0));
        PeopleDetectionButton->setText(QApplication::translate("MainWindow", "People Detection           ", 0));
        SaveResultsButton->setText(QApplication::translate("MainWindow", "Save results                    ", 0));
        ContextMaskButton->setText(QApplication::translate("MainWindow", "Set context mask           ", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
