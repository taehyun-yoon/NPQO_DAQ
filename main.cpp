#include <QApplication>

#include "mainwindow.h"

int ChannelClass::analogInChannelCount = 0;
int ChannelClass::counterChannelCount = 0;
int ChannelClass::serialChannelCount = 0;
int ChannelClass::pulseChannelCount = DOBoxPortNum;
int ChannelClass::analogOutChannelCount = 0;
int ChannelClass::clockSpeed = 10;
uInt32 PulseChannelClass::P0dataSingle = 0x00000000;
// TaskHandle PulseChannelClass::P0taskHandleSingle = 0;
// uInt64 PulseChannelClass::samplesPerChan = 1;
// float64 PulseChannelClass::timeout = 10.0;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(NPQO_DAQ);
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
