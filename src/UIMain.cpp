#include "UIMain.hpp"
#include "ui_UIMain.h"

#include "NoteInfo.hpp"
#include "Tuning.hpp"

#include <QtConcurrentRun>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QString>

namespace qTuner{

UIMain::UIMain():
   QMainWindow(),
   m_sizeDrawArea(800, 140),
   m_deviceInfo(QAudioDeviceInfo::defaultInputDevice()),
   m_audioInput(0),
   m_FFTDevice(0)
{
   ui.setupUi(this);
   setWindowIcon(QIcon(":/img/qTuner.png"));
   ui.widgetNumTuner->setVisible(false);
   setupTunings();
   slotSetupDrawArea();

   connect(ui.actionShowGrTuner , SIGNAL(triggered(bool)),
           ui.drawArea          , SLOT  (setVisible(bool)));
   connect(ui.actionShowGrTuner , SIGNAL(triggered(bool)),
           ui.widgetPresets     , SLOT  (setVisible(bool)));
   connect(ui.actionShowNumTuner, SIGNAL(triggered(bool)),
           ui.widgetNumTuner    , SLOT  (setVisible(bool)));
   connect(ui.actionAbout, SIGNAL(activated()),
           this          , SLOT  (slotShowAboutDialog()));
   connect(ui.cbPresets, SIGNAL(currentIndexChanged(int)),
           this        , SLOT  (slotSetupDrawArea()));

   m_audioFormat.setSampleRate(32000);
   m_audioFormat.setChannels(1);
   m_audioFormat.setSampleSize(16);
   m_audioFormat.setSampleType(QAudioFormat::SignedInt);
   m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
   m_audioFormat.setCodec("audio/pcm");

   m_FFTDevice  = new FFTDevice(m_audioFormat, this);
   //m_FFTDevice->moveToThread(&m_fftThread);
   m_audioInput = new QAudioInput(m_deviceInfo, m_audioFormat, this);

   connect(m_FFTDevice, SIGNAL(signalNoteUpdated(NoteInfo)),
           this       , SLOT  (slotUpdateNoteInfo(NoteInfo)));

   m_FFTDevice ->start();
   m_audioInput->start(m_FFTDevice);
   //QtConcurrent::run(m_FFTDevice, &FFTDevice::start);
   //QtConcurrent::run(m_audioInput, &QAudioInput::start,m_FFTDevice);
}

UIMain::~UIMain()
{
   delete m_FFTDevice;
   delete m_audioInput;
}

void UIMain::slotSetupDrawArea()
{
   int imgNoteMarkHeight = 100;
   m_imgNoteMark = QPixmap(20,imgNoteMarkHeight);
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
      painter.drawText(pos,imgNoteMarkHeight+40,sNote);
   }

   // draw string numbers of tuning preset:
   painter.setPen(QApplication::palette().color(QPalette::Window));
   painter.setFont(QFont("Arial", 20, QFont::Bold));
   int iPreset = ui.cbPresets->currentIndex();
   Tuning tun = m_lTuningPresets[iPreset];
   if (!tun.isEmpty()){
      QVector<QStringList> drawLater(12); // index = position, QStringList = Labels
      SemiToneSymbol stSym;
      for (int i = 0; i < tun.size(); i++){
         stSym = tun.at(i);
         if (tun.count(stSym) > 1){
            // if multiple strings with same tuning: draw later smaller
            drawLater[(int)stSym].append(QString::number(i+1));
         }else{
            pos = (m_sizeDrawArea.width()/12.0)*((int)stSym+.5)-8;
            painter.drawText(pos,imgNoteMarkHeight-1,QString::number(i+1));
         }
      }
      // draw double occurrences
      // TODO just one column if <= 3?
      painter.setFont(QFont("Arial", 10, QFont::Bold));
      for (int iPos = 0; iPos < drawLater.size(); iPos++){
         QStringList& strings = drawLater[iPos];
         int iStrings = strings.size();
         for (int iStr = 0; iStr < iStrings; iStr++){
            pos = (m_sizeDrawArea.width()/12.0)*(iPos+.5)-8;
            if (iStr%2 == 1) // column alignment
               pos += 10;
            double height = imgNoteMarkHeight-1 - ((iStrings-1)/2-iStr/2) * 16;
            painter.drawText(pos,height,strings[iStr]);
         }
      }
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

void UIMain::setupTunings()
{
   m_lTuningPresets.append(Tuning(";None"));
   m_lTuningPresets.append(Tuning("E,A,D,G,B,E;Standard Tuning"));
   m_lTuningPresets.append(Tuning("D,A,D,G,B,E;Drop D"));
   m_lTuningPresets.append(Tuning("E,E,E;Test"));
   m_lTuningPresets.append(Tuning("E,E,E,E,E,E,E;Test"));
   QIcon icon(":/img/qTuner.png");
   for (int i = 0; i < m_lTuningPresets.size(); i++){
      ui.cbPresets->addItem(icon,m_lTuningPresets[i].getName());   
   }
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
