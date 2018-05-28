#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QInputDialog>
#include <QPushButton>
#include <QKeyEvent>
#include <QMap>
#include "analogInChannelClass.h"
#include "pulseChannelClass.h"
#include "analogOutChannelClass.h"
#include "counterChannelClass.h"
#include "serialChannelClass.h"
#include "plotDockClass.h"
#include "qcustomplot.h"

//#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
//  void newLetter();
  void save();
//  void print();
  void undo();
  void about();
  void insertCustomer(const QString &customer);
  void addParagraph(const QString &paragraph);
  void updateReadRate();
//  void plotUpdate(int);
//  void writeControlCheckBoxClicked();
  void measureControlComboBoxChanged();
  void freqScanTypeComboBoxChanged();
  void startButtonPressed();
  void getFreqScanFinal();
//  void setFreqText(int detune);
  void stopButtonPressed();
//  void runCycles();

// signals:
//   void freqUpdated(int detuning);

private:
  void createControlGroupBox();
  void createTabWidget();
  void createFreqScanGroupBox();
  void createBleachGroupBox();
  void createChannelGroupBox();
  void createNoteGroupBox();
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void createDockWindows();
  void startSingleRun();
  void bleachRun(int _cleanCh, int _cleanStart, int _pumpCh1, int pumpCh2, int pumpStart);
  void freqScanTabEnable(bool b);
  void bleachTabEnable(bool b);
  void CsScanTabEnable(bool b);
  void keyPressEvent(QKeyEvent *event);

  bool stopFlag;
  char AIchan[10 * analogInChannelNum];
  char AOchan[10 * analogOutChannelNum];
  bool P0flag;
  uInt32 *P0data;
  int P0idleInt;
  int *P0stateInt;
  int *P0timeIndex;
  QString *SRsetString;
  int SRsetStrLen;
  int *SRcommandBit;
  float AOidleFloat;
  float *AOsetFloat;
  int *AOtimeIndex;
  float64 *AOdata;
  int AItimeIndex;
  float64 *AIdata;
  // double temAIdata;
  uInt32 *CNTdata;
  int32 CNTread;
  int CNTdataSize;
  int CNTclockSpeed;
  int lastTime;
  int activeAOchannelNum;
  int activeAIchannelNum;
  int dataSize;
  double xLowLim;
  double xHighLim;

  float freqScanInit;
  int freqScanNum;
  int repeatCount;
  float freqScanStep;
  float freqScanFinal;
  float freqScanPeriod;
  float freqMultiplier;
  float freqScanRef;
  // float freqScanDivider;
  float freqScanMaster;
  int freqScanAOM;
  int freqScanNDiv1;
  int freqScanNDiv2;
  int freqScanProgress;
  float freqScanDetune;
  int freqScanDetuneInt;
  int freqScanCount;
  float *detuneArray;
  int *countDataArray;
  float *AIDataArray;
  int bleachRepeatNum;
  int bleachRepeatCount;
  int bleachTimeNum;
  int bleachTimeCount;
  int bleachProbeStart;
  int bleachCleanStart;
  int bleachPumpStart;
  int bleachWaitStart;
  int bleachCleanTime;
  int bleachWaitTime;
  float bleachPeriod;
  QString freqScanMasterText;
  QString freqScanSlaveText;
  QByteArray freqScanMasterBA;
  QByteArray freqScanSlaveBA;
  QStringList pumpTimeList;

  TaskHandle  P0taskHandle;
  TaskHandle  AItaskHandle;
  TaskHandle  AOtaskHandle;
  TaskHandle  CNTtaskHandle;
  float64 timeout;

  QWidget *channelScrollWidget;
  QTabWidget *tabWidget;
  QScrollArea *channelScrollArea;
  QGroupBox *controlGroupBox;
//  QGroupBox *freqScanGroupBox;
  QWidget *freqScanWidget;
  QWidget *bleachWidget;
  QGroupBox *bleachGroupBox;
  QGroupBox *channelGroupBox;
  QGroupBox *noteGroupBox;
  QTextEdit *textEdit;
  //  QListWidget *customerList;
  //  QListWidget *paragraphsList;

  QComboBox *measureControlComboBox;
  QSpinBox *clockSpeedSpinBox;
  QSpinBox *repeatNumSpinBox;
  QCheckBox *writeDataCheckBox;
  // QCheckBox *writeControlCheckBox;
  QPushButton *startPushButton;
  QPushButton *stopPushButton;
  QTextEdit *noteText;
  // QString *freqScanTimeString;
  QCheckBox *writeNoteCheckBox;
  QComboBox *freqScanTypeComboBox;
  QComboBox *freqScanMasterComboBox;
  QComboBox *freqScanTerminalComboBox;
  QComboBox *freqScanTimeComboBox;
  QComboBox *freqScanAOMComboBox;
  QComboBox *freqScanSlaveComboBox;
  QComboBox *freqScanDirectionComboBox;
  QLineEdit *repeatProgressLine;
  QLineEdit *freqScanInitLine;
  QLineEdit *freqScanStepLine;
  QLineEdit *freqScanFinalLine;
  QLineEdit *freqScanNumLine;
  QLineEdit *freqScanPeriodLine;
  QLineEdit *freqScanProgressLine;
  QComboBox *freqScanDetectComboBox;
  QComboBox *bleachDirectionComboBox;
  QSpinBox *bleachNumSpinBox;
  QComboBox *bleachProbePOComboBox;
  QComboBox *bleachCleanPOComboBox;
  QComboBox *bleachPumpPOComboBox1;
  QComboBox *bleachPumpPOComboBox2;
  QComboBox *bleachProbeStartComboBox;
  QComboBox *bleachCleanStartComboBox;
  QComboBox *bleachPumpStartComboBox;
  QLineEdit *bleachTimeLine;
  QLineEdit *bleachPeriodLine;
  QLineEdit *bleachRepeatProgressLine;
  QLineEdit *bleachTimeProgressLine;
  // QLineEdit *progressControlLine;
  // QLineEdit *cycleControlLine;

  // widgets in plot dock
  QDockWidget *dockObj[plotDockNum];
  PlotDockClass *plotDockObj[plotDockNum];
  // QSignalMapper *signalMapper[plotDockNum];
  // QTimer *delayControlTimer;

  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *viewMenu;
  QMenu *helpMenu;
  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QAction *saveAct;
  QAction *undoAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
  QAction *quitAct;

  QWidget *window;
  //QGridLayout *windowLayout;
  QGridLayout *channelGroupLayout;
  QVBoxLayout *windowLayout;
  QGridLayout *controlLayout;
  QGridLayout *freqScanLayout;
  QGridLayout *bleachLayout;
  QGridLayout *channelLayout;
  QGridLayout *noteLayout;

  AnalogInChannelClass *analogInChannelObj[analogInChannelNum];
  CounterChannelClass *counterChannelObj[counterChannelNum];
  SerialChannelClass *serialChannelObj[serialChannelNum];
  PulseChannelClass *pulseChannelObj[DOBoxPortNum+pulseChannelNum];
  AnalogOutChannelClass *analogOutChannelObj[analogOutChannelNum];

};

#endif
