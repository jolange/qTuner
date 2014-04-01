#ifndef UIMAIN_HPP_
#define UIMAIN_HPP_

#include "ui_UIMain.h"

#include "FFTDevice.hpp"
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
   Ui::UIMain ui;
   
   QSize  m_sizeDrawArea;
   QImage m_imgOctave;
   
   QAudioFormat     m_audioFormat;
   QAudioDeviceInfo m_deviceInfo;
   QAudioInput*     m_audioInput;
   FFTDevice*       m_FFTDevice;
   
   void setupDrawArea();
   void updateDrawArea(double semitone);
private slots:
   void slotUpdateNoteInfo(NoteInfo note);
};
#endif // UIMAIN_HPP_
