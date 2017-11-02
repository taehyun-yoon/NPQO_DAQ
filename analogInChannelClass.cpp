#include "analogInChannelClass.h"

AnalogInChannelClass::AnalogInChannelClass() : ChannelClass("AI")
{
  AIrateSpinBox = new QSpinBox;
  AIrateSpinBox->setEnabled(false);
  AIrateSpinBox->setSuffix(" us");
  AIrateSpinBox->setRange(1,60000000); // The maximum period 60s
  AIrateSpinBox->setSingleStep(clockSingleStep);
  AIrateSpinBox->setValue(50*clockSingleStep);   // Set analog in rate
  AIrateSpinBox->setKeyboardTracking(false);

  connect(AIrateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(AIrateSpinBoxChanged()));

  connect(channelCheckBox,SIGNAL(toggled(bool)),this,SLOT(analogInCheckBoxClicked()));
}

void AnalogInChannelClass::analogInCheckBoxClicked()
{
  if (channelCheckBox->isChecked()) {
    channelNameLine->setEnabled(true);
    AIrateSpinBox->setEnabled(true);

    createAItaskHandle();

//    AIconvertComboBox->setEnabled(true);

//    writeFile = fopen("dummy.txt", "w");
//    fclose(writeFile);
//    remove("dummy.txt");

  }
  else {
    channelNameLine->setEnabled(false);
    AIrateSpinBox->setEnabled(false);
    //  AIconvertComboBox->setEnabled(false);
    clearAItaskHandle();

  }
}

void AnalogInChannelClass::AIrateSpinBoxChanged()
{
  int t = AIrateSpinBox->value();
  int ref = getClockSpeed();
  if((t%ref)!=0) { AIrateSpinBox->setValue(getRound(t,ref)); }
  // AIrateSpinBox->setValue(getRound(t,ref));
}

void AnalogInChannelClass::createAItaskHandle()
{
    /************************ Create TaskHandle **************************/
    AItaskHandleSingle = 0;
    samplesPerChan = 1;
    timeout = 10.0;

    DAQmxCreateTask("",&AItaskHandleSingle);
    DAQmxCreateAIVoltageChan(AItaskHandleSingle,chan,"",DAQmx_Val_Cfg_Default,minVoltage,maxVoltage,DAQmx_Val_Volts,"");
    /*********************************************************************/
}

void AnalogInChannelClass::clearAItaskHandle()
{
    /************************* Clear TaskHandle **************************/
    DAQmxStopTask(AItaskHandleSingle);
    DAQmxClearTask(AItaskHandleSingle);
    /*********************************************************************/
}

// void AnalogInChannelClass::updateAIrateSpinBox(int t)
// {
//   AIrateSpinBox->setRange(t,500000);
//   AIrateSpinBox->setSingleStep(t);
// }

/*
double AnalogInChannelClass::convertFunc1()
{
  return 1.5*data;
}

double AnalogInChannelClass::convertFunc2()
{
  return 0.75*data;
}
*/
