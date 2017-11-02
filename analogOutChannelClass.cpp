#include "analogOutChannelClass.h"

AnalogOutChannelClass::AnalogOutChannelClass() : ChannelClass("AO")
{
  int k;

  AOidleLine = new QLineEdit;
  AOidleLineText = "0";
  AOidleLine->setText(AOidleLineText);
  AOidleLine->setAlignment(Qt::AlignCenter);
  AOidleLine->setEnabled(false);

  for(k=0; k<analogOutNum; k++){
    AOsetLineText[k] = "Off";
    AOsetLine[k] = new QLineEdit;
    AOsetLine[k]->setText(AOsetLineText[k]);
    AOsetLine[k]->setAlignment(Qt::AlignCenter);
    AOsetLine[k]->setEnabled(false);
    AOsetLineFlag[k] = false;

    AOtimeLine[k] = new QLineEdit;
    AOtimeLine[k]->setText("0");
    AOtimeLine[k]->setAlignment(Qt::AlignCenter);
    AOtimeLine[k]->setEnabled(false);
  }

  analogOutCount = -1;

  /******************************** Signals to Slots ***************************************/
  connect(channelCheckBox,SIGNAL(toggled(bool)),this,SLOT(analogOutCheckBoxClicked()));
  connect(AOidleLine,SIGNAL(editingFinished()),this,SLOT(AOidleLineChanged()));

  QSignalMapper* signalMapper1 = new QSignalMapper (this) ;
  for(k=0; k<analogOutNum; k++) {
    connect(AOsetLine[k],SIGNAL(editingFinished()),signalMapper1,SLOT(map()));
    signalMapper1 -> setMapping (AOsetLine[k], k) ;
  }
  connect (signalMapper1, SIGNAL(mapped(int)), this, SLOT(AOsetLineChanged(int))) ;

  QSignalMapper* signalMapper2 = new QSignalMapper (this) ;
  for(k=0; k<analogOutNum; k++) {
    connect(AOtimeLine[k],SIGNAL(editingFinished()),signalMapper2,SLOT(map()));
    signalMapper2 -> setMapping (AOtimeLine[k], k) ;
  }
  connect (signalMapper2, SIGNAL(mapped(int)), this, SLOT(AOtimeLineChanged(int))) ;
}

void AnalogOutChannelClass::analogOutCheckBoxClicked()
{
  int k;
  if (channelCheckBox->isChecked()) {
    channelNameLine->setEnabled(true);
    AOidleLine->setEnabled(true);
    for(k=0; k<analogOutNum; k++){
      AOsetLine[k]->setEnabled(true);
      if(AOsetLineFlag[k] == true) {AOtimeLine[k]->setEnabled(true);}
    }

    // AOidleLine->selectAll();

    createAOtaskHandle();

  }
  else {
    channelNameLine->setEnabled(false);
    AOidleLine->setEnabled(false);
    for(k=0; k<analogOutNum; k++){
      AOsetLine[k]->setEnabled(false);
      AOtimeLine[k]->setEnabled(false);
    }
    clearAOtaskHandle();

  }
}

void AnalogOutChannelClass::AOidleLineChanged()
{
  bool isNum = true;
  float setValue = AOidleLine->text().toFloat(&isNum);
  if((isNum == true) && ((setValue <= maxVoltage) && (setValue >= minVoltage))) {
    AOidleLineText = AOidleLine->text();

    /************************* Start TaskHandle **************************/
    AOdataSingle = setValue;
    DAQmxStartTask(AOtaskHandleSingle);
    DAQmxWriteAnalogF64(AOtaskHandleSingle,samplesPerChan,0,timeout,DAQmx_Val_GroupByChannel,&AOdataSingle,NULL,NULL);
    DAQmxStopTask(AOtaskHandleSingle);
    /*********************************************************************/
  }
  else {
    AOidleLine->setText(AOidleLineText);
  }
}

void AnalogOutChannelClass::AOsetLineChanged(int k)
{
    int i;
    bool isNum = true;
    int ref = getClockSpeed();
    if ((AOsetLine[k]->text() == NULL) || ((AOsetLine[k]->text() == "Off") || (AOsetLine[k]->text() == "off")) ) {  // When activated
        // Turn off all signals [k] and after [k]
        AOtimeLine[k]->setEnabled(false);
        AOsetLine[k]->setText("Off");
        AOsetLineFlag[k] = false;
        for(i=k+1; i<analogOutNum; i++) {
            AOsetLine[i]->setText("Off");
            AOsetLineFlag[i] = false;
            AOtimeLine[i]->setEnabled(false);
        }
        if(k < analogOutCount+1) {
            analogOutCount = k-1;
            if(analogOutCount == -1) { lastTime = 0; }
            else { lastTime = AOtimeLine[analogOutCount]->text().toInt(); }
        }
    }
    else { // When deactivated
        float setValue = AOsetLine[k]->text().toFloat(&isNum);
        if ((isNum == true) && ((setValue <= maxVoltage) && (setValue >= minVoltage))) {
            // for(i=0; i<k; i++) {
              // pulseCount += (AOsetLineFlag[i]==true);
            //}
            if (k == analogOutCount + 1) { // Activate the signal only when all the former signals are activated
                AOtimeLine[k]->setEnabled(true);
                AOsetLineText[k] = AOsetLine[k]->text();
                AOsetLineFlag[k] = true;
                if( analogOutCount != -1 ) {
                    if( AOtimeLine[k]->text().toInt() <= AOtimeLine[k-1]->text().toInt() ) {
                        AOtimeLine[k]->setText(QString::number(AOtimeLine[k-1]->text().toInt()+ref));
                    }
                    lastTime = AOtimeLine[k]->text().toInt();
                }
                analogOutCount++;
            }
            else if (k < analogOutCount + 1) {
                AOsetLineText[k] = AOsetLine[k]->text();
                if(analogOutCount == -1) { lastTime = 0; }
                else { lastTime = AOtimeLine[analogOutCount]->text().toInt(); }
            }
            else {
                AOsetLine[k]->setText("Off");
                AOsetLineFlag[k] = false;
            }
       }
       else {
          AOsetLine[k]->setText(AOsetLineText[k]);
       }
    }
}

void AnalogOutChannelClass::AOtimeLineChanged(int k)
{
  bool isNum = true;
  int ref = getClockSpeed();
  int timeValue = AOtimeLine[k]->text().toInt(&isNum);
  int i;

  if((isNum == true) && ((timeValue <= timeLimit) && (timeValue >= 0))) {
    if(k==0) {
      if ((AOsetLineFlag[k+1]==false) || (timeValue < AOtimeLine[k+1]->text().toInt())) {
        if((timeValue%ref)!=0) { AOupdateTimeLine(k, ref, timeValue); }
        else { AOtimeLineText[k] = AOtimeLine[k]->text(); }
      }
      else { AOtimeLine[k]->setText("0"); }
    }
    else if(k == analogOutCount) {
      if (timeValue > AOtimeLine[k-1]->text().toInt()) {
        if((timeValue%ref)!=0) {
          AOupdateTimeLine(k, ref, timeValue);
          // lastTime = AOtimeLine[k]->text().toInt();
        }
        else { AOtimeLineText[k] = AOtimeLine[k]->text(); }
      }
      else { AOtimeLine[k]->setText(QString::number(AOtimeLine[k-1]->text().toInt()+ref)); }
      lastTime = AOtimeLine[analogOutCount]->text().toInt();
    }
    else {
      if ((timeValue > AOtimeLine[k-1]->text().toInt()) && ( (AOsetLineFlag[k+1]==false) || (timeValue < AOtimeLine[k+1]->text().toInt()))) {
        if((timeValue%ref)!=0) { AOupdateTimeLine(k, ref, timeValue); }
        else { AOtimeLineText[k] = AOtimeLine[k]->text(); }
      }
      else { AOtimeLine[k]->setText(QString::number(AOtimeLine[k-1]->text().toInt()+ref)); }
    }
    // lastTime = AOtimeLine[analogOutCount]->text().toInt();
  }
  else {
    AOtimeLine[k]->setText(AOtimeLineText[k]);
  }
}

void AnalogOutChannelClass::AOupdateTimeLine(int k, int ref, int t)
{
  AOtimeLineText[k] = QString::number(getRound(t, ref));
  AOtimeLine[k]->setText(AOtimeLineText[k]);
}

void AnalogOutChannelClass::createAOtaskHandle()
{
    /************************ Create TaskHandle **************************/
    AOtaskHandleSingle = 0;
    samplesPerChan = 1;
    timeout = 10.0;

    DAQmxCreateTask("",&AOtaskHandleSingle);
    DAQmxCreateAOVoltageChan(AOtaskHandleSingle,chan,"",minVoltage,maxVoltage,DAQmx_Val_Volts,NULL);
    /*********************************************************************/
}

void AnalogOutChannelClass::clearAOtaskHandle()
{
    /************************* Clear TaskHandle **************************/
    DAQmxStopTask(AOtaskHandleSingle);
    DAQmxClearTask(AOtaskHandleSingle);
    /*********************************************************************/
}
