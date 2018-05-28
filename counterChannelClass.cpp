#include "counterChannelClass.h"

CounterChannelClass::CounterChannelClass() : ChannelClass("P2")
{
  CNTrateSpinBox = new QSpinBox;
  CNTrateSpinBox->setEnabled(false);
  CNTrateSpinBox->setSuffix(" us");
  CNTrateSpinBox->setRange(1,1000000);
  CNTrateSpinBox->setSingleStep(clockSingleStep);
  CNTrateSpinBox->setValue(5*clockSingleStep);
  CNTrateSpinBox->setKeyboardTracking(false);

  CNTrefTimeComboBox = new QComboBox;
  CNTrefTimeComboBox->setEnabled(false);

  CNTstartComboBox = new QComboBox;
  CNTstartComboBox->setEnabled(false);

  CNTendComboBox = new QComboBox;
  CNTendComboBox->setEnabled(false);

  connect(CNTrateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(CNTrateSpinBoxChanged()));

  connect(channelCheckBox,SIGNAL(toggled(bool)),this,SLOT(counterCheckBoxClicked()));
}

void CounterChannelClass::counterCheckBoxClicked()
{
  if (channelCheckBox->isChecked()) {
    channelNameLine->setEnabled(true);
    CNTrateSpinBox->setEnabled(true);

//    createCNTtaskHandle();

//    CNTconvertComboBox->setEnabled(true);

//    writeFile = fopen("dummy.txt", "w");
//    fclose(writeFile);
//    remove("dummy.txt");

  }
  else {
    channelNameLine->setEnabled(false);
    CNTrateSpinBox->setEnabled(false);
//    CNTconvertComboBox->setEnabled(false);
  }
}

void CounterChannelClass::CNTrateSpinBoxChanged()
{
  int t = CNTrateSpinBox->value();
  int ref = getClockSpeed();
  if((t%ref)!=0) { CNTrateSpinBox->setValue(getRound(t, ref)); }
}

void CounterChannelClass::createCNTtaskHandle()
{
    /************************ Create TaskHandle **************************/
    CNTtaskHandle = 0;
    // samplesPerChan = 1;
    // timeout = 10.0;

    DAQmxCreateTask("",&CNTtaskHandle);
    DAQmxCreateCICountEdgesChan(CNTtaskHandle,chan,"",DAQmx_Val_Rising,0,DAQmx_Val_CountUp);
    /*********************************************************************/
}

void CounterChannelClass::clearCNTtaskHandle()
{
    /************************* Clear TaskHandle **************************/
    DAQmxStopTask(CNTtaskHandle);
    DAQmxClearTask(CNTtaskHandle);
    /*********************************************************************/
}

/*
double CounterChannelClass::convertFunc1()
{
  return 1.5*data;
}

double CounterChannelClass::convertFunc2()
{
  return 0.75*data;
}
*/
