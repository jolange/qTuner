#ifndef UIMAIN_HPP_
#define UIMAIN_HPP_

//#include "ui_UIMain.h"

#include "OutputDevice.hpp"
#include <QMainWindow>

namespace qTuner { class UIMain; }

/*!
 * \class UIMain
 * Main Window
 */
class qTuner::UIMain: public QMainWindow
{
Q_OBJECT
   
public:
   UIMain();
   virtual ~UIMain();
private:
   //Ui::UIMain ui;
   QAudioFormat     m_audioFormat;
   QAudioDeviceInfo m_deviceInfo;
   QAudioInput*     m_audioInput;
   OutputDevice*    m_outputDevice;
   
};
#endif // UIMAIN_HPP_
