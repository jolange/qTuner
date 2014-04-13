#include "UIMain.hpp"
#include "ui_UIMain.h"

#include "NoteInfo.hpp"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
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
   setWindowIcon(QIcon(":/img/qTuner.png"));
   ui.widgetNumTuner->setVisible(false);
   
   connect(ui.actionShowGrTuner , SIGNAL(triggered(bool)),
           ui.drawArea          , SLOT  (setVisible(bool)));
   connect(ui.actionShowNumTuner, SIGNAL(triggered(bool)),
           ui.widgetNumTuner    , SLOT  (setVisible(bool)));
   connect(ui.actionAbout, SIGNAL(activated()),
           this          , SLOT  (slotShowAboutDialog()));

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
   m_imgNoteMark = QPixmap(20,60);
   m_imgNoteMark.fill(QApplication::palette().color(QPalette::WindowText));
   m_imgArrow.load(":/img/arrow.png");
   m_imgOctave = QPixmap(m_sizeDrawArea);
   m_imgOctave.fill(Qt::transparent);
   QPainter painter(&m_imgOctave);
   painter.setFont(QFont("Arial", 30, QFont::Bold));
   painter.setPen(QApplication::palette().color(QPalette::WindowText));
   QString sNote;
   int pos;
   for (int i=0; i<12; i++){
      pos = (m_sizeDrawArea.width()/12.0)*(i+.5);
      painter.drawPixmap(pos-m_imgNoteMark.width()/2,0,m_imgNoteMark);
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
   ui.drawArea->setPixmap(m_imgOctave);
}

void UIMain::updateDrawArea(double semitone)
{
   QPixmap imgTotal(m_sizeDrawArea);
   imgTotal.fill(Qt::transparent);
   QPainter painter(&imgTotal);
   painter.drawPixmap(0,0,m_imgOctave);
   double pos = fmod(semitone+.5,12.0) * m_sizeDrawArea.width()/12.0;
   painter.drawPixmap(pos-m_imgArrow.width()/2,0,m_imgArrow);
   painter.end();
   ui.drawArea->setPixmap(imgTotal);
}

void UIMain::slotUpdateNoteInfo(NoteInfo note)
{
   updateDrawArea(note.getSemitone());
   double f = note.getFrequency();
   QString freq = QString(" (%1 Hz)")
                  .arg(f == 0 ? "?" : QString::number(f,'f',0));
   ui.labelFreq     ->setText(freq);
   ui.labelNote     ->setText(note.getSymbolString()+" ");
   ui.labelRemainder->setText(QString::number(note.getRemainder(),'f',2));
}

void UIMain::slotShowAboutDialog()
{
   QString text = "<b>qTuner</b> by Johannes Lange"; // backup text
   #define STRING1(x) #x
   #define STRING(x) STRING1(x)
   QString version( STRING(PROJECTVERSION) );
   #undef STRING
   #undef STRING1
   QFile aboutFile(":/resources/aboutText.html");
   if(aboutFile.open(QIODevice::ReadOnly)){
      text = aboutFile.readAll();
      text = text.arg(version).arg( __DATE__ );
      aboutFile.close();
   }
   QMessageBox::about(this, QString("About qTuner"), text);
}

} // end namespace qTuner
