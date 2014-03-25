#include "UIMain.hpp"
//#include "ui_UIMain.h"

//#include <iostream>
#include <QDebug>
#include <QString>

namespace qTuner{

UIMain::UIMain():
   QMainWindow(),
   m_deviceInfo(QAudioDeviceInfo::defaultInputDevice()),
   m_audioInput(0),
   m_FFTDevice(0)
{
   //ui.setupUi(this);

   m_audioFormat.setSampleRate(16000);
   m_audioFormat.setChannels(1);
   m_audioFormat.setSampleSize(16);
   m_audioFormat.setSampleType(QAudioFormat::SignedInt);
   m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
   m_audioFormat.setCodec("audio/pcm");

   m_FFTDevice  = new FFTDevice(m_audioFormat, this);
   m_audioInput = new QAudioInput(m_deviceInfo, m_audioFormat, this);
   m_FFTDevice ->start();
   m_audioInput->start(m_FFTDevice);
}

UIMain::~UIMain()
{
   delete m_FFTDevice;
   delete m_audioInput;
}

} // end namespace qTuner
