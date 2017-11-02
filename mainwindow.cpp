#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  resize(1280,500);
  createControlGroupBox();
  createFreqScanGroupBox();
  createChannelGroupBox();
  createNoteGroupBox();

  windowLayout= new QVBoxLayout;
  windowLayout->addWidget(controlGroupBox);
  windowLayout->addWidget(freqScanGroupBox);
  windowLayout->addWidget(channelGroupBox);
  windowLayout->addWidget(noteGroupBox);

  window = new QWidget();
  window->setLayout(windowLayout);

  setCentralWidget(window);


  createActions();
  createMenus();
  // createToolBars();
  createStatusBar();
  createDockWindows();

  setWindowTitle(tr("NI-DAQ"));
}

void MainWindow::createControlGroupBox()
{

  controlGroupBox = new QGroupBox(tr("Control"));

  startPushButton = new QPushButton(tr("&Start"));

  stopPushButton = new QPushButton(tr("&Stop"));

  QLabel *clockSpeedLabel = new QLabel(tr("Clock Speed"));
  clockSpeedLabel->setAlignment(Qt::AlignCenter);

  clockSpeedSpinBox = new QSpinBox;
  clockSpeedSpinBox->setRange(1,1000000);  // Clock speed range
  clockSpeedSpinBox->setSingleStep(clockSingleStep);
  clockSpeedSpinBox->setSuffix(" us");
  clockSpeedSpinBox->setValue(5*clockSingleStep);   // Set Clock speed
  clockSpeedSpinBox->setKeyboardTracking(false);

  QLabel *repeatNumLabel = new QLabel(tr("Repeat Num"));
  clockSpeedLabel->setAlignment(Qt::AlignCenter);

  repeatNumSpinBox = new QSpinBox;
  repeatNumSpinBox->setRange(1,20);  // Repeat Num range
  // repeatNumSpinBox->setSingleStep(clockSingleStep);
  clockSpeedSpinBox->setSuffix(" ");
  repeatNumSpinBox->setValue(1);   // Set the default repeat number 1
  repeatNumSpinBox->setKeyboardTracking(false);

  repeatProgressLine = new QLineEdit;
  repeatProgressLine->setAlignment(Qt::AlignRight);
  // repeatProgressLine->setEnabled(false);
  repeatProgressLine->setReadOnly(true);

  freqScanControlComboBox = new QComboBox;
  freqScanControlComboBox->addItem("Single");
  freqScanControlComboBox->addItem("Frequency Scan");
  freqScanControlComboBox->setEnabled(true);

  QLabel *writeDataLabel = new QLabel(tr("Write"));
  writeDataLabel->setAlignment(Qt::AlignCenter);

  writeDataCheckBox = new QCheckBox();

  controlLayout= new QGridLayout;
  controlLayout->addWidget(clockSpeedLabel,0,0);
  controlLayout->addWidget(clockSpeedSpinBox,0,1);
  controlLayout->addWidget(repeatNumLabel,0,2);
  controlLayout->addWidget(repeatNumSpinBox,0,3);
  controlLayout->addWidget(repeatProgressLine,0,4);
  controlLayout->addWidget(freqScanControlComboBox,0,5);
  controlLayout->addWidget(writeDataLabel,0,6);
  controlLayout->addWidget(writeDataCheckBox,0,7);
  controlLayout->addWidget(startPushButton,0,8);
  controlLayout->addWidget(stopPushButton,0,9);
  controlLayout->setColumnStretch(8,1);
  controlLayout->setColumnStretch(9,1);
  
  controlGroupBox->setLayout(controlLayout);
  connect(clockSpeedSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateReadRate()));
  connect(freqScanControlComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(freqScanControlComboBoxChanged())) ;

  timeout = timeLimit/1000000;

  connect(startPushButton, SIGNAL(clicked()),this,SLOT(startButtonPressed()));
  connect(stopPushButton, SIGNAL(clicked()),this,SLOT(stopButtonPressed()));
}

void MainWindow::createFreqScanGroupBox() {
  freqScanGroupBox = new QGroupBox(tr("Frequency Scan"));

  QLabel *freqScanMasterLabel = new QLabel(tr("Master"));
  freqScanMasterComboBox = new QComboBox;
  freqScanMasterComboBox->addItem("F=4 - F'=4co5");
  freqScanMasterComboBox->addItem("F=4 - F'=5");
  freqScanMasterComboBox->addItem("F=3 - F'=2co3");
  freqScanMasterComboBox->addItem("F=3 - F'=3");
  freqScanMasterComboBox->setEnabled(false);

  QLabel *freqScanTerminalLabel = new QLabel(tr("Terminal"));
  freqScanTerminalComboBox = new QComboBox;
  freqScanTerminalComboBox->addItem("P0.0 (DDS1)");
  freqScanTerminalComboBox->addItem("P0.1 (DDS2)");
  freqScanTerminalComboBox->setEnabled(false);

  QLabel *freqScanTimeLabel = new QLabel(tr("Time"));
  freqScanTimeComboBox = new QComboBox;
  // freqScanTimeString = new QString;
  QString freqScanTimeString;
  int i;
  for (i=0; i<pulseNum; i++) {
      freqScanTimeString = QString::number(i);
      freqScanTimeComboBox->addItem(freqScanTimeString);
  }
  freqScanTimeComboBox->setEnabled(false);

  QLabel *freqScanAOMLabel = new QLabel(tr("AOM"));
  freqScanAOMComboBox = new QComboBox;
  freqScanAOMComboBox->addItem("+80MHz x2");
  freqScanAOMComboBox->addItem("-80MHz x2");
  freqScanAOMComboBox->addItem("+80MHz x3");
  freqScanAOMComboBox->addItem("+200MHz x2");
  freqScanAOMComboBox->addItem("-200MHz x2");
  freqScanAOMComboBox->setEnabled(false);

  QLabel *freqScanSlaveLabel = new QLabel(tr("Slave"));
  freqScanSlaveComboBox = new QComboBox;
  freqScanSlaveComboBox->addItem("F=4 - F'=5");
  freqScanSlaveComboBox->addItem("F=4 - F'=4");
  freqScanSlaveComboBox->addItem("F=4 - F'=3");
  freqScanSlaveComboBox->addItem("F=3 - F'=4");
  freqScanSlaveComboBox->addItem("F=3 - F'=3");
  freqScanSlaveComboBox->addItem("F=3 - F'=2");
  freqScanSlaveComboBox->setEnabled(false);

  QLabel *freqScanDirectionLabel = new QLabel(tr("Direction"));
  freqScanDirectionComboBox = new QComboBox;
  freqScanDirectionComboBox->addItem("Increase");
  freqScanDirectionComboBox->addItem("Decrease");
  freqScanDirectionComboBox->addItem("Center");
  freqScanDirectionComboBox->addItem("Edges");
  freqScanDirectionComboBox->setEnabled(false);

  QLabel *freqScanInitLabel = new QLabel(tr("Init [MHz]"));
  freqScanInitLabel->setAlignment(Qt::AlignRight);
  freqScanInitLine = new QLineEdit;
  freqScanInitLine->setAlignment(Qt::AlignRight);
  freqScanInitLine->setEnabled(false);

  QLabel *freqScanStepLabel = new QLabel(tr("Step size [MHz]"));
  freqScanStepLabel->setAlignment(Qt::AlignRight);
  freqScanStepLine = new QLineEdit;
  freqScanStepLine->setAlignment(Qt::AlignRight);
  freqScanStepLine->setEnabled(false);

  QLabel *freqScanNumLabel = new QLabel(tr("Number"));
  freqScanNumLabel->setAlignment(Qt::AlignRight);
  freqScanNumLine = new QLineEdit;
  freqScanNumLine->setAlignment(Qt::AlignRight);
  freqScanNumLine->setEnabled(false);

  QLabel *freqScanPeriodLabel = new QLabel(tr("Period [s]"));
  freqScanPeriodLabel->setAlignment(Qt::AlignRight);
  freqScanPeriodLine = new QLineEdit;
  freqScanPeriodLine->setAlignment(Qt::AlignRight);
  freqScanPeriodLine->setEnabled(false);

  QLabel *freqScanFinalLabel = new QLabel(tr("Final [MHz]"));
  freqScanFinalLabel->setAlignment(Qt::AlignRight);
  freqScanFinalLine = new QLineEdit;
  freqScanFinalLine->setAlignment(Qt::AlignRight);
  freqScanFinalLine->setEnabled(false);
  freqScanFinalLine->setReadOnly(true);

  QLabel *freqScanProgressLabel = new QLabel(tr("Progress"));
  freqScanProgressLabel->setAlignment(Qt::AlignRight);
  freqScanProgressLine = new QLineEdit;
  freqScanProgressLine->setAlignment(Qt::AlignRight);
  freqScanProgressLine->setEnabled(false);
  freqScanProgressLine->setReadOnly(true);

  freqScanLayout= new QGridLayout;
  freqScanLayout->addWidget(freqScanMasterLabel,0,0);
  freqScanLayout->addWidget(freqScanMasterComboBox,1,0);
  freqScanLayout->addWidget(freqScanTerminalLabel,0,1);
  freqScanLayout->addWidget(freqScanTerminalComboBox,1,1);
  freqScanLayout->addWidget(freqScanTimeLabel,0,2);
  freqScanLayout->addWidget(freqScanTimeComboBox,1,2);
  freqScanLayout->addWidget(freqScanSlaveLabel,0,3);
  freqScanLayout->addWidget(freqScanSlaveComboBox,1,3);
  freqScanLayout->addWidget(freqScanAOMLabel,0,4);
  freqScanLayout->addWidget(freqScanAOMComboBox,1,4);
  freqScanLayout->addWidget(freqScanDirectionLabel,0,5);
  freqScanLayout->addWidget(freqScanDirectionComboBox,1,5);
  freqScanLayout->addWidget(freqScanInitLabel,0,6);
  freqScanLayout->addWidget(freqScanInitLine,1,6);
  freqScanLayout->addWidget(freqScanStepLabel,0,7);
  freqScanLayout->addWidget(freqScanStepLine,1,7);
  freqScanLayout->addWidget(freqScanNumLabel,0,8);
  freqScanLayout->addWidget(freqScanNumLine,1,8);
  freqScanLayout->addWidget(freqScanPeriodLabel,0,9);
  freqScanLayout->addWidget(freqScanPeriodLine,1,9);
  freqScanLayout->addWidget(freqScanFinalLabel,0,10);
  freqScanLayout->addWidget(freqScanFinalLine,1,10);
  freqScanLayout->addWidget(freqScanProgressLabel,0,11);
  freqScanLayout->addWidget(freqScanProgressLine,1,11);

  freqScanGroupBox->setLayout(freqScanLayout);

  connect(freqScanInitLine,SIGNAL(editingFinished()),this,SLOT(getFreqScanFinal()));
  connect(freqScanStepLine,SIGNAL(editingFinished()),this,SLOT(getFreqScanFinal()));
  connect(freqScanNumLine,SIGNAL(editingFinished()),this,SLOT(getFreqScanFinal()));
}

void MainWindow::freqScanControlComboBoxChanged()
{
    if (freqScanControlComboBox->currentIndex() == 0) {
        freqScanMasterComboBox->setEnabled(false);
        freqScanTerminalComboBox->setEnabled(false);
        freqScanTimeComboBox->setEnabled(false);
        freqScanSlaveComboBox->setEnabled(false);
        freqScanAOMComboBox->setEnabled(false);
        freqScanDirectionComboBox->setEnabled(false);
        freqScanInitLine->setEnabled(false);
        freqScanStepLine->setEnabled(false);
        freqScanNumLine->setEnabled(false);
        freqScanPeriodLine->setEnabled(false);
        freqScanFinalLine->setEnabled(false);
        freqScanProgressLine->setEnabled(false);
        plotDockObj[0]->plotInComboBox->removeItem(plotDockObj[0]->plotInComboBox->count()-1);
        for (int i = 0; i < counterChannelNum; i++) {
            counterChannelObj[i]->CNTrefTimeComboBox->setEnabled(false);
            counterChannelObj[i]->CNTstartComboBox->setEnabled(false);
            counterChannelObj[i]->CNTendComboBox->setEnabled(false);
        }
    }
    else if (freqScanControlComboBox->currentIndex() == 1){
        freqScanMasterComboBox->setEnabled(true);
        freqScanTerminalComboBox->setEnabled(true);
        freqScanTimeComboBox->setEnabled(true);
        freqScanSlaveComboBox->setEnabled(true);
        freqScanAOMComboBox->setEnabled(true);
        freqScanDirectionComboBox->setEnabled(true);
        freqScanInitLine->setEnabled(true);
        freqScanStepLine->setEnabled(true);
        freqScanNumLine->setEnabled(true);
        freqScanPeriodLine->setEnabled(true);
        freqScanFinalLine->setEnabled(true);
        freqScanProgressLine->setEnabled(true);
        plotDockObj[0]->plotInComboBox->addItem("Frequency Scan");
        for (int i = 0; i < counterChannelNum; i++) {
            counterChannelObj[i]->CNTrefTimeComboBox->setEnabled(true);
            counterChannelObj[i]->CNTstartComboBox->setEnabled(true);
            counterChannelObj[i]->CNTendComboBox->setEnabled(true);
        }
    }
}

void MainWindow::getFreqScanFinal(){
    freqScanInit = freqScanInitLine->text().toFloat();
    freqScanNum = freqScanNumLine->text().toInt();
    freqScanStep = freqScanStepLine->text().toFloat();
    freqScanFinal = freqScanInit + (freqScanNum-1) * freqScanStep;
    freqScanFinalLine->setText(QString::number(freqScanFinal));

}

void MainWindow::startButtonPressed()
{
    stopFlag = false;
    repeatProgressLine->setText(QString::number(0)+"/"+QString::number(repeatNumSpinBox->value()));
    for (repeatCount = 0; repeatCount<repeatNumSpinBox->value();repeatCount++) {
        QCoreApplication::processEvents(); // Let events process (e.g. StopButton pressed)
        if (stopFlag) { break; }
        if (freqScanControlComboBox->currentIndex() == 0) {  // Single Run
            startSingleRun();
        }
        else if (freqScanControlComboBox->currentIndex() == 1) {  // Frequency Scan
            // stopFlag = false;
            freqScanInit = freqScanInitLine->text().toFloat();
            freqScanNum = freqScanNumLine->text().toInt();
            freqScanStep = freqScanStepLine->text().toFloat();
            freqScanPeriod = freqScanPeriodLine->text().toFloat();
            freqScanFinal = freqScanFinalLine->text().toFloat();
            freqScanDivider = 1.0/(2 - freqScanTerminalComboBox->currentIndex()); // 0.5 for P0.0, 1 for P0.1
            detuneArray = new float [freqScanNum];
            countDataArray = new int [freqScanNum];
            for (freqScanCount=0;freqScanCount<freqScanNum;freqScanCount++) {
                if (stopFlag) { break; }
                if (freqScanDirectionComboBox->currentIndex() == 0) { // Increase frequency
                    freqScanRef = freqScanInit; freqMultiplier = freqScanCount;
                }
                else if (freqScanDirectionComboBox->currentIndex() == 1) { // Decrease frequency
                    freqScanRef = freqScanFinal; freqMultiplier = -freqScanCount;
                }
                else if (freqScanDirectionComboBox->currentIndex() == 2) { // scan frequency from center to edge
                    freqScanRef = (freqScanInit + freqScanFinal)/2;
                    if (freqScanNum%2) { freqMultiplier = (2*(((freqScanCount+1)%4)/2)-1)*(2*((freqScanCount+1)%2)-1)*((freqScanCount+1)/2); }
                    else { freqMultiplier = (2*((freqScanCount%4)/2)-1)*(2*(freqScanCount%2)-1)*(freqScanCount/2+0.5); }
                }
                else if (freqScanDirectionComboBox->currentIndex() == 3) { // scan frequency from edge to center
                    freqScanRef = (freqScanInit + freqScanFinal)/2;
                    if (freqScanNum%2) { freqMultiplier = (2*(((freqScanNum-freqScanCount)%4)/2)-1)*(2*((freqScanNum-freqScanCount)%2)-1)*((freqScanNum-freqScanCount)/2); }
                    else { freqMultiplier = (2*(((freqScanNum-freqScanCount-1)%4)/2)-1)*(2*((freqScanNum-freqScanCount-1)%2)-1)*((freqScanNum-freqScanCount-1)/2+0.5); }
                }
                if (freqScanMasterComboBox->currentIndex() == 0) { freqScanMaster = 0; freqScanNDiv1 = 8; freqScanNDiv2 = 64; } // Master F=4 to F'=4co5 (Reference)
                else if (freqScanMasterComboBox->currentIndex() == 1) { freqScanMaster = -125500000; freqScanNDiv1 = 8; freqScanNDiv2 = 64;  }  // Master F=4 to F'=5
                else if (freqScanMasterComboBox->currentIndex() == 2) { freqScanMaster = 125500000+201240000+75605000-9192631770; freqScanNDiv1 = 64; freqScanNDiv2 = 8;  }  // Master F=3 to F'=2co3
                else if (freqScanMasterComboBox->currentIndex() == 3) { freqScanMaster = 125500000+201240000-9192631770; freqScanNDiv1 = 64; freqScanNDiv2 = 8;  }  // Master F=3 to F'=3

                switch(freqScanAOMComboBox->currentIndex()) {
                    case 0:
                        freqScanAOM = -80*2;  break; // +80MHz x2
                    case 1:
                        freqScanAOM = 80*2;  break; // -80MHz x2
                    case 2:
                        freqScanAOM = -80*3;  break; // +80MHz x3
                    case 3:
                        freqScanAOM = -200*2;  break; // +200MHz x2
                    case 4:
                        freqScanAOM = 200*2;  break; // -200MHz x2
                }

                if (freqScanSlaveComboBox->currentIndex() == 0) { // F=4 - F'=5
                    freqScanDetune = abs((int)(freqScanDivider*(125500000+freqScanMaster+(freqScanAOM+freqScanRef+freqMultiplier*freqScanStep)*1000000)/freqScanNDiv1));
                }
                else if (freqScanSlaveComboBox->currentIndex() == 1) { // F=4 - F'=4
                    freqScanDetune = abs((int)(freqScanDivider*(-125500000+freqScanMaster+(freqScanAOM+freqScanRef+freqMultiplier*freqScanStep)*1000000)/freqScanNDiv1));
                }
                else if (freqScanSlaveComboBox->currentIndex() == 2) { // F=4 - F'=3
                    freqScanDetune = abs((int)(freqScanDivider*(-125500000+freqScanMaster-201240000+(freqScanAOM+freqScanRef+freqMultiplier*freqScanStep)*1000000)/freqScanNDiv1));
                }
                else if (freqScanSlaveComboBox->currentIndex() == 3) { // F=3 - F'=4
                    freqScanDetune = abs((int)(freqScanDivider*(9192631770-125500000+freqScanMaster+(freqScanAOM+freqScanRef+freqMultiplier*freqScanStep)*1000000)/freqScanNDiv2));
                }
                else if (freqScanSlaveComboBox->currentIndex() == 4) { // F=3 - F'=3
                    freqScanDetune = abs((int)(freqScanDivider*(9192631770-125500000+freqScanMaster-201240000+(freqScanAOM+freqScanRef+freqMultiplier*freqScanStep)*1000000)/freqScanNDiv2));
                }
                else if (freqScanSlaveComboBox->currentIndex() == 5) { // F=3 - F'=2
                    freqScanDetune = abs((int)(freqScanDivider*(9192631770-125500000+freqScanMaster-201240000-151210000+(freqScanAOM+freqScanRef+freqMultiplier*freqScanStep)*1000000)/freqScanNDiv2));
                }
                serialChannelObj[freqScanTerminalComboBox->currentIndex()]->SRsetLine[freqScanTimeComboBox->currentIndex()]->setText(QString::number(freqScanDetune));

                QCoreApplication::processEvents(); // Let events process (e.g. StopButton pressed)
                startSingleRun();
                freqScanProgressLine->setText(QString::number(freqScanCount+1)+"/"+QString::number(freqScanNum));
                QCoreApplication::processEvents(); // Let events processed (e.g. StopButton pressed)
                QThread::msleep((int) 1000*freqScanPeriod);
            }

            if (stopFlag == false) {
                time_t rawtime;
                struct tm * timeinfo;
                time ( &rawtime );
                timeinfo = localtime ( &rawtime );

                char fileName[70];
                sprintf ( fileName, "%02d%02d%02d_%02d%02d%02d_fscan.txt", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,  timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
                FILE *fp;
                fp = fopen(fileName, "w");
                freqScanMasterText = freqScanMasterComboBox->currentText();
                freqScanSlaveText = freqScanSlaveComboBox->currentText();
                freqScanMasterBA = freqScanMasterText.toLatin1();
                freqScanSlaveBA = freqScanSlaveText.toLatin1();
                fprintf(fp, "Master %s, Slave %s\n",freqScanMasterBA.data(),freqScanSlaveBA.data());
                fprintf(fp, "Detuning (MHz)\tCount\n");
                int i;
                for(i=0; i<freqScanNum; i++) {
                   fprintf(fp, "%.5f\t%d\n", detuneArray[i], countDataArray[i]);
                }
                fclose(fp);
            }

            delete[] detuneArray;
            delete[] countDataArray;
        }
        QCoreApplication::processEvents(); // Let events process (e.g. StopButton pressed)
        QThread::msleep(50);
        if (stopFlag) { break; }
        repeatProgressLine->setText(QString::number(repeatCount+1)+"/"+QString::number(repeatNumSpinBox->value()));
    } // End of the loop for 'repeat'
}

/*
void MainWindow::setFreqText(int detune)
{
    serialChannelObj[1]->SRsetLine[1]->setText(QString::number(detune));
}
*/

void MainWindow::startSingleRun()
{
    statusBar()->showMessage(tr("Running"));
    int i, j, k, l, m, temporalBit;
    int refClockSpeed = clockSpeedSpinBox->value();
    P0flag = false;
    activeAOchannelNum = 0;
    activeAIchannelNum = 0;
    lastTime = 0;

    for(i=0; i<serialChannelNum; i++) {
        if(serialChannelObj[i]->channelCheckBox->isChecked()) {
            if(lastTime < serialChannelObj[i]->lastTime) { lastTime = serialChannelObj[i]->lastTime; }
        }
        P0flag = P0flag || (serialChannelObj[i]->channelCheckBox->isChecked());
    }

    for(i=DOBoxPortNum; i<DOBoxPortNum+pulseChannelNum; i++) {
        if(pulseChannelObj[i]->channelCheckBox->isChecked()) {
            if(lastTime < pulseChannelObj[i]->lastTime) { lastTime = pulseChannelObj[i]->lastTime; }
        }
        P0flag = P0flag || (pulseChannelObj[i]->channelCheckBox->isChecked());
    }

    for(i=0; i<analogOutChannelNum; i++) {
        if(analogOutChannelObj[i]->channelCheckBox->isChecked()) {
            if(lastTime < analogOutChannelObj[i]->lastTime) { lastTime = analogOutChannelObj[i]->lastTime; }
            if(activeAOchannelNum==0) { strcpy(AOchan,analogOutChannelObj[i]->chan); }
            else {
                 strcat(AOchan,",");
                 strcat(AOchan,analogOutChannelObj[i]->chan);
            }
            activeAOchannelNum++;
        }
    }

    for(i=0; i<analogInChannelNum; i++) {
        if(analogInChannelObj[i]->channelCheckBox->isChecked()) {
            if(activeAIchannelNum==0) { strcpy(AIchan,analogInChannelObj[i]->chan); }
            else {
                 strcat(AIchan,",");
                 strcat(AIchan,analogInChannelObj[i]->chan);
            }
            activeAIchannelNum++;
       }
    }

    for(i=0; i<counterChannelNum; i++) { P0flag = P0flag || (counterChannelObj[i]->channelCheckBox->isChecked()); }

    // ******************************* Create Task Handles *************************************
    if(P0flag) {
        P0taskHandle = 0;
        DAQmxCreateTask("",&P0taskHandle);
        DAQmxCreateDOChan(P0taskHandle,"Dev1/port0","",DAQmx_Val_ChanForAllLines);
        if(counterChannelObj[0]->channelCheckBox->isChecked()) {
            CNTdataSize = (lastTime/counterChannelObj[0]->CNTrateSpinBox->value()+1);
            // CNTdataSize = (lastTime/counterChannelObj[0]->CNTrateSpinBox->value());
        }
    }

    if(activeAIchannelNum != 0) {
        AItaskHandle = 0;
        DAQmxCreateTask("",&AItaskHandle);
        DAQmxCreateAIVoltageChan(AItaskHandle,AIchan,"",DAQmx_Val_Cfg_Default,minVoltage,maxVoltage,DAQmx_Val_Volts,"");
    }

    if(activeAOchannelNum != 0) {
        AOtaskHandle = 0;
        DAQmxCreateTask("",&AOtaskHandle);
        DAQmxCreateAOVoltageChan(AOtaskHandle,AOchan,"",minVoltage,maxVoltage,DAQmx_Val_Volts,NULL);
    }

    if(counterChannelObj[0]->channelCheckBox->isChecked()) {
        CNTtaskHandle = 0;
        DAQmxCreateTask("",&CNTtaskHandle);
        // DAQmxCreateCICountEdgesChan(CNTtaskHandle,counterChannelObj[0]->chan,"",DAQmx_Val_Rising,0,DAQmx_Val_CountUp);
        DAQmxCreateCICountEdgesChan(CNTtaskHandle,"Dev1/ctr0","",DAQmx_Val_Rising,0,DAQmx_Val_CountUp);
    }

    // ******************************** Prepare Parameters *************************************
    dataSize = lastTime/refClockSpeed + 1;
    // dataSize = lastTime/(refClockSpeed/1000) + 1; // modification for Jeremy's measurement

    float64 rate = 1000000/refClockSpeed; // 1000000 us = 1s
    // int32 CNTnumRead;
    // int32 AInumRead;
    uInt64 sampsPerChanToAcquire = dataSize;
    int32	numSampsPerChan = dataSize;
    uInt32 arraySizeInSamps = activeAIchannelNum * dataSize;


    // ***************** Generate Sample Clock Timing / Create and Write Data Array **********************
    DAQmxCfgSampClkTiming(AItaskHandle,"OnboardClock",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,sampsPerChanToAcquire);
    AIdata = new float64[activeAIchannelNum * dataSize];

    /*
    DAQmxCfgSampClkTiming(analogInChannelObj[0]->AItaskHandle,"OnboardClock",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,sampsPerChanToAcquire);
    analogInChannelObj[0]->AIdata = new float64[dataSize];

    for(i=1; i<analogInChannelNum; i++) {
      if(analogInChannelObj[i]->channelCheckBox->isChecked()) {
        DAQmxCfgSampClkTiming(analogInChannelObj[i]->AItaskHandle,"/Dev1/ai/SampleClock",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,sampsPerChanToAcquire);
        analogInChannelObj[i]->AIdata = new float64[dataSize];
      }
    }
    */


    // **************************** Generate Pulse Data Array ***************************************
    if(P0flag) {
      DAQmxCfgSampClkTiming(P0taskHandle,"/Dev1/ai/SampleClock",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,sampsPerChanToAcquire);
      P0data = new uInt32[dataSize];
      j = 0 ;
      while(j<dataSize) {
          P0data[j] = pulseChannelObj[8]->readP0dataSingle();
          j++;
      }

      for(i=0; i<serialChannelNum; i++) {
          temporalBit = 1 << i;
          if(serialChannelObj[i]->channelCheckBox->isChecked()) {
              SRsetString = new QString[(serialChannelObj[i]->pulseCount)+1];
              P0timeIndex = new int[(serialChannelObj[i]->pulseCount)+1];

              P0timeIndex[0] = 0;

              for(k=0; k < (serialChannelObj[i]->pulseCount)+1; k++) {
                  if (serialChannelObj[i]->ddsCheckBox->isChecked()) {
                      SRsetString[k] = serialChannelObj[i]->detuneToSerial(serialChannelObj[i]->SRsetLine[k]->text());
                  }
                  else {
                      SRsetString[k] = serialChannelObj[i]->SRsetLine[k]->text();
                      SRsetString[k].replace(QString("\\r"),QString("\r"));
                  }

                  P0timeIndex[k] = (serialChannelObj[i]->P0timeLine[k]->text().toInt())/refClockSpeed;
              }

              // ********************** Fill the serial data array, POdata[j] ***********************
              j = 0;
              while(j<dataSize) {
                   P0data[j] ^= (-1 ^ P0data[j]) & temporalBit; // Set idle state high
                   // P0data[j] ^= (-1 ^ P0data[j]) & (1<<10); // Duplicate the serial command on P0.10 // TEMPORAL
                   j++;
              }
              for(k=0; k < (serialChannelObj[i]->pulseCount)+1; k++) {
                  QByteArray array = SRsetString[k].toLocal8Bit();
                  char* command = array.data();
                  SRsetStrLen = strlen(command);
                  SRcommandBit = new int[10*SRsetStrLen];
                  int p;
                  for(j=0; j < SRsetStrLen; j++) {
                      SRcommandBit[10*j] = 0;
                      for(p=0; p<8; p++) {
                        SRcommandBit[10*j+p+1] = (command[j]>>p)&(0x00001);
                      }
                      SRcommandBit[10*j+9] = 1;
                  }

                  // j = P0timeIndex[k]-(10*SRsetStrLen*4+(10*SRsetStrLen+2)/3); // Set the last bit of signal on the P0timeIndex
                  j = P0timeIndex[k]-(10*SRsetStrLen*9-(10*SRsetStrLen+2)/3); // Set the last bit of signal on the P0timeIndex
                  m = 0;
                  for(l=0;l<(10*SRsetStrLen);l++){
                      if(l%3) {
                        // for(p=0; p<4; p++){
                        for(p=0; p<9; p++){
                          P0data[j+m] ^= (-SRcommandBit[l] ^ P0data[j+m]) & temporalBit;
                          P0data[j+m] ^= (-SRcommandBit[l] ^ P0data[j+m]) & (1<<10); // Duplicate the serial command on P0.10
                          m++;
                        }
                      }
                      else {
                        // for(p=0; p<5; p++){
                        for(p=0; p<8; p++){
                          P0data[j+m] ^= (-SRcommandBit[l] ^ P0data[j+m]) & temporalBit;
                          P0data[j+m] ^= (-SRcommandBit[l] ^ P0data[j+m]) & (1<<10); // Duplicate the serial command on P0.10
                          m++;
                        }
                      }
                  }
                  delete[] SRcommandBit;
              }
              // ***********************************************************************************
              delete[] SRsetString;
              delete[] P0timeIndex;
          }
          else {
              for(j=0; j < dataSize; j++) { P0data[j] ^= (-1 ^ P0data[j]) & temporalBit; } // Set all off-channels high
          }
      }

      for(i=DOBoxPortNum; i<DOBoxPortNum+pulseChannelNum; i++) {
          temporalBit = 1 << i;
          P0idleInt = pulseChannelObj[i]->P0idleComboBox->currentIndex();

          if(pulseChannelObj[i]->channelCheckBox->isChecked()) {
              P0stateInt = new int[(pulseChannelObj[i]->pulseCount)+1];
              P0timeIndex = new int[(pulseChannelObj[i]->pulseCount)+1];

              for(k=0; k < (pulseChannelObj[i]->pulseCount)+1; k++) {
                  if((k%2)==0) { P0stateInt[k] = 1 - P0idleInt; }
                  else { P0stateInt[k] = P0idleInt; }
                  P0timeIndex[k] = (pulseChannelObj[i]->P0timeLine[k]->text().toInt())/refClockSpeed;
              }

              // *********************** Fill the data array, P0data[j] ****************************
              j = 0;
              while(j<P0timeIndex[0]) { // Assign 'High' or 'Low' to each time section
                  P0data[j] ^= (-P0idleInt ^ P0data[j]) & temporalBit;
                  j++;
              }
              for(k=1; k < (pulseChannelObj[i]->pulseCount)+1; k++) {
                  while(j<P0timeIndex[k]) {
                    P0data[j] ^= (-P0stateInt[k-1] ^ P0data[j]) & temporalBit;
                    j++;
                  }
              }
              while(j<dataSize) {
                  P0data[j] ^= (-P0stateInt[k-1] ^ P0data[j]) & temporalBit;
                  j++;
              }
              // ***********************************************************************************
              delete[] P0stateInt;
              delete[] P0timeIndex;
          }
          else {
          //    for(j=0; j < dataSize; j++) { P0data[j] ^= (-P0idleInt ^ P0data[j]) & temporalBit; }
          }
      }
      //pulseChannelObj[8]->writeP0dataSingle(P0data[dataSize-1]); // update the last state to P0dataSingle
    }

    // **************************** Generate Analog Data Array ***************************************
    AOdata = new float64[activeAOchannelNum * dataSize];
    DAQmxCfgSampClkTiming(AOtaskHandle,"/Dev1/ai/SampleClock",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,sampsPerChanToAcquire);
    l = 0;
    j = 0;
    for(i=0; i<analogOutChannelNum; i++) {
        if(analogOutChannelObj[i]->channelCheckBox->isChecked()) {
            // DAQmxCfgSampClkTiming(analogOutChannelObj[i]->AOtaskHandle,"/Dev1/ai/SampleClock",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,sampsPerChanToAcquire);

            // analogOutChannelObj[i]->AOdata = new float64[dataSize];

            AOidleFloat = analogOutChannelObj[i]->AOidleLine->text().toFloat();
            AOsetFloat = new float[(analogOutChannelObj[i]->analogOutCount)+1];
            AOtimeIndex = new int[(analogOutChannelObj[i]->analogOutCount)+1];

            AOsetFloat[0] = AOidleFloat; // for the case that all set values are off
            AOtimeIndex[0] = 0;

            for(k=0; k < (analogOutChannelObj[i]->analogOutCount)+1; k++) {
                AOsetFloat[k] = analogOutChannelObj[i]->AOsetLine[k]->text().toFloat();
                AOtimeIndex[k] = (analogOutChannelObj[i]->AOtimeLine[k]->text().toInt())/refClockSpeed;
            }

            // *********************** Fill the data array, AOdata[j] ****************************
            while(j<(l * dataSize + AOtimeIndex[0])) {
                AOdata[j] = AOidleFloat;
                j++;
            }
            for(k=1; k < (analogOutChannelObj[i]->analogOutCount)+1; k++) {
                while(j< (l * dataSize + AOtimeIndex[k])) {
                    AOdata[j] = AOsetFloat[(k-1)];
                    j++;
                }
            }
            while(j<((l+1)*dataSize)) {
                AOdata[j] = AOsetFloat[(k-1)];
                j++;
            }
            // ***********************************************************************************
            delete[] AOsetFloat;
            delete[] AOtimeIndex;
            l++;
        }
    }

    if(counterChannelObj[0]->channelCheckBox->isChecked()) {

        // ****************** Generate External Clock for counter on P0 ************************
        int zero = 0;
        int one = 1;
        temporalBit = 1 << cntTrigPort;  // P0.19 is used for the external clock; P0.19 to P2.1
        CNTclockSpeed = (counterChannelObj[0]->CNTrateSpinBox->value())/refClockSpeed;
        for(j=0; j < dataSize; j++) {
          P0data[j] ^= (-zero ^ P0data[j]) & temporalBit; // Set all zero
        }
        for(j=0; j < dataSize; j+=CNTclockSpeed) {
          P0data[j] ^= (-one ^ P0data[j]) & temporalBit;  // Set the clock signal one
        }
        CNTdata = new uInt32[CNTdataSize];
        DAQmxCfgSampClkTiming(CNTtaskHandle,"/Dev1/PFI9",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,CNTdataSize);
    }

    // **************************** Write DAQmx Code *******************************
    if(P0flag) {
      DAQmxWriteDigitalU32(P0taskHandle,numSampsPerChan,0,timeout,DAQmx_Val_GroupByChannel,P0data,NULL,NULL);
    }

    if(activeAOchannelNum != 0)
    { DAQmxWriteAnalogF64(AOtaskHandle,numSampsPerChan,FALSE,timeout,DAQmx_Val_GroupByChannel,AOdata,NULL,NULL); }

    // *****************************************************************************

    // **************************** Start DAQmx Code *******************************
    if(P0flag) { DAQmxStartTask(P0taskHandle); }

    if(activeAOchannelNum != 0) { DAQmxStartTask(AOtaskHandle); }

    if(counterChannelObj[0]->channelCheckBox->isChecked()) { DAQmxStartTask(CNTtaskHandle); }

    if(activeAIchannelNum != 0) { DAQmxStartTask(AItaskHandle); }

    // ******************************************************************************

    // ********************** Write DAQmx Code (Counter, AI) ***************************
    if(counterChannelObj[0]->channelCheckBox->isChecked()) {
        DAQmxReadCounterU32(CNTtaskHandle,CNTdataSize,timeout,CNTdata,CNTdataSize,&CNTread,NULL);
    }
    if(activeAIchannelNum != 0)
    { DAQmxReadAnalogF64(AItaskHandle,numSampsPerChan,timeout,DAQmx_Val_GroupByChannel,AIdata,arraySizeInSamps,NULL,NULL); }

    // ****************** End of  Write DAQmx Code (Counter, AI) ***********************

    /********************** Save SPCM count for frequency scan *********************/
    if ((freqScanControlComboBox->currentIndex() == 1) & (counterChannelObj[0]->channelCheckBox->isChecked())) {
        detuneArray[freqScanCount] = freqScanRef+freqMultiplier*freqScanStep;
        int ch = DOBoxPortNum + counterChannelObj[0]->CNTrefTimeComboBox->currentIndex();
        int tStart = counterChannelObj[0]->CNTstartComboBox->currentIndex();
        int tEnd = counterChannelObj[0]->CNTendComboBox->currentIndex();
        int CNTrate = counterChannelObj[0]->CNTrateSpinBox->value();
        countDataArray[freqScanCount] = CNTdata[(pulseChannelObj[ch]->P0timeLine[tEnd]->text().toInt())/CNTrate] - CNTdata[(pulseChannelObj[ch]->P0timeLine[tStart]->text().toInt())/CNTrate];
        // countDataArray[freqScanCount] = CNTdata[(pulseChannelObj[ch]->P0timeLine[tEnd]->text().toInt())/(counterChannelObj[0]->CNTrateSpinBox->value())] - CNTdata[(pulseChannelObj[ch]->P0timeLine[tStart]->text().toInt())/(counterChannelObj[0]->CNTrateSpinBox->value())];
    }
    /*******************************************************************************/

    /*********************************** Plot Data ***********************************/
    for (i=0; i<plotDockNum; i++) {
        int m = plotDockObj[i]->plotInComboBox->currentIndex();
        if(m != 0) {
             k = plotDockObj[i]->xAxisTime1ComboBox->currentIndex();
             l = plotDockObj[i]->xAxisTime2ComboBox->currentIndex();
             if (m < (analogInChannelNum+1)) { // plot analog in
                 // Needs to be filled
                 // plotDockObj[i]->plotDockUpdate();
             }
             else if (m < analogInChannelNum+counterChannelNum + 1) {  // plot counter
                 if(counterChannelObj[m-1-analogInChannelNum]->channelCheckBox->isChecked()) {
                     xLowLim = 0;
                     xHighLim = 0;
                     for (j = 0; j < serialChannelNum; j++) {
                         if ( (plotDockObj[i]->xAxisChannelComboBox->currentIndex() != 0) && (serialChannelObj[j]->channelTerminalLabel->text() == plotDockObj[i]->xAxisChannelComboBox->currentText()) ) {
                             xLowLim = (serialChannelObj[j]->P0timeLine[k]->text().toDouble())/1000; // low limit of x axis [ms]
                             xHighLim = (serialChannelObj[j]->P0timeLine[l]->text().toDouble())/1000; // high limit of x axis [ms]
                         }
                     }
                     for (j = DOBoxPortNum; j < DOBoxPortNum + pulseChannelNum; j++) {
                         if ( (plotDockObj[i]->xAxisChannelComboBox->currentIndex() != 0) && (pulseChannelObj[j]->channelTerminalLabel->text() == plotDockObj[i]->xAxisChannelComboBox->currentText()) ) {
                             xLowLim = (pulseChannelObj[j]->P0timeLine[k]->text().toDouble())/1000; // low limit of x axis [ms]
                             xHighLim = (pulseChannelObj[j]->P0timeLine[l]->text().toDouble())/1000; // high limit of x axis [ms]
                         }
                     }
                     for (j = 0; j < analogOutChannelNum; j++) {
                         if ( (plotDockObj[i]->xAxisChannelComboBox->currentIndex() != 0) && (analogOutChannelObj[j]->channelTerminalLabel->text() == plotDockObj[i]->xAxisChannelComboBox->currentText()) ) {
                             xLowLim = (analogOutChannelObj[j]->AOtimeLine[k]->text().toDouble())/1000; // low limit of x axis [ms]
                             xHighLim = (analogOutChannelObj[j]->AOtimeLine[l]->text().toDouble())/1000; // high limit of x axis [ms]
                         }
                     }
                     plotDockObj[i]->plotDockUpdate(xLowLim, xHighLim, CNTdataSize, counterChannelObj[m-1-analogInChannelNum]->CNTrateSpinBox->value(),CNTdata);
                 }
             }
             else {  // plot Frequency Scan
                 // plotDockObj[i]->plotFreqScan(freqScanInit, freqScanCount+1, freqScanNum, freqScanStep, countDataArray);
                 plotDockObj[i]->plotFreqScan(freqScanInit, freqScanCount+1, freqScanNum, freqScanStep, detuneArray, countDataArray);
             }
        }
    }
    /****************************** End of Plot Data ********************************/

    // ************************* Write Data to File *********************************

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    /*
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_hh:mm:ss");
    // fileName.append(".txt");
    // QString fileName = "DATA.txt";

    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << "something" << endl;
    }
    */

    if(writeDataCheckBox->isChecked()) {
        char fileName[70];
        // sprintf ( fileName, "C:\\Users\\Taehyun\\Documents\\Work\\%02d%02d%02d_%02d:%02d:%02d.txt", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,  timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
        sprintf ( fileName, "%02d%02d%02d_%02d%02d%02d.txt", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,  timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );

        FILE *fp;
        fp = fopen(fileName, "w");

        l = 0;
        /****************** Analog In Writing Off temporally *******************/
        for(i=0; i<analogInChannelNum; i++) {
            if(analogInChannelObj[i]->channelCheckBox->isChecked()) {
                fprintf(fp, "AI%d - %s\n", i, analogInChannelObj[i]->chanTitle);
                fprintf(fp, "time(us)\t read\n");
                // fprintf(fp, "time(ms)\t read\n");  // modification for Jeremy's measurement

                AItimeIndex = analogInChannelObj[i]->AIrateSpinBox->value()/refClockSpeed;
                for(j=l*dataSize; j<(l+1)*dataSize; j+=AItimeIndex) {
                    fprintf(fp, "%d\t%f\n", (j-(l*dataSize)) * refClockSpeed, AIdata[j]);
                    // fprintf(fp, "%d\t%f\n", (j-(l*dataSize)) * (refClockSpeed/1000), AIdata[j]);    // modification for Jeremy's measurement
                }
                fprintf(fp,"\n");
                l++;
            }
        }
        /***********************************************************************/

        // fp.write(str(yourQTextEdit.toPlainText()))
        // QString str1 = noteText->toPlainText();
        if(writeNoteCheckBox->isChecked()) {
            QByteArray noteByteArray = (noteText->toPlainText()).toLatin1();
            const char *noteCString = noteByteArray.data();
            fprintf(fp, "%s\n\n", noteCString);
        }

        if(counterChannelObj[0]->channelCheckBox->isChecked()) {
            // int a = counterChannelObj[i]->CNTdata[j];
            for(j=1; j<CNTdataSize; j++) {
                fprintf(fp, "%d\t%d\n", j * (counterChannelObj[0]->CNTrateSpinBox->value()), (unsigned int)(CNTdata[j]-CNTdata[j-1]));
            }
        }
        fclose(fp);
    }

    // *********************** Debug line ******************************
    // statusBar()->showMessage(QString::number(CNTclockSpeed));
    // statusBar()->showMessage(QString::number((P0data[2] & temporalBit) >> (pulseChannelNum+i)));
    // statusBar()->showMessage(QString::number(lastTime));
    // *****************************************************************

    // ********************** Delete dynamically allocated memory ********************
    if(P0flag) {
        DAQmxStopTask(P0taskHandle);
        DAQmxClearTask(P0taskHandle);
        delete[] P0data;
    }
    if(activeAIchannelNum != 0) {
        DAQmxStopTask(AItaskHandle);
        DAQmxClearTask(AItaskHandle);
        delete[] AIdata;
    }
    if(activeAOchannelNum != 0) {
        DAQmxStopTask(AOtaskHandle);
        DAQmxClearTask(AOtaskHandle);
        delete[] AOdata;
    }
    if(counterChannelObj[0]->channelCheckBox->isChecked()) {
        DAQmxStopTask(CNTtaskHandle);
        DAQmxClearTask(CNTtaskHandle);
        delete[] CNTdata;
    }

    pulseChannelObj[8]->P0idleComboBoxChanged(); // Set the P0 channel to the idle state

    statusBar()->showMessage(tr("Ready"));
}
  
void MainWindow::stopButtonPressed()
{
  stopFlag = true;
  statusBar()->showMessage(tr("Ready"));
}


void MainWindow::createChannelGroupBox()
{
    channelGroupBox = new QGroupBox(tr("Channels"));
    int i, j, k;
    // *************** Titles for AI terminal **************** //
    QLabel *AIterminalTitle = new QLabel(tr("Terminal"));
    AIterminalTitle->setAlignment(Qt::AlignCenter);
    QLabel *AItitleTitle = new QLabel(tr("Title"));
    AItitleTitle->setAlignment(Qt::AlignCenter);
    QLabel *AIrateTitle = new QLabel(tr("Rate"));
    AIrateTitle->setAlignment(Qt::AlignCenter);
    // QLabel *AIconvertTitle = new QLabel(tr("Convert"));
    // AIconvertTitle->setAlignment(Qt::AlignCenter);

    // *************** Titles for CNT terminal **************** //
    QLabel *CNTterminalTitle = new QLabel(tr("Terminal"));
    CNTterminalTitle->setAlignment(Qt::AlignCenter);
    QLabel *CNTtitleTitle = new QLabel(tr("Title"));
    CNTtitleTitle->setAlignment(Qt::AlignCenter);
    QLabel *CNTrateTitle = new QLabel(tr("Rate"));
    CNTrateTitle->setAlignment(Qt::AlignCenter);
    QLabel *CNTrefTimeTitle = new QLabel(tr("Ref Time"));
    CNTrefTimeTitle->setAlignment(Qt::AlignCenter);
    QLabel *CNTstartTitle = new QLabel(tr("Start"));
    CNTstartTitle->setAlignment(Qt::AlignCenter);
    QLabel *CNTendTitle = new QLabel(tr("End"));
    CNTendTitle->setAlignment(Qt::AlignCenter);
    // QLabel *CNTconvertTitle = new QLabel(tr("Convert"));
    // CNTconvertTitle->setAlignment(Qt::AlignCenter);

    // *************** Titles for Serial terminal **************** //
    QLabel *SRterminalTitle = new QLabel(tr("Terminal"));
    SRterminalTitle->setAlignment(Qt::AlignCenter);
    QLabel *SRtitleTitle = new QLabel(tr("Title"));
    SRtitleTitle->setAlignment(Qt::AlignCenter);
    QLabel *SRddsTitle = new QLabel(tr("DDS"));
    SRddsTitle->setAlignment(Qt::AlignCenter);
    QLabel *SRidleTitle = new QLabel(tr("Idle"));
    SRidleTitle->setAlignment(Qt::AlignCenter);
    QLabel *SRsetTitle[analogOutNum];
    QLabel *SRtimeTitle[analogOutNum];
    for(k=0; k<analogOutNum; k++){
      SRsetTitle[k] = new QLabel(tr("Command"));
      SRsetTitle[k]->setAlignment(Qt::AlignCenter);
      SRtimeTitle[k] = new QLabel(tr("Time (us)"));
      SRtimeTitle[k]->setAlignment(Qt::AlignCenter);
    }


    // *************** Titles for P0 terminal **************** //
    QLabel *P0terminalTitle = new QLabel(tr("Terminal"));
    P0terminalTitle->setAlignment(Qt::AlignCenter);
    QLabel *P0titleTitle = new QLabel(tr("Title"));
    P0titleTitle->setAlignment(Qt::AlignCenter);
    QLabel *P0idleTitle = new QLabel(tr("Idle"));
    P0idleTitle->setAlignment(Qt::AlignCenter);
    QLabel *P0stateTitle[pulseNum];
    QLabel *P0timeTitle[pulseNum];
    for(k=0; k<pulseNum; k++){
      P0stateTitle[k] = new QLabel(tr("State"));
      P0stateTitle[k]->setAlignment(Qt::AlignCenter);
      P0timeTitle[k] = new QLabel(tr("Time (us)"));
      // P0timeTitle[k] = new QLabel(tr("Time (ms)"));  // modification for Jeremy's measurement
      P0timeTitle[k]->setAlignment(Qt::AlignCenter);
    }

    // *************** Titles for AO terminal **************** //
    QLabel *AOterminalTitle = new QLabel(tr("Terminal"));
    AOterminalTitle->setAlignment(Qt::AlignCenter);
    QLabel *AOtitleTitle = new QLabel(tr("Title"));
    AOtitleTitle->setAlignment(Qt::AlignCenter);
    QLabel *AOidleTitle = new QLabel(tr("Idle (V)"));
    AOidleTitle->setAlignment(Qt::AlignCenter);
    QLabel *AOsetTitle[analogOutNum];
    QLabel *AOtimeTitle[analogOutNum];
    for(k=0; k<analogOutNum; k++){
      AOsetTitle[k] = new QLabel(tr("Set (V)"));
      AOsetTitle[k]->setAlignment(Qt::AlignCenter);
      AOtimeTitle[k] = new QLabel(tr("Time (us)"));
      AOtimeTitle[k]->setAlignment(Qt::AlignCenter);
    }

    // *************** Generate each terminal *************** //
    for (i = 0; i < analogInChannelNum; i++) {
      analogInChannelObj[i] = new AnalogInChannelClass;
    }

    for (i = 0; i < counterChannelNum; i++) {
      counterChannelObj[i] = new CounterChannelClass;
    }

    for (i = 0; i < serialChannelNum; i++) {
      serialChannelObj[i] = new SerialChannelClass;
    }

    for (i = DOBoxPortNum; i < DOBoxPortNum+pulseChannelNum; i++) {
      pulseChannelObj[i] = new PulseChannelClass;
    }

    for (i = 0; i < analogOutChannelNum; i++) {
      analogOutChannelObj[i] = new AnalogOutChannelClass;
    }


    channelLayout= new QGridLayout;
    channelLayout->setAlignment(Qt::AlignCenter);

    channelLayout->addWidget(AIterminalTitle,0,1);
    channelLayout->addWidget(AItitleTitle,0,2);
    channelLayout->addWidget(AIrateTitle,0,4);

    for (i = 0; i < analogInChannelNum; i++) {
      j = i + 1;
      channelLayout->addWidget(analogInChannelObj[i]->channelCheckBox,j,0);
      channelLayout->addWidget(analogInChannelObj[i]->channelTerminalLabel,j,1);
      channelLayout->addWidget(analogInChannelObj[i]->channelNameLine,j,2);
      channelLayout->addWidget(analogInChannelObj[i]->AIrateSpinBox,j,4);
      //channelLayout->addWidget(analogInChannelObj[i]->AIconvertComboBox,j,4);
    }

    j = analogInChannelNum + 1;
    channelLayout->addWidget(CNTterminalTitle,j,1);
    channelLayout->addWidget(CNTtitleTitle,j,2);
    channelLayout->addWidget(CNTrateTitle,j,4);
    channelLayout->addWidget(CNTrefTimeTitle,j,6);
    channelLayout->addWidget(CNTstartTitle,j,7);
    channelLayout->addWidget(CNTendTitle,j,8);

    for (i = 0; i < counterChannelNum; i++) {
      j = analogInChannelNum + i + 2;
      channelLayout->addWidget(counterChannelObj[i]->channelCheckBox,j,0);
      channelLayout->addWidget(counterChannelObj[i]->channelTerminalLabel,j,1);
      channelLayout->addWidget(counterChannelObj[i]->channelNameLine,j,2);
      channelLayout->addWidget(counterChannelObj[i]->CNTrateSpinBox,j,4);
      channelLayout->addWidget(counterChannelObj[i]->CNTrefTimeComboBox,j,6);
      channelLayout->addWidget(counterChannelObj[i]->CNTstartComboBox,j,7);
      channelLayout->addWidget(counterChannelObj[i]->CNTendComboBox,j,8);
      int m = 0;
      for (m = DOBoxPortNum; m < DOBoxPortNum + pulseChannelNum; m++) {
        counterChannelObj[i]->CNTrefTimeComboBox->addItem(pulseChannelObj[m]->channelTerminalLabel->text());
      }
      for (m = 0; m < pulseNum; m++) {
        QString s = QString ("Time %1").arg(m);
        counterChannelObj[i]->CNTstartComboBox->addItem(s);
        counterChannelObj[i]->CNTendComboBox->addItem(s);
      }
    }

    j = analogInChannelNum + counterChannelNum + 2;
    channelLayout->addWidget(SRterminalTitle,j,1);
    channelLayout->addWidget(SRtitleTitle,j,2);
    channelLayout->addWidget(SRddsTitle,j,3);
    channelLayout->addWidget(SRidleTitle,j,4);
    for(k=0; k<pulseNum; k++){
      channelLayout->addWidget(SRsetTitle[k],j,5+2*k);
      channelLayout->addWidget(SRtimeTitle[k],j,5+2*k+1);
    }

    for (i = 0; i < serialChannelNum; i++) {
        if((i==2) or (i==3)) {}\
        else{
            j = analogInChannelNum + counterChannelNum + i + 3;
            channelLayout->addWidget(serialChannelObj[i]->channelCheckBox,j,0);
            channelLayout->addWidget(serialChannelObj[i]->channelTerminalLabel,j,1);
            channelLayout->addWidget(serialChannelObj[i]->channelNameLine,j,2);
            channelLayout->addWidget(serialChannelObj[i]->ddsCheckBox,j,3);
            channelLayout->addWidget(serialChannelObj[i]->SRidleLine,j,4);
            for(k=0; k<serialNum; k++){
                channelLayout->addWidget(serialChannelObj[i]->SRsetLine[k],j,5+2*k);
                channelLayout->addWidget(serialChannelObj[i]->P0timeLine[k],j,5+2*k+1);
            }
        }
    }

    j = analogInChannelNum + counterChannelNum + serialChannelNum + 3;
    channelLayout->addWidget(P0terminalTitle,j,1);
    channelLayout->addWidget(P0titleTitle,j,2);
    channelLayout->addWidget(P0idleTitle,j,4);
    for(k=0; k<pulseNum; k++){
      channelLayout->addWidget(P0stateTitle[k],j,5+2*k);
      channelLayout->addWidget(P0timeTitle[k],j,5+2*k+1);
    }

    for (i = DOBoxPortNum; i < DOBoxPortNum+pulseChannelNum; i++) {
      j = analogInChannelNum + counterChannelNum + serialChannelNum + i - DOBoxPortNum + 4;
      channelLayout->addWidget(pulseChannelObj[i]->channelCheckBox,j,0);
      channelLayout->addWidget(pulseChannelObj[i]->channelTerminalLabel,j,1);
      channelLayout->addWidget(pulseChannelObj[i]->channelNameLine,j,2);
      channelLayout->addWidget(pulseChannelObj[i]->P0idleComboBox,j,4);
      for(k=0; k<pulseNum; k++){
        channelLayout->addWidget(pulseChannelObj[i]->P0stateComboBox[k],j,5+2*k);
        channelLayout->addWidget(pulseChannelObj[i]->P0timeLine[k],j,5+2*k+1);
      }
    }

    j = analogInChannelNum + counterChannelNum + serialChannelNum + pulseChannelNum + 4;
    channelLayout->addWidget(AOterminalTitle,j,1);
    channelLayout->addWidget(AOtitleTitle,j,2);
    channelLayout->addWidget(AOidleTitle,j,4);
    for(k=0; k<analogOutNum; k++){
      channelLayout->addWidget(AOsetTitle[k],j,5+2*k);
      channelLayout->addWidget(AOtimeTitle[k],j,5+2*k+1);
    }
  //  channelLayout->addWidget(AOsetTitle,j,4);
  //  channelLayout->addWidget(signalOff1Title,j,5);
  //  channelLayout->addWidget(signalRampUpTitle,j,6);
  //  channelLayout->addWidget(signalOnTitle,j,7);
  //  channelLayout->addWidget(signalRampDownTitle,j,8);
  //  channelLayout->addWidget(signalOff2Title,j,9);

    for (i = 0; i < analogOutChannelNum; i++) {
      j = analogInChannelNum + counterChannelNum + serialChannelNum + pulseChannelNum + i + 5;
      channelLayout->addWidget(analogOutChannelObj[i]->channelCheckBox,j,0);
      channelLayout->addWidget(analogOutChannelObj[i]->channelTerminalLabel,j,1);
      channelLayout->addWidget(analogOutChannelObj[i]->channelNameLine,j,2);
      channelLayout->addWidget(analogOutChannelObj[i]->AOidleLine,j,4);
      for(k=0; k<analogOutNum; k++){
        channelLayout->addWidget(analogOutChannelObj[i]->AOsetLine[k],j,5+2*k);
        channelLayout->addWidget(analogOutChannelObj[i]->AOtimeLine[k],j,5+2*k+1);
      }
    }
    channelGroupBox->setLayout(channelLayout);
}

void MainWindow::createNoteGroupBox()
{
  noteGroupBox = new QGroupBox(tr("Measurement Note"));

  noteText = new QTextEdit;

  QLabel *writeNoteLabel = new QLabel(tr("Write"));
  writeNoteLabel->setAlignment(Qt::AlignCenter);

  writeNoteCheckBox = new QCheckBox();

  noteLayout= new QGridLayout;
  noteLayout->addWidget(writeNoteLabel,0,0);
  noteLayout->addWidget(writeNoteCheckBox,0,1);
  noteLayout->addWidget(noteText,0,2);

  noteGroupBox->setLayout(noteLayout);
}


/*
void MainWindow::mousePressEvent ( QMouseEvent * event )
{
    if(event->button() == Qt::LeftButton)
        emit leftClickAction();
    else if(event->button() == Qt::RightButton)
        emit rightClickAction();
    else if(event->button() == Qt::MidButton)
        emit midClickAction();
}


void MainWindow::print()
{
#ifndef QT_NO_PRINTDIALOG
    QTextDocument *document = textEdit->document();
    QPrinter printer;

    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    document->print(&printer);
    statusBar()->showMessage(tr("Ready"), 2000);
#endif
}
*/

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                        tr("Choose a file name"), ".",
                        tr("HTML (*.html *.htm)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Dock Widgets"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toHtml();
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
}

void MainWindow::undo()
{
    QTextDocument *document = textEdit->document();
    document->undo();
}

void MainWindow::insertCustomer(const QString &customer)
{
    if (customer.isEmpty())
        return;
    QStringList customerList = customer.split(", ");
    QTextDocument *document = textEdit->document();
    QTextCursor cursor = document->find("NAME");
    if (!cursor.isNull()) {
        cursor.beginEditBlock();
        cursor.insertText(customerList.at(0));
        QTextCursor oldcursor = cursor;
        cursor = document->find("ADDRESS");
        if (!cursor.isNull()) {
            for (int i = 1; i < customerList.size(); ++i) {
                cursor.insertBlock();
                cursor.insertText(customerList.at(i));
            }
            cursor.endEditBlock();
        }
        else
            oldcursor.endEditBlock();
    }
}

void MainWindow::addParagraph(const QString &paragraph)
{
  if (paragraph.isEmpty())
      return;
  QTextDocument *document = textEdit->document();
  QTextCursor cursor = document->find(tr("Yours sincerely,"));
  if (cursor.isNull())
      return;
  cursor.beginEditBlock();
  cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, 2);
  cursor.insertBlock();
  cursor.insertText(paragraph);
  cursor.insertBlock();
  cursor.endEditBlock();

}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Dock Widgets"),
            tr("The <b>Dock Widgets</b> example demonstrates how to "
               "use Qt's dock widgets. You can enter your own text, "
               "click a customer to add a customer name and "
               "address, and click standard paragraphs to add them."));
}

void MainWindow::createActions()
{

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current form letter"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(newLetterAct);
    fileMenu->addAction(saveAct);
//    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

/*
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newLetterAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
}
*/

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    int i;
    for(i=0; i < plotDockNum; i++) {
      QString dockString("Plot ");
      dockString.append(QString::number(i+1));
      dockObj[i] = new QDockWidget(dockString, this);
      plotDockObj[i] = new PlotDockClass();

      plotDockObj[i]->plotInComboBox->addItem("Off");
      plotDockObj[i]->xAxisChannelComboBox->addItem("0");
      int j;
      for (j = 0; j < analogInChannelNum; j++) {
        plotDockObj[i]->plotInComboBox->addItem(analogInChannelObj[j]->channelTerminalLabel->text());
        // plotDockObj[i]->xAxisChannelComboBox->addItem(analogInChannelObj[j]->channelTerminalLabel->text());
      }
      for (j = 0; j < counterChannelNum; j++) {
        plotDockObj[i]->plotInComboBox->addItem(counterChannelObj[j]->channelTerminalLabel->text());
        // plotDockObj[i]->xAxisChannelComboBox->addItem(counterChannelObj[j]->channelTerminalLabel->text());
      }
      for (j = 0; j < serialChannelNum; j++) {
        plotDockObj[i]->xAxisChannelComboBox->addItem(serialChannelObj[j]->channelTerminalLabel->text());
      }
      for (j = DOBoxPortNum; j < DOBoxPortNum + pulseChannelNum; j++) {
        plotDockObj[i]->xAxisChannelComboBox->addItem(pulseChannelObj[j]->channelTerminalLabel->text());
      }
      for (j = 0; j < analogOutChannelNum; j++) {
        plotDockObj[i]->xAxisChannelComboBox->addItem(analogOutChannelObj[j]->channelTerminalLabel->text());
      }
      for (j = 0; j < pulseNum; j++) {
        QString s = QString ("Time %1").arg(j);
        plotDockObj[i]->xAxisTime1ComboBox->addItem(s);
        plotDockObj[i]->xAxisTime2ComboBox->addItem(s);
      }

      connect(plotDockObj[i]->plotInComboBox,SIGNAL(currentIndexChanged(int)), plotDockObj[i], SLOT(plotInComboBoxChanged(int)));
      connect(plotDockObj[i]->xAxisChannelComboBox,SIGNAL(currentIndexChanged(int)), plotDockObj[i], SLOT(xAxisChannelComboBoxChanged(int)));

      // signalMapper[i] = new QSignalMapper (this);
      // connect(plotDockObj[i]->plotUpdateTimer, SIGNAL(timeout()),signalMapper[i], SLOT(map()));
      // signalMapper[i]->setMapping(plotDockObj[i]->plotUpdateTimer, i);
      // connect(signalMapper[i], SIGNAL(mapped(int)), this, SLOT(plotUpdate(int)));

      dockObj[i]->setWidget(plotDockObj[i]);
      addDockWidget(Qt::RightDockWidgetArea, dockObj[i]);
      viewMenu->addAction(dockObj[i]->toggleViewAction());
    }
  //  connect(plotInComboBox, SIGNAL(currentIndexChanged(int))),
  //          this, SLOT(plotInComboBoxChanged(int));
}

/*
void MainWindow::plotUpdate(int i)
{
  int m = plotDockObj[i]->plotInComboBox->currentIndex();
  if(m != 0) {
    plotDockObj[i]->plotDockUpdate(analogInChannelObj[m-1]->currentTime, analogInChannelObj[m-1]->data, analogInChannelObj[m-1]->convertedValue); 
  }
}
*/

void MainWindow::updateReadRate()
{
  int i;
  int t = clockSpeedSpinBox->value();
  int ref = clockSingleStep;

  if((t%ref)!=0) {
    t = analogInChannelObj[0]->getRound(t, ref);
    clockSpeedSpinBox->setValue(t);
  }
  analogInChannelObj[0]->setClockSpeed(t);

  for (i=0; i<analogInChannelNum; i++) {
    // analogInChannelObj[i]->AIrateSpinBox->setRange(1,500000);
    analogInChannelObj[i]->AIrateSpinBox->setSingleStep(t);
    analogInChannelObj[i]->AIrateSpinBoxChanged();
  }
  for (i=0; i<counterChannelNum; i++) {
    // counterChannelObj[i]->CNTrateSpinBox->setRange(1,500000);
    counterChannelObj[i]->CNTrateSpinBox->setSingleStep(t);
    counterChannelObj[i]->CNTrateSpinBoxChanged();
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_T:
            pulseChannelObj[10]->P0timeLine[1]->setFocus();
            pulseChannelObj[10]->P0timeLine[1]->selectAll();
            break;
        case Qt::Key_M:
            pulseChannelObj[11]->P0timeLine[1]->setFocus();
            pulseChannelObj[11]->P0timeLine[1]->selectAll();
            break;
        case Qt::Key_F:
            serialChannelObj[1]->SRsetLine[1]->setFocus();
            serialChannelObj[1]->SRsetLine[1]->selectAll();
            break;
        case Qt::Key_P:
            pulseChannelObj[9]->P0timeLine[0]->setFocus();
            pulseChannelObj[9]->P0timeLine[0]->selectAll();
            break;
        case Qt::Key_R:
            this->startButtonPressed();
            break;
        default:
            QWidget::keyPressEvent(event);
        }
    }
}

MainWindow::~MainWindow()
{

}
