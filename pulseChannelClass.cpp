#include "pulseChannelClass.h"  
 
PulseChannelClass::PulseChannelClass() : ChannelClass("P0")
{
  int k;

  P0idleComboBox = new QComboBox;
  P0idleComboBox->addItem("Low");
  P0idleComboBox->addItem("High");
  P0idleComboBox->setEnabled(false);

  for(k=0; k<pulseNum; k++){
  P0stateComboBox[k] = new QComboBox;
      P0stateComboBox[k]->addItem("Off");
    if((k%2)==0){
      P0stateComboBox[k]->addItem("High");
    }
    else {
      P0stateComboBox[k]->addItem("Low");
    }
    P0stateComboBox[k]->setEnabled(false);

    P0timeLine[k] = new QLineEdit;
    P0timeLine[k]->setText("0");
    P0timeLine[k]->setAlignment(Qt::AlignCenter);
    P0timeLine[k]->setEnabled(false);
  }

  pulseCount = -1;

  /******************************** Signals to Slots ***************************************/
  if (serialChannelCount == serialChannelNum) { // Not apply for serialChannelObj
    connect(channelCheckBox,SIGNAL(toggled(bool)),this,SLOT(pulseCheckBoxClicked()));
  }
  connect(P0idleComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(P0idleComboBoxChanged()));

  QSignalMapper* signalMapper1 = new QSignalMapper (this) ;
  for(k=0; k<pulseNum; k++) {
    connect(P0stateComboBox[k],SIGNAL(currentIndexChanged(int)),signalMapper1,SLOT(map()));
    signalMapper1 -> setMapping (P0stateComboBox[k], k) ;
  }
  connect (signalMapper1, SIGNAL(mapped(int)), this, SLOT(P0stateComboBoxChanged(int))) ;

  if (serialChannelCount == serialChannelNum) { // Not apply for serialChannelObj
    QSignalMapper* signalMapper2 = new QSignalMapper (this) ;
    for(k=0; k<pulseNum; k++) {
      connect(P0timeLine[k],SIGNAL(editingFinished()),signalMapper2,SLOT(map()));
      signalMapper2 -> setMapping (P0timeLine[k], k) ;
    }
    connect (signalMapper2, SIGNAL(mapped(int)), this, SLOT(P0timeLineChanged(int))) ;
  }
}


void PulseChannelClass::pulseCheckBoxClicked()
{
  int k;
  if (channelCheckBox->isChecked()) {
    channelNameLine->setEnabled(true);
    P0idleComboBox->setEnabled(true);
    for(k=0; k<pulseNum; k++){
      P0stateComboBox[k]->setEnabled(true);
      if(k <= pulseCount) { P0timeLine[k]->setEnabled(true); }
    }
    /************************ Create TaskHandle **************************/
    P0taskHandleSingle = 0;
    samplesPerChan = 1;
    timeout = 10.0;

    DAQmxCreateTask("",&P0taskHandleSingle);
    DAQmxCreateDOChan(P0taskHandleSingle,chan,"",DAQmx_Val_ChanForAllLines);
    /*********************************************************************/
  }
  else {
    channelNameLine->setEnabled(false);
    P0idleComboBox->setEnabled(false);
    for(k=0; k<pulseNum; k++){
      P0stateComboBox[k]->setEnabled(false);
      P0timeLine[k]->setEnabled(false);
    }
    /************************* Clear TaskHandle **************************/
    DAQmxStopTask(P0taskHandleSingle);
    DAQmxClearTask(P0taskHandleSingle);
    /*********************************************************************/
  }
}

void PulseChannelClass::P0idleComboBoxChanged()
{
  int i;
  if (P0idleComboBox->currentIndex() == 0) {
    for(i=0; i<pulseNum; i++) {
      if((i%2)==0){
        P0stateComboBox[i]->setItemText(1,"High");
      }
      else {
        P0stateComboBox[i]->setItemText(1,"Low");
      }
    }
  }
  else {
    for(i=0; i<pulseNum; i++) {
      if((i%2)==0){
        P0stateComboBox[i]->setItemText(1,"Low");
      }
      else {
        P0stateComboBox[i]->setItemText(1,"High");
      }
    }
  }
  P0dataSingle ^= (-0 ^ P0dataSingle) & (0x0001 << cntTrigPort1); // Set the counter trigger to 0.19
  P0dataSingle ^= (-0 ^ P0dataSingle) & (0x0001 << cntTrigPort2); // Set the counter trigger to 0.20
  P0dataSingle ^= (-P0idleComboBox->currentIndex() ^ P0dataSingle) & (0x0001 << P0lineNum);

  /************************ Run single value update ***********************/
  DAQmxStartTask(P0taskHandleSingle);
  DAQmxWriteDigitalU32(P0taskHandleSingle,samplesPerChan,1,timeout,DAQmx_Val_GroupByChannel,&P0dataSingle,NULL,NULL);
  DAQmxStopTask(P0taskHandleSingle);
  /************************************************************************/
}

void PulseChannelClass::P0stateComboBoxChanged(int k)
{
    int i;
    int ref = getClockSpeed();

    if (P0stateComboBox[k]->currentIndex() == 0) { // When deactivated,
        P0timeLine[k]->setEnabled(false);
        for(i=k+1; i<pulseNum; i++) {
            P0timeLine[i]->setEnabled(false);
            P0stateComboBox[i]->setCurrentIndex(0);
        }
        if(k < (pulseCount + 1)) {
            pulseCount = k-1;
            if(pulseCount == -1) { lastTime = 0; }
            else { lastTime = P0timeLine[pulseCount]->text().toInt(); }
        }
    }
    else {  // When activated
        if(k == pulseCount+1) {  // Activate the signal only when all the previous signals are activated
            P0timeLine[k]->setEnabled(true);
            if( pulseCount != -1 ) {
                if( P0timeLine[k]->text().toInt() <= P0timeLine[k-1]->text().toInt() ) {
                    P0timeLine[k]->setText(QString::number(P0timeLine[k-1]->text().toInt()+ref));
                }
                lastTime = P0timeLine[k]->text().toInt();
            }
            pulseCount++;
        }
        else if (k < pulseCount + 1) { }
        else {
            P0stateComboBox[k]->setCurrentIndex(0);
        }
    }
}

void PulseChannelClass::P0timeLineChanged(int k)
{
    bool isNum = true;
    int ref = getClockSpeed();
    int timeValue = P0timeLine[k]->text().toInt(&isNum);

    if((isNum == true) && ((timeValue <= timeLimit) && (timeValue >= 0))) {
        if(k==0) {  // In the case of the first time line
            if (( P0stateComboBox[k+1]->currentIndex() == 0) || (timeValue < P0timeLine[k+1]->text().toInt())) {
                if((timeValue%ref)!=0) { P0updateTimeLine(k, ref, timeValue); }
                else { P0timeLineText[k] = P0timeLine[k]->text(); }
            }
            else { P0timeLine[k]->setText("0"); }
        }
        else if(k == pulseCount) { // In the case of the last time line
            if (timeValue > P0timeLine[k-1]->text().toInt()) {
                if((timeValue%ref)!=0) {
                    P0updateTimeLine(k, ref, timeValue);
                    // lastTime = P0timeLine[k]->text().toInt();
                }
                else { P0timeLineText[k] = P0timeLine[k]->text(); }
            }
            else { P0timeLine[k]->setText(QString::number(P0timeLine[k-1]->text().toInt()+ref)); }
            lastTime = P0timeLine[k]->text().toInt();
        }
        else {
            if ((timeValue > P0timeLine[k-1]->text().toInt()) && ( (P0stateComboBox[k+1]->currentIndex()==0) || (timeValue < P0timeLine[k+1]->text().toInt()))) {
                if((timeValue%ref)!=0) { P0updateTimeLine(k, ref, timeValue); }
                else { P0timeLineText[k] = P0timeLine[k]->text(); }
            }
            else { P0timeLine[k]->setText(QString::number(P0timeLine[k-1]->text().toInt()+ref)); }
        }
        // lastTime = P0timeLine[k]->text().toInt();
    }
    else {
        P0timeLine[k]->setText(P0timeLineText[k]);
    }
}

void PulseChannelClass::P0updateTimeLine(int k, int ref, int t)
{
  P0timeLineText[k] = QString::number(getRound(t, ref));
  P0timeLine[k]->setText(P0timeLineText[k]);
}

void PulseChannelClass::writeP0dataSingle(uInt32 d)
{
   P0dataSingle = d;
}

uInt32 PulseChannelClass::readP0dataSingle()
{
   return P0dataSingle;
}
