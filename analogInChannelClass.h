#ifndef ANALOGINCHANNELCLASS_H
#define ANALOGINCHANNELCLASS_H

#include "channelClass.h"

class AnalogInChannelClass : public ChannelClass
{
private:
//  static int analogOutChannelCount;

public slots:
  void analogInCheckBoxClicked();
//  void AIrateTextChanged(QString);
//  void AIWriteCheckBoxClicked();
//  void analogInReadData();
  void AIrateSpinBoxChanged();

public:
  AnalogInChannelClass();
  void createAItaskHandle();
  void clearAItaskHandle();
  void updateAIrateSpinBox(int);
  QSpinBox *AIrateSpinBox;
  // QComboBox *AIconvertComboBox;
  FILE *writeFile;
  // bool AIWriteFlag;
  // double convertedValue;

  TaskHandle  AItaskHandleSingle;
  uInt64 samplesPerChan;
  float64 timeout;
  int AIreadRate;
  // float64 *AIdata;
  // double convertFunc1();
  // double convertFunc2();
  // bool analogInReadFlag;
  // double currentTime;

};

#endif

