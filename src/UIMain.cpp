#include "UIMain.hpp"
#include "ui_UIMain.h"

#include "NoteInfo.hpp"

#include <QDebug>
#include <QPainter>
#include <QString>

namespace qTuner{

UIMain::UIMain():
   QMainWindow(),
   m_sizeDrawArea(800, 100),
   m_deviceInfo(QAudioDeviceInfo::defaultInputDevice()),
   m_audioInput(0),
   m_FFTDevice(0)
{
   ui.setupUi(this);


   m_audioFormat.setSampleRate(32000);
   m_audioFormat.setChannels(1);
   m_audioFormat.setSampleSize(16);
   m_audioFormat.setSampleType(QAudioFormat::SignedInt);
   m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
   m_audioFormat.setCodec("audio/pcm");

   m_FFTDevice  = new FFTDevice(m_audioFormat, this);
   m_audioInput = new QAudioInput(m_deviceInfo, m_audioFormat, this);

   connect (m_FFTDevice, SIGNAL(signalNoteUpdated(NoteInfo)),
            this       , SLOT  (slotUpdateNoteInfo(NoteInfo)));


   m_FFTDevice ->start();
   m_audioInput->start(m_FFTDevice);

   setupDrawArea();
}

UIMain::~UIMain()
{
   delete m_FFTDevice;
   delete m_audioInput;
}

void UIMain::setupDrawArea()
{
   m_imgNoteMark.load(":img/marker.png");
   m_imgArrow.load(":/img/arrow.png");
   m_imgOctave = QImage(m_sizeDrawArea, QImage::Format_ARGB32_Premultiplied);
   QPainter painter(&m_imgOctave);
   painter.setPen(Qt::black);
   painter.setFont(QFont("Arial", 30, QFont::Bold));
   QString sNote;
   int pos;
   for (int i=0; i<12; i++){
      pos = (m_sizeDrawArea.width()/12.0)*(i+.5);
      painter.drawImage(pos-m_imgNoteMark.width()/2,0,m_imgNoteMark);
      sNote = NoteInfo::getSymbolString((SemiToneSymbol)i);
      if (sNote.size() == 1){
         pos -= 14;
      }else{
         pos -= 26;
      }
      painter.drawText(pos,100,sNote);
   }

   painter.end();
   ui.drawArea->setMinimumWidth(m_sizeDrawArea.width());
   ui.drawArea->resize(m_sizeDrawArea);
   ui.drawArea->setPixmap(QPixmap::fromImage(m_imgOctave));
}

void UIMain::updateDrawArea(double semitone)
{
   QImage imgTotal(m_sizeDrawArea, QImage::Format_ARGB32_Premultiplied);
   QPainter painter(&imgTotal);
   painter.drawImage(0,0,m_imgOctave);
   double pos = fmod(semitone+.5,12.0) * m_sizeDrawArea.width()/12.0;
   painter.drawImage(pos-m_imgArrow.width()/2,0,m_imgArrow);
   painter.end();
   ui.drawArea->setPixmap(QPixmap::fromImage(imgTotal));
}

void UIMain::slotUpdateNoteInfo(NoteInfo note)
{
   updateDrawArea(note.getSemitone());
   ui.labelNote     ->setText(note.getSymbolString());
   ui.labelRemainder->setText(QString::number(note.getRemainder(),'f',2));
}

} // end namespace qTuner
