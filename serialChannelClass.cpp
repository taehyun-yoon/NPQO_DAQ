#include "serialChannelClass.h"

SerialChannelClass::SerialChannelClass() : PulseChannelClass()
{
    int k;

    ddsCheckBox = new QCheckBox();
    ddsCheckBox->setEnabled(false);

    SRidleLine = new QLineEdit;
    SRidleLineText = "0";
    SRidleLine->setText(SRidleLineText);
    SRidleLine->setAlignment(Qt::AlignCenter);
    SRidleLine->setEnabled(false);

    for(k=0; k<serialNum; k++){
      SRsetLineText[k] = "Off";
      SRsetLine[k] = new QLineEdit;
      SRsetLine[k]->setText(SRsetLineText[k]);
      SRsetLine[k]->setAlignment(Qt::AlignCenter);
      SRsetLine[k]->setEnabled(false);
      SRsetLineFlag[k] = false;
    }

    // pulseCount = -1;

    /******************************** Signals to Slots ***************************************/
    connect(channelCheckBox,SIGNAL(toggled(bool)),this,SLOT(serialCheckBoxClicked()));
    connect(SRidleLine,SIGNAL(editingFinished()),this,SLOT(SRidleLineChanged()));

    QSignalMapper* signalMapper3 = new QSignalMapper (this) ;
    for(k=0; k<serialNum; k++) {
      connect(SRsetLine[k],SIGNAL(editingFinished()),signalMapper3,SLOT(map()));
      signalMapper3 -> setMapping (SRsetLine[k], k) ;
    }
    connect (signalMapper3, SIGNAL(mapped(int)), this, SLOT(SRsetLineChanged(int))) ;

    QSignalMapper* signalMapper4 = new QSignalMapper (this) ;
    for(k=0; k<serialNum; k++) {
      connect(P0timeLine[k],SIGNAL(editingFinished()),signalMapper4,SLOT(map()));
      signalMapper4 -> setMapping (P0timeLine[k], k) ;
    }
    connect (signalMapper4, SIGNAL(mapped(int)), this, SLOT(SRtimeLineChanged(int))) ;
}

void SerialChannelClass::serialCheckBoxClicked()
{
    int k;
    if (channelCheckBox->isChecked()) {
        channelNameLine->setEnabled(true);
        SRidleLine->setEnabled(true);
        ddsCheckBox->setEnabled(true);
        for(k=0; k<serialNum; k++){
            SRsetLine[k]->setEnabled(true);
            if(SRsetLineFlag[k] == true) {P0timeLine[k]->setEnabled(true);}
        }

        P0taskHandleSingle = 0;
        AItaskHandleSR = 0;
        samplesPerChan = 1;
        timeout = 10.0;
        /************************ Create TaskHandle **************************
        P0taskHandleSingle = 0;
        AItaskHandleSR = 0;
        samplesPerChan = 1;
        timeout = 10.0;

        DAQmxCreateTask("",&P0taskHandleSingle);
        DAQmxCreateTask("",&AItaskHandleSR);
        DAQmxCreateAIVoltageChan(AItaskHandleSR,"Dev1/ai0","",DAQmx_Val_Cfg_Default,minVoltage,maxVoltage,DAQmx_Val_Volts,"");
        DAQmxCreateDOChan(P0taskHandleSingle,chan,"",DAQmx_Val_ChanForAllLines);

        *********************************************************************/
    }
    else {
        channelNameLine->setEnabled(false);
        SRidleLine->setEnabled(false);
        ddsCheckBox->setEnabled(false);
        for(k=0; k<analogOutNum; k++){
          SRsetLine[k]->setEnabled(false);
          P0timeLine[k]->setEnabled(false);
        }
        /************************* Clear TaskHandle **************************
        DAQmxStopTask(P0taskHandleSingle);
        DAQmxStopTask(AItaskHandleSR);
        DAQmxClearTask(P0taskHandleSingle);
        DAQmxClearTask(AItaskHandleSR);
        *********************************************************************/
    }
}

void SerialChannelClass::SRidleLineChanged()
{
    // bool isNum = true;
    // float setValue = SRidleLine->text().toFloat(&isNum);
    if (ddsCheckBox->isChecked()) {
        SRidleLineText = detuneToSerial(SRidleLine->text());
    }
    else {
        SRidleLineText = SRidleLine->text();
        SRidleLineText.replace(QString("\\r"),QString("\r"));
    }

    // P0dataSingle ^= (-P0idleComboBox->currentIndex() ^ P0dataSingle) & (0x0001 << P0lineNum);
    genSerialCommand(SRidleLineText);

    /************************* Start TaskHandle **************************/
    uInt64 sampsPerChanToAcquire = 10*10*SRcommandLen;
    int32 numSampsPerChan = sampsPerChanToAcquire;
    uInt32 arraySizeInSamps = sampsPerChanToAcquire;
    float64 AIdataSingle[10*10*SRcommandLen];
    // int rate = 500000;
    int rate = 1000000;

    DAQmxCreateTask("",&P0taskHandleSingle);
    DAQmxCreateTask("",&AItaskHandleSR);
    DAQmxCreateAIVoltageChan(AItaskHandleSR,"Dev1/ai0","",DAQmx_Val_Cfg_Default,minVoltage,maxVoltage,DAQmx_Val_Volts,"");
    DAQmxCreateDOChan(P0taskHandleSingle,chan,"",DAQmx_Val_ChanForAllLines);

    DAQmxCfgSampClkTiming(AItaskHandleSR,"OnboardClock",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,sampsPerChanToAcquire);
    DAQmxCfgSampClkTiming(P0taskHandleSingle,"/Dev1/ai/SampleClock",rate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,sampsPerChanToAcquire);
    DAQmxWriteDigitalU32(P0taskHandleSingle,numSampsPerChan,0,timeout,DAQmx_Val_GroupByChannel,SRdataSingle,NULL,NULL);

    DAQmxStartTask(P0taskHandleSingle); // This line should be prior to the line of DAQmx StartTask(taskHandleAO))
    DAQmxStartTask(AItaskHandleSR);

    DAQmxReadAnalogF64(AItaskHandleSR,numSampsPerChan,timeout,DAQmx_Val_GroupByChannel,AIdataSingle,arraySizeInSamps,NULL,NULL);
    DAQmxStopTask(P0taskHandleSingle);
    DAQmxStopTask(AItaskHandleSR);
    DAQmxClearTask(P0taskHandleSingle);
    DAQmxClearTask(AItaskHandleSR);
    /*********************************************************************/
    delete[] SRdataSingle;
}


void SerialChannelClass::SRsetLineChanged(int k)
{
    int i;
    bool isNum = true;
    int ref = getClockSpeed();
    if ((SRsetLine[k]->text() == NULL) || ((SRsetLine[k]->text() == "Off") || (SRsetLine[k]->text() == "off")) ) { // When deactivated
        // Turn off all signals [k] and after [k]
        P0timeLine[k]->setEnabled(false);
        SRsetLine[k]->setText("Off");
        SRsetLineFlag[k] = false;
        for(i=k+1; i<serialNum; i++) {
            SRsetLine[i]->setText("Off");
            SRsetLineFlag[i] = false;
            P0timeLine[i]->setEnabled(false);
        }
        if(k < pulseCount+1) {
            pulseCount = k-1;
            if(pulseCount == -1) { lastTime = 0; }
            else { lastTime = P0timeLine[pulseCount]->text().toInt(); }
        }
    }
    else { // When activated
        float setValue = SRsetLine[k]->text().toFloat(&isNum);
        if (1) { // condition for acceptable string - needs to be added
            if (k == pulseCount + 1) { // Activate the signal only when all the previous signals are activated
                P0timeLine[k]->setEnabled(true);
                SRsetLineText[k] = SRsetLine[k]->text();
                SRsetLineFlag[k] = true;
                if( pulseCount != -1 ) {
                    if( P0timeLine[k]->text().toInt() <= P0timeLine[k-1]->text().toInt() ) {
                        P0timeLine[k]->setText(QString::number(P0timeLine[k-1]->text().toInt()+ref));
                    }
                    lastTime = P0timeLine[k]->text().toInt();
                }
                pulseCount++;
            }
            else if (k < pulseCount + 1) {
                SRsetLineText[k] = SRsetLine[k]->text();
                if(pulseCount == -1) { lastTime = 0; }
                else { lastTime = P0timeLine[pulseCount]->text().toInt(); }
            }
            else {
                SRsetLine[k]->setText("Off");
                SRsetLineFlag[k] = false;
            }
       }
       else {
          SRsetLine[k]->setText(SRsetLineText[k]);
       }
    }
}

void SerialChannelClass::SRtimeLineChanged(int k)
{
  bool isNum = true;
  int ref = getClockSpeed();
  int timeValue = P0timeLine[k]->text().toInt(&isNum);
  int i;

  if((isNum == true) && ((timeValue <= timeLimit) && (timeValue >= 0))) {
    if(k==0) {
      if ((SRsetLineFlag[k+1]==false) || (timeValue < P0timeLine[k+1]->text().toInt())) {
        if((timeValue%ref)!=0) { P0updateTimeLine(k, ref, timeValue); }
        else { P0timeLineText[k] = P0timeLine[k]->text(); }
      }
      else { P0timeLine[k]->setText("0"); }
    }
    else if(k == pulseCount) {
      if (timeValue > P0timeLine[k-1]->text().toInt()) {
        if((timeValue%ref)!=0) {
          P0updateTimeLine(k, ref, timeValue);
          // lastTime = P0timeLine[k]->text().toInt();
        }
        else { P0timeLineText[k] = P0timeLine[k]->text(); }
      }
      else { P0timeLine[k]->setText(QString::number(P0timeLine[k-1]->text().toInt()+ref)); }
      lastTime = P0timeLine[pulseCount]->text().toInt();
    }
    else {
      if ((timeValue > P0timeLine[k-1]->text().toInt()) && ( (SRsetLineFlag[k+1]==false) || (timeValue < P0timeLine[k+1]->text().toInt()))) {
        if((timeValue%ref)!=0) { P0updateTimeLine(k, ref, timeValue); }
        else { P0timeLineText[k] = P0timeLine[k]->text(); }
      }
      else { P0timeLine[k]->setText(QString::number(P0timeLine[k-1]->text().toInt()+ref)); }
    }
    // lastTime = P0timeLine[pulseCount]->text().toInt();
  }
  else {
    P0timeLine[k]->setText(P0timeLineText[k]);
  }
}

void SerialChannelClass::genSerialCommand(QString serialStr)
{
  int i, j, k;

  QByteArray array = serialStr.toLocal8Bit();
  char* command = array.data();

  SRcommandLen = strlen(command);
  int commandBit[10*SRcommandLen];
  SRdataSingle = new uInt32[10*10*SRcommandLen];
  P0dataSingle ^= (-1 ^ P0dataSingle) & (0x0001 << P0lineNum);
  for(i=0;i<(10*10*SRcommandLen);i++){ // update the current states to SRdataSingle
    SRdataSingle[i] = P0dataSingle;
  }
  i=0;

  for(j=0;j<SRcommandLen;j++){
    commandBit[10*j] = 0;
    for(k=0; k<8; k++){
      commandBit[10*j+k+1]=(command[j]>>k)&(0x0001);
    }
    commandBit[10*j+9] = 1;
  }
  for(j=0;j<(10*SRcommandLen);j++){
    if(j%3) {
      // for(k=0; k<4; k++){
      for(k=0; k<9; k++){
        SRdataSingle[i] ^= (-commandBit[j] ^ SRdataSingle[i]) & (0x0001 << (P0lineNum));
        i++;
      }
    }
    else {
      // for(k=0; k<5; k++){
      for(k=0; k<8; k++){
        SRdataSingle[i] ^= (-commandBit[j] ^ SRdataSingle[i]) & (0x0001 << (P0lineNum));
        i++;
      }
    }
  }
}

QString SerialChannelClass::detuneToSerial(QString ddsStr)
{
    int frequencyVal = (int)(17.179875*(ddsStr.toFloat()));
    QString serialStr = "&4_"; // Ch2: &4, Ch3: &5
    serialStr.append(QString("%1").arg(frequencyVal, 10, 10, QLatin1Char('0')));
    serialStr.append(".00_0255_060.0_0_*");
    return serialStr;
}
