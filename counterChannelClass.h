#ifndef COUNTERCHANNELCLASS_H
#define COUNTERCHANNELCLASS_H

#include "channelClass.h"

class CounterChannelClass : public ChannelClass
{
private:
//  static int analogOutChannelCount;

public slots:
  void counterCheckBoxClicked();
//  void CNTrateTextChanged(QString);
//  void AIWriteCheckBoxClicked();
//  void CNTreadData();
  void CNTrateSpinBoxChanged();

public:
  CounterChannelClass();
  void createCNTtaskHandle();
  void clearCNTtaskHandle();
  void updateCNTrateSpinBox(int);
  void CNTrefTimeEnable();
  QSpinBox *CNTrateSpinBox;
  QComboBox *CNTrefTimeComboBox;
  QComboBox *CNTstartComboBox;
  QComboBox *CNTendComboBox;
  FILE *writeFile;
  static bool freqScanFlag;
  bool CNTwriteFlag;
  // double convertedValue;
  int CNTreadRate;

  TaskHandle CNTtaskHandle;
  // uInt32 *CNTdata;
  uInt64 CNTsampsPerChanToAcquire;
  int32	CNTnumSampsPerChan;
  // int CNTdataSize;

  // double convertFunc1();
  // double convertFunc2();
  // bool CNTreadFlag;
  // double currentTime;

};

#endif

