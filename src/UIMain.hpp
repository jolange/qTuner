#ifndef UIMAIN_HPP_
#define UIMAIN_HPP_

#include "ui_UIMain.h"

#include "FFTDevice.hpp"
#include "Tuning.hpp"
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

   QSize   m_sizeDrawArea;
   QPixmap m_imgNoteMark;
   QPixmap m_imgArrow;
   QPixmap m_imgOctave;

   QAudioFormat     m_audioFormat;
   QAudioDeviceInfo m_deviceInfo;
   QAudioInput*     m_audioInput;
   FFTDevice*       m_FFTDevice;

   QList<Tuning> m_lTuningPresets;

   void updateDrawArea(double semitone);
   void setupTunings();
private slots:
   void slotSetupDrawArea();
   void slotUpdateNoteInfo(NoteInfo note);
   void slotShowAboutDialog();
};
#endif // UIMAIN_HPP_
