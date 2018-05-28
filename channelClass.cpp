#include "channelClass.h"

ChannelClass::ChannelClass(QString str1)
{
  channelCheckBox = new QCheckBox();

  // Making the terminal name
  if (str1 == "AI") 
  { 
    str1.append(QString::number(analogInChannelCount));
 
    // Making the device and channel name
    char channelNum[1];
    sprintf(channelNum,"%d",analogInChannelCount);
    strcpy(chan,"Dev1/ai");
    strcat(chan,channelNum);
    str2 = "Analog in";
 
    analogInChannelCount++;
  }
  else if (str1 == "P2")
  {
    str1.append(".");
    str1.append(QString::number(counterChannelCount));
    // Making the device and channel name
    char channelNum[1];
    sprintf(channelNum,"%d",counterChannelCount);
    strcpy(chan,"Dev1/ctr");
    strcat(chan,channelNum);
    if(counterChannelCount == 0) { strcpy(extClockChan,"Dev1/PFI9"); }
    else if (counterChannelCount == 1){ strcpy(extClockChan,"Dev1/PFI17"); }
    str2 = "Counter";

    counterChannelCount++;
  }

  else if (str1 == "AO") 
  { 
    str1.append(QString::number(analogOutChannelCount));

    // Making the device and channel name
    char channelNum[1];
    sprintf(channelNum,"%d",analogOutChannelCount);
    strcpy(chan,"Dev1/ao");
    strcat(chan,channelNum);
    str2 = "Analog out";

    analogOutChannelCount++;
  }

  else if (str1 == "P0")
  {
    str1.append(".");

    // Making the device and channel name
    // char channelNum[1];
    // sprintf(channelNum,"%d",pulseChannelCount);
    strcpy(chan,"Dev1/port0");
    if (serialChannelCount < serialChannelNum) {
      str1.append(QString::number(serialChannelCount));
      P0lineNum = serialChannelCount;
      str2 = "Serial";
      serialChannelCount++;
    }
    else {
      str1.append(QString::number(pulseChannelCount));
      P0lineNum = pulseChannelCount;
      str2 = "Pulse out";
      pulseChannelCount++;
    }
  }

  channelTerminalLabel = new QLabel(str1);

  // Making the channel name
  channelNameLine = new QLineEdit;
  channelNameLine->setText(str2);
  channelNameLine->setAlignment(Qt::AlignCenter);
  channelNameLine->setStyleSheet("QLineEdit { background-color: transparent; }");
  channelNameLine->setEnabled(false);

  array = str2.toLocal8Bit();
  chanTitle = array.data();

  connect(channelNameLine,SIGNAL(editingFinished()),this,SLOT(channelNameLineChanged()));
}

int ChannelClass::getClockSpeed()
{
  return clockSpeed;
}

void ChannelClass::setClockSpeed(int t)
{
  clockSpeed = t;
}

int ChannelClass::getRound(int t, int ref)
{
  if(t <= ref) { return ref; }
  else {
    int q = t/ref;
    int r = t%ref;
    if(r < (0.5*ref)) { return (q*ref); }
    else { return ((q+1)*ref); }
  }
}

void ChannelClass::channelNameLineChanged()
{
    array = (channelNameLine->text()).toLocal8Bit();
    chanTitle = array.data();
}
