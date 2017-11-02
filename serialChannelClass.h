#ifndef SERIALCHANNELCLASS_H
#define SERIALCHANNELCLASS_H

#include "pulseChannelClass.h"

class SerialChannelClass : public PulseChannelClass
{
private:
  QString SRidleLineText;
  // QString SRtimeLineText[serialNum];  // P0timeLineText is used instead
  QString SRsetLineText[serialNum];
  bool SRsetLineFlag[serialNum];
  uInt32 *SRdataSingle;
//  static int serialChannelCount;

public slots:
  void serialCheckBoxClicked();
  void SRidleLineChanged();
  void SRsetLineChanged(int k);
  void SRtimeLineChanged(int k);

public:
  SerialChannelClass();
  // void createSRtaskHandle();
  // void clearSRtaskHandle();
  // void SRupdateTimeLine(int k, int ref, int t);
  QCheckBox *ddsCheckBox;
  QLineEdit *SRsetLine[serialNum];
  // QLineEdit *SRtimeLine[serialNum];
  // void serialTimerStop();
  QLabel *serialTerminalLabel;
  QLineEdit *SRidleLine;
  void genSerialCommand(QString);
  QString detuneToSerial(QString);
  TaskHandle AItaskHandleSR;
  int SRcommandLen;
  // int lastTime;
  // int serialCount;

  // TaskHandle  SRtaskHandleSingle;
  // uInt64 samplesPerChan;
  // float64 timeout;
  // float64 SRdataSingle;
  // float64 *SRdata;
};

#endif
