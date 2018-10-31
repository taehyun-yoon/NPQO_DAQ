#ifndef CHANNELCLASS_H
#define CHANNELCLASS_H

#include <QtWidgets>
#include <QObject>
#include <QMainWindow>
#include <stdio.h>
#include <time.h>
#include <QShortcut>
/******************************** No DAQ CARD **********************************/
#include "NIDAQmx.h"
/******************************** No DAQ CARD **********************************/


#define analogInChannelNum 3 // number of AI channels
#define counterChannelNum 2 // Number of CNT channels
#define serialChannelNum 5 // Number of SR channels
#define pulseChannelNum 12 // number of P0 channels
#define analogOutChannelNum 4 // Number of AO channels
#define DOBoxPortNum 6 // Starting port number P0.X (P0.0 - P0.5), P0.6: 3.3V DO2, P0.7: 3.3V DO3
#define pulseNum 6 // Number of pulses per each P0 channel
#define serialNum pulseNum // Number of serials per each P0 channel
#define analogOutNum pulseNum // Number of analog value changes per each AO channel
#define clockSingleStep 1 // Size of single step of the clock [us]
#define maxVoltage 10.0 // Maximum voltage of analog output [V]
#define minVoltage -10.0 // Minimum voltage of analog output [V]
#define cntTrigPort1 19 // P0.19 used for triggering Counter
#define cntTrigPort2 18 // P0.18 used for triggering Counter
#define timeLimit 5000000 // Time Limit of one cycle [us]
//#define timeLimit 5000000000 // Time Limit of one cycle [us] : 5000s // modification for Jeremy's measurement
#define zero 0
#define one 1

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGridLayout;
class QGroupBox;
class QInputDialog;
class QLabel;
class QListWidget;
class QLineEdit;
class QMenu;
class QScollArea;
class QSpinBox;
class QTextEdit;
class QTimer;
class QWidget;
QT_END_NAMESPACE

class ChannelClass : public QObject // abstract class
{
  Q_OBJECT

private:
  static int analogInChannelCount;
  static int pulseChannelCount;
  static int analogOutChannelCount;
  static int counterChannelCount;
  static int clockSpeed;

protected:
  static int serialChannelCount;


public slots:
  /*
  virtual void analogInCheckBoxClicked() = 0;
  virtual void counterCheckBoxClicked() = 0;
  virtual void pulseCheckBoxClicked() = 0;
  virtual void serialCheckBoxClicked() = 0;
  virtual void analogOutCheckBoxClicked() = 0;
  virtual void P0idleComboBoxChanged() = 0;
  virtual void P0stateComboBoxChanged(int) = 0;
  virtual void P0timeLineChanged(int) = 0;
  virtual void SRidleLineChanged() = 0;
  virtual void SRsetLineChanged(int) = 0;
  virtual void SRtimeLineChanged(int) = 0;
  virtual void AOidleLineChanged() = 0;
  virtual void AOsetLineChanged(int) = 0;
  virtual void AOtimeLineChanged(int) = 0;
  virtual void AIrateSpinBoxChanged() = 0;
  virtual void CNTrateSpinBoxChanged() = 0;
  */

  virtual void analogInCheckBoxClicked() {}
  virtual void counterCheckBoxClicked() {}
  virtual void pulseCheckBoxClicked() {}
  virtual void serialCheckBoxClicked() {}
  virtual void analogOutCheckBoxClicked() {}
  virtual void P0idleComboBoxChanged() {}
  virtual void P0stateComboBoxChanged(int) {}
  virtual void P0timeLineChanged(int) {}
  virtual void SRidleLineChanged() {}
  virtual void SRsetLineChanged(int) {}
  virtual void SRtimeLineChanged(int) {}
  virtual void AOidleLineChanged() {}
  virtual void AOsetLineChanged(int) {}
  virtual void AOtimeLineChanged(int) {}
  virtual void AIrateSpinBoxChanged() {}
  virtual void CNTrateSpinBoxChanged() {}
  void channelNameLineChanged();

public:
  ChannelClass(QString);
  QLabel *channelTerminalLabel;
  QCheckBox *channelCheckBox;
  QLineEdit *channelNameLine;
  QString str2;
  int getClockSpeed();
  void setClockSpeed(int);
  int getRound(int, int);
  int P0lineNum;

  double data;
//  int32 error;
//  TaskHandle taskHandleOff;
//  TaskHandle taskHandleOn;
//  char errBuff[2048];

  // Channel parameter;
  char chan[10];
  char *chanTitle;
  QByteArray array;
  char extClockChan[12];

};

#endif
