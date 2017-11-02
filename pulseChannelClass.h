#ifndef PULSECHANNELCLASS_H
#define PULSECHANNELCLASS_H


#include "channelClass.h"

class PulseChannelClass : public ChannelClass
{
private:
//  static int pulseChannelCount;
//  QString P0timeLineText[pulseNum];
//  static TaskHandle  P0taskHandleSingle;
//  static uInt64 samplesPerChan;
//  static float64 timeout;

protected:
  static uInt32 P0dataSingle;
  QString P0timeLineText[pulseNum];


public slots:
  void pulseCheckBoxClicked();
  void P0idleComboBoxChanged();
  void P0stateComboBoxChanged(int);
  void P0timeLineChanged(int);
  void pulseOnTaskStart();
  void pulseOffTaskStart();
  void pulseOnTimerStart();
  void pulseOffTimerStart();
  virtual void serialCheckBoxClicked() {}
  virtual void SRidleLineChanged() {}
  virtual void SRsetLineChanged(int) {}
  virtual void SRtimeLineChanged(int) {}

public:
  PulseChannelClass();
  void P0updateTimeLine(int k, int ref, int t);
  QComboBox *P0idleComboBox;
  QComboBox *P0stateComboBox[pulseNum];
  QLineEdit *P0timeLine[pulseNum];
  int lastTime;
  int pulseCount;
  void writeP0dataSingle(uInt32);
  uInt32 readP0dataSingle();
  // void pulseTimerStop();

  TaskHandle P0taskHandleSingle;
  uInt64 samplesPerChan;
  float64 timeout;
  
//  time_t startTime;
//  float64 freq;
//  float64 duty;
//  float64 delay;
//  bool32 done;

/*
  PulseChannelClass();
  QLabel *pulseTerminalLabel;
  QLineEdit *pulseNameLine;
  QComboBox *pulseIdleComboBox;
  QLineEdit *pulseDelayLine;
  QLineEdit *pulseOnLine;
  QLineEdit *pulseOffLine;
*/
};

#endif
