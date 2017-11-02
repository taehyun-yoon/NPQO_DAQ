#ifndef ANALOGOUTCHANNELCLASS_H
#define ANALOGOUTCHANNELCLASS_H

#include "channelClass.h"

class AnalogOutChannelClass : public ChannelClass
{
private:
  QString AOidleLineText;
  QString AOtimeLineText[analogOutNum];
  QString AOsetLineText[analogOutNum];
  bool AOsetLineFlag[analogOutNum];
//  static int analogOutChannelCount;

public slots:
  void analogOutCheckBoxClicked();
  void AOidleLineChanged();
  void AOsetLineChanged(int k);
  void AOtimeLineChanged(int k);
  // void analogOutOnTaskStart();
  // void analogOutOffTaskStart();
  // void analogOutOnTimerStart();
  // void analogOutOffTimerStart();

public:
  AnalogOutChannelClass();
  void createAOtaskHandle();
  void clearAOtaskHandle();
  void AOupdateTimeLine(int k, int ref, int t);
  QLineEdit *AOsetLine[analogOutNum];
  QLineEdit *AOtimeLine[analogOutNum];
  void analogOutTimerStop();
  QLabel *analogOutTerminalLabel;
  QLineEdit *AOidleLine;
  int lastTime;
  int analogOutCount;

  TaskHandle  AOtaskHandleSingle;
  uInt64 samplesPerChan;
  float64 timeout;
  float64 AOdataSingle;
  // float64 *AOdata;
};

#endif
