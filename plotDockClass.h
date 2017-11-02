#ifndef PLOTDOCKCLASS_H
#define PLOTDOCKCLASS_H

#include <QtWidgets>
#include <QObject>
#include <QMainWindow>
#include "analogInChannelClass.h"
#include "qcustomplot.h"

#define plotRefreshRate 20
#define plotDockNum 2

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QLineEdit;
class QTextEdit;
class QGroupBox;
class QLabel;
class QComboBox;
class QCheckBox;
class QGridLayout;
class QInputDialog;
QT_END_NAMESPACE

class PlotDockClass : public QWidget
{
  Q_OBJECT

private:
  void createSettingDialog();

  // double plotXRange;
  bool xAxisRescaleFlag;
  bool yAxisRescaleFlag;

  // setting dialog layout
  QWidget *settingDialog;
  QPushButton *settingOkButton;
  QPushButton *settingCancelButton;
  QGroupBox *xAxisGroupBox;
  QLabel *xAxisLabel;
  QCheckBox *xAxisCheckBox;
  QLabel *xAxisRangeLabel;
  QLineEdit *xAxisRangeLowLine;
  QLabel *xAxisRangeToLabel;
  QLineEdit *xAxisRangeHighLine;
  QGridLayout *xAxisLayout;
  QGroupBox *yAxisGroupBox;
  QLabel *yAxisLabel;
  QCheckBox *yAxisCheckBox;
  QLabel *yAxisRangeLabel;
  QLineEdit *yAxisRangeLowLine;
  QLabel *yAxisRangeToLabel;
  QLineEdit *yAxisRangeHighLine;
  QGridLayout *yAxisLayout;
  QGridLayout *settingLayout;
  
  // store values to restore when the cancel button is clicked
  QString xAxisRangeLowStored;
  QString xAxisRangeHighStored;
  bool xAxisCheckStored;
  QString yAxisRangeLowStored;
  QString yAxisRangeHighStored;
  bool yAxisCheckStored;
//  static int plotDockCount;

public slots:
  void plotInComboBoxChanged(int);
  void xAxisChannelComboBoxChanged(int);
  void settingButtonPressed();
  void clearButtonPressed();
  void xAxisCheckBoxClicked();
  void yAxisCheckBoxClicked();
  void okButtonClicked();
  void cancelButtonClicked();

public:
  PlotDockClass(QWidget *parent = 0);

  // plot dock layout
  QCustomPlot *inPlot;
  QComboBox *plotInComboBox;
  // QLineEdit *plotReadLine;
  // QLineEdit *plotConvertLine;
  QTimer *plotUpdateTimer;
  QComboBox *xAxisChannelComboBox;
  QComboBox *xAxisTime1ComboBox;
  QComboBox *xAxisTime2ComboBox;

  void plotFreqScan(float init, int dataNum, int size, float step, float *detune, int *data);
  void plotDockUpdate(double low, double high, int size, int timeStep, uInt32 data[]);

//  QDockWidget *plotDock;  
};

#endif

