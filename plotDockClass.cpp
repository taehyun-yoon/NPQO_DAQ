#include "plotDockClass.h"

PlotDockClass::PlotDockClass(QWidget *parent) : QWidget(parent)
{
  inPlot = new QCustomPlot(this);

  plotInComboBox = new QComboBox;
  xAxisRescaleFlag = true;
  yAxisRescaleFlag = true;

  createSettingDialog();

  // QLabel *plotReadLabel = new QLabel(tr("Read"));
  // plotReadLabel->setAlignment(Qt::AlignCenter);

  // plotReadLine = new QLineEdit;
  // plotReadLine->setText("0");
  // plotReadLine->setAlignment(Qt::AlignCenter);

  // QLabel *plotConvertLabel = new QLabel(tr("Converted"));
  // plotConvertLabel->setAlignment(Qt::AlignCenter);

  // plotConvertLine = new QLineEdit;
  // plotConvertLine->setText("0");
  // plotConvertLine->setAlignment(Qt::AlignCenter);

  QPushButton *plotSettingButton = new QPushButton(tr("&Setting"));
  connect(plotSettingButton, SIGNAL(clicked()),this,SLOT(settingButtonPressed()));

  QPushButton *plotClearButton = new QPushButton(tr("&Clear"));
  connect(plotClearButton, SIGNAL(clicked()),this,SLOT(clearButtonPressed()));

  QGridLayout *inPlotLayout = new QGridLayout;
  inPlotLayout->addWidget(plotInComboBox,0,0,1,2);
  inPlotLayout->addWidget(plotSettingButton,0,2,1,2);
  inPlotLayout->addWidget(plotClearButton,1,2,1,2);
  // inPlotLayout->addWidget(plotReadLabel,2,0);
  // inPlotLayout->addWidget(plotReadLine,2,1);
  // inPlotLayout->addWidget(plotConvertLabel,2,2);
  // inPlotLayout->addWidget(plotConvertLine,2,3);
  inPlotLayout->setColumnStretch(0,1);
  inPlotLayout->setColumnStretch(1,1);
  inPlotLayout->setColumnStretch(2,1);
  inPlotLayout->setColumnStretch(3,1);
  inPlotLayout->addWidget(inPlot,2,0,2,4);

  setLayout(inPlotLayout);

  inPlot->addGraph(); // blue line
  inPlot->graph(0)->setPen(QPen(Qt::blue));
  inPlot->graph(0)->setAntialiasedFill(false);

  inPlot->addGraph(); // red dot
  inPlot->graph(1)->setPen(QPen(Qt::red));
  inPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  inPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);

//  inPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
//  inPlot->xAxis->setDateTimeFormat("ms");
//  inPlot->xAxis->setAutoTickStep(false);
//  inPlot->xAxis->setTickStep(2);
//  inPlot->axisRect()->setupFullAxesBox();


  // make left and bottom axes transfer their ranges to right and top axes:
  // connect(inPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), inPlot->xAxis2, SLOT(setRange(QCPRange)));
  // connect(inPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), inPlot->yAxis2, SLOT(setRange(QCPRange)));

  // plotUpdateTimer = new QTimer(this);
}  

void PlotDockClass::plotInComboBoxChanged(int i)
{
    if (i == 0) {
  //    plotUpdateTimer->stop();
      inPlot->graph(0)->clearData();
  //    inPlot->replot();
  //    inPlot->clearGraphs();
    }
    else {
  //    plotUpdateTimer->stop();
      inPlot->graph(0)->clearData();
  //    inPlot->clearGraphs();
  //    plotUpdateTimer->start(plotRefreshRate);
    }
}

void PlotDockClass::xAxisChannelComboBoxChanged(int i)
{
    if (i == 0) {
        xAxisTime1ComboBox->setEnabled(false);
        xAxisTime2ComboBox->setEnabled(false);
    }
    else {
        xAxisTime1ComboBox->setEnabled(true);
        xAxisTime2ComboBox->setEnabled(true);
    }

}

// void PlotDockClass::plotFreqScan(float init, int dataNum, int size, float step, int *data)
void PlotDockClass::plotFreqScan(float init, int dataNum, int size, float step, float *detune, int *data)
{
    double maxCount = 0;
    QVector<double> x(dataNum), y(dataNum); // initialize with entries 0..100
    for (int i=0; i<dataNum; ++i)
    {
        x[i] = (double)detune[i];
        y[i] = (double)data[i];
        // if (data[i] > maxCount) { maxCount = data[i]; }
    }
    inPlot->graph(0)->clearData();
    inPlot->graph(0)->setData(x, y);
    inPlot->graph(0)->rescaleAxes();
    inPlot->xAxis->setRange(init-step,init+size*step);
    if (yAxisRescaleFlag == false) {
        inPlot->yAxis->setRange((yAxisRangeLowLine->text()).toDouble(), (yAxisRangeHighLine->text()).toDouble());
    }
    inPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    inPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    // inPlot->yAxis->setRange(0, maxCount);
    inPlot->replot();
}

void PlotDockClass::plotDockUpdate(double low, double high, int size, int timeStep, uInt32 data[])
{
    QVector<double> x(size), y(size); // initialize with entries 0..100
    int i;
    for (int i=1; i<size; ++i)
    {
        x[i] = (i*timeStep)/1000.0;
        y[i] = (double)(data[i]-data[i-1]);
    }
    inPlot->graph(0)->clearData();
    inPlot->graph(0)->setData(x, y);
    //inPlot->graph(0)->addData(x, z);
      // set data of dots:
    //inPlot->graph(1)->clearData();
    //inPlot->graph(1)->addData(x, z);
    // remove data of lines that's outside visible range:
    // inPlot->graph(0)->removeDataBefore(x-plotXRange);

    // rescale value (vertical) axis to fit the current data:
    inPlot->graph(0)->rescaleAxes();
    if (xAxisRescaleFlag == false) {
        inPlot->xAxis->setRange(low+(xAxisRangeLowLine->text()).toDouble(), high+(xAxisRangeHighLine->text()).toDouble());
    }
    if (yAxisRescaleFlag == false) {
        inPlot->yAxis->setRange((yAxisRangeLowLine->text()).toDouble(), (yAxisRangeHighLine->text()).toDouble());
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    // inPlot->xAxis->setRange(x+0.1*plotXRange, plotXRange, Qt::AlignRight);
    // inPlot->xAxis->setRange((xAxisRangeLowLine->text()).toDouble(), (xAxisRangeHighLine->text()).toDouble());
    inPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
    inPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    inPlot->replot();

    // update read and converted values
    // plotReadLine->setText(QString::number(y));
    // plotConvertLine->setText(QString::number(z));
}

void PlotDockClass::clearButtonPressed()
{
  inPlot->graph(0)->clearData();
}

void PlotDockClass::createSettingDialog()
{
    settingDialog = new QWidget;

    settingOkButton = new QPushButton(tr("&OK"));
    settingCancelButton = new QPushButton(tr("&Cancel"));

    xAxisGroupBox = new QGroupBox(tr("x-axis"));
    xAxisLabel = new QLabel;
    xAxisLabel->setText("Full scale");
    xAxisCheckBox = new QCheckBox;
    xAxisCheckBox->setChecked(1);
    xAxisRangeLabel = new QLabel;
    xAxisRangeLabel->setText("Range [ms]");
    xAxisRangeLabel->setAlignment(Qt::AlignCenter);
    xAxisChannelComboBox = new QComboBox;
    xAxisChannelComboBox->setEnabled(false);
    xAxisTime1ComboBox = new QComboBox;
    xAxisTime1ComboBox->setEnabled(false);
    xAxisTime2ComboBox = new QComboBox;
    xAxisTime2ComboBox->setEnabled(false);
    xAxisRangeLowLine = new QLineEdit;
    xAxisRangeLowLine->setEnabled(false);
    xAxisRangeToLabel = new QLabel("to");
    xAxisRangeToLabel->setAlignment(Qt::AlignCenter);
    xAxisRangeHighLine = new QLineEdit;
    xAxisRangeHighLine->setEnabled(false);

    xAxisLayout = new QGridLayout;
    xAxisLayout->addWidget(xAxisLabel,0,0);
    xAxisLayout->addWidget(xAxisCheckBox,0,1);
    xAxisLayout->addWidget(xAxisRangeLabel,1,0);
    xAxisLayout->addWidget(xAxisChannelComboBox,1,1);
    xAxisLayout->addWidget(xAxisTime1ComboBox,1,2);
    xAxisLayout->addWidget(xAxisRangeLowLine,1,3);
    xAxisLayout->addWidget(xAxisRangeToLabel,1,4);
    xAxisLayout->addWidget(xAxisTime2ComboBox,1,5);
    xAxisLayout->addWidget(xAxisRangeHighLine,1,6);
    xAxisLayout->setColumnStretch(0,1);
    xAxisLayout->setColumnStretch(1,1);
    xAxisLayout->setColumnStretch(2,1);
    xAxisLayout->setColumnStretch(3,1);
    xAxisLayout->setColumnStretch(4,1);
    xAxisLayout->setColumnStretch(5,1);
    xAxisLayout->setColumnStretch(6,1);
    xAxisGroupBox->setLayout(xAxisLayout);

    yAxisGroupBox = new QGroupBox(tr("y-axis"));
    yAxisLabel = new QLabel;
    yAxisLabel->setText("Auto scale");
    yAxisCheckBox = new QCheckBox;
    yAxisCheckBox->setChecked(1);
    yAxisRangeLabel = new QLabel;
    yAxisRangeLabel->setText("Range");
    yAxisRangeLabel->setAlignment(Qt::AlignCenter);
    yAxisRangeLowLine = new QLineEdit;
    yAxisRangeLowLine->setEnabled(false);
    yAxisRangeToLabel = new QLabel("to");
    yAxisRangeToLabel->setAlignment(Qt::AlignCenter);
    yAxisRangeHighLine = new QLineEdit;
    yAxisRangeHighLine->setEnabled(false);

    yAxisLayout = new QGridLayout;
    yAxisLayout->addWidget(yAxisLabel,0,0);
    yAxisLayout->addWidget(yAxisCheckBox,0,1);
    yAxisLayout->addWidget(yAxisRangeLabel,1,0);
    yAxisLayout->addWidget(yAxisRangeLowLine,1,1);
    yAxisLayout->addWidget(yAxisRangeToLabel,1,2);
    yAxisLayout->addWidget(yAxisRangeHighLine,1,3);
    yAxisLayout->setColumnStretch(0,1);
    yAxisLayout->setColumnStretch(1,1);
    yAxisLayout->setColumnStretch(2,1);
    yAxisLayout->setColumnStretch(3,1);
    yAxisGroupBox->setLayout(yAxisLayout);

    settingLayout = new QGridLayout;
    settingLayout->addWidget(xAxisGroupBox,0,0,1,2);
    settingLayout->addWidget(yAxisGroupBox,1,0,1,2);
    settingLayout->addWidget(settingOkButton,2,0);
    settingLayout->addWidget(settingCancelButton,2,1);
    settingDialog->setLayout(settingLayout);

    connect(xAxisCheckBox,SIGNAL(toggled(bool)),this,SLOT(xAxisCheckBoxClicked()));
    connect(yAxisCheckBox,SIGNAL(toggled(bool)),this,SLOT(yAxisCheckBoxClicked()));
    connect(settingOkButton,SIGNAL(clicked()),this,SLOT(okButtonClicked()));
    connect(settingCancelButton,SIGNAL(clicked()),this,SLOT(cancelButtonClicked()));
}

void PlotDockClass::settingButtonPressed()
{
    settingDialog->show();
    // store values to restore when the cancel button is clicked
    xAxisRangeLowStored = xAxisRangeLowLine->text();
    xAxisRangeHighStored = xAxisRangeHighLine->text();
    xAxisCheckStored = xAxisCheckBox->isChecked();
    yAxisRangeLowStored = yAxisRangeLowLine->text();
    yAxisRangeHighStored = yAxisRangeHighLine->text();
    yAxisCheckStored = yAxisCheckBox->isChecked();
}

void PlotDockClass::xAxisCheckBoxClicked()
{
    if (xAxisCheckBox->isChecked()) {
        xAxisChannelComboBox->setEnabled(false);
        xAxisTime1ComboBox->setEnabled(false);
        xAxisTime2ComboBox->setEnabled(false);
        xAxisRangeLowLine->setEnabled(false);
        xAxisRangeHighLine->setEnabled(false);
    }
    else {
        xAxisChannelComboBox->setEnabled(true);
        if (xAxisChannelComboBox->currentIndex() != 0) {
            xAxisTime1ComboBox->setEnabled(true);
            xAxisTime2ComboBox->setEnabled(true);
        }
        xAxisRangeLowLine->setEnabled(true);
        xAxisRangeHighLine->setEnabled(true);
    }
}

void PlotDockClass::yAxisCheckBoxClicked()
{
  if (yAxisCheckBox->isChecked()) {
    yAxisRangeLowLine->setEnabled(false);
    yAxisRangeHighLine->setEnabled(false);
  }
  else {
    yAxisRangeLowLine->setEnabled(true);
    yAxisRangeHighLine->setEnabled(true);
  }
}

void PlotDockClass::okButtonClicked()
{
    if (xAxisCheckBox->isChecked()) {
      xAxisRescaleFlag = true;
    }
    else {
      xAxisRescaleFlag = false;
      // inPlot->xAxis->setRange((xAxisRangeLowLine->text()).toDouble(), (xAxisRangeHighLine->text()).toDouble());
    }

    if (yAxisCheckBox->isChecked()) {
      yAxisRescaleFlag = true;
    }
    else {
      yAxisRescaleFlag = false;
      // inPlot->yAxis->setRange((yAxisRangeLowLine->text()).toDouble(), (yAxisRangeHighLine->text()).toDouble());
    }

    settingDialog->close();
}

void PlotDockClass::cancelButtonClicked()
{
    // restore values
    xAxisRangeLowLine->setText(xAxisRangeLowStored);
    xAxisRangeHighLine->setText(xAxisRangeHighStored);
    yAxisRangeLowLine->setText(yAxisRangeLowStored);
    yAxisRangeHighLine->setText(yAxisRangeHighStored);
    yAxisCheckBox->setChecked(yAxisCheckStored);

    settingDialog->close();
}
