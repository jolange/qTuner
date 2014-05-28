#include "UIMain.hpp"
#include "ui_UIMain.h"

#include "NoteInfo.hpp"
#include "Tuning.hpp"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
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

   connect(ui.actionShowGrTuner , SIGNAL(toggled(bool)),
           ui.drawArea          , SLOT  (setVisible(bool)));
   connect(ui.actionShowGrTuner , SIGNAL(toggled(bool)),
           ui.widgetPresets     , SLOT  (setVisible(bool)));
   connect(ui.actionShowNumTuner, SIGNAL(toggled(bool)),
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
   m_audioInput = new QAudioInput(m_deviceInfo, m_audioFormat, this);

   connect(ui.sliderThreshold, SIGNAL(valueChanged(int)),
           m_FFTDevice       , SLOT  (slotSetSignalThreshold(int)));
   connect(m_FFTDevice       , SIGNAL(signalNoteUpdated(NoteInfo)),
           this              , SLOT  (slotUpdateNoteInfo(NoteInfo)));
   connect(m_FFTDevice       , SIGNAL(signalStatusMessage(QString,int)),
           statusBar()       , SLOT  (showMessage(QString,int)));

   // read settings after connections: toggles are recognized
   readSettings();

   m_FFTDevice ->start();
   m_audioInput->start(m_FFTDevice);
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
         int iString = tun.size()-i;
         stSym = tun.at(i);
         if (tun.count(stSym) > 1){
            // if multiple strings with same tuning: draw later smaller
            drawLater[(int)stSym].append(QString::number(i+1));
         }else{
            pos = (m_sizeDrawArea.width()/12.0)*((int)stSym+.5)-8;
            painter.drawText(pos,imgNoteMarkHeight-1,QString::number(iString));
         }
      }
      // draw double occurrences
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
   QIcon icon(":/img/qTuner.png");
   // no tuning preset by default
   m_lTuningPresets.append(Tuning(";None"));
   // read presets from file
   QFile tuningsFile(":/resources/tunings.conf");
   if(tuningsFile.open(QIODevice::ReadOnly)){
      QTextStream in(&tuningsFile);
      QString line;
      while (!in.atEnd()) {
         line = in.readLine();
         m_lTuningPresets.append(Tuning(line));
      }
      tuningsFile.close();
   }
   for (int i = 0; i < m_lTuningPresets.size(); i++){
      ui.cbPresets->addItem(icon,m_lTuningPresets[i].getName());   
   }
}

void UIMain::slotUpdateNoteInfo(NoteInfo note)
{
   if (note.getSymbol()!=err){
      updateDrawArea(note.getSemitone());
      double f = note.getFrequency();
      QString freq = QString(" (%1 Hz)")
                     .arg(f == 0 ? "?" : QString::number(f,'f',0));
      ui.labelFreq     ->setText(freq);
      ui.labelNote     ->setText(note.getSymbolString()+" ");
      double rem = note.getRemainder();
      QString sRem = QString::number(rem,'f',2);
      if (rem>=0) sRem.prepend("+");
      ui.labelRemainder->setText(sRem);
   }else{
      updateDrawArea(-100); // move out of sight
      ui.labelFreq     ->setText(" (? Hz)");
      ui.labelNote     ->setText("?");
      ui.labelRemainder->setText("+0.0");      
   }
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

void UIMain::writeSettings() const
{
   QSettings settings;
   settings.setValue("UIMain/size", size());
   settings.setValue("UIMain/pos",  pos());
   settings.setValue("UIMain/iTuning",         ui.cbPresets->currentIndex());
   settings.setValue("UIMain/bGraphicalTuner", ui.actionShowGrTuner->isChecked());
   settings.setValue("UIMain/bNumericalTuner", ui.actionShowNumTuner->isChecked());
   settings.setValue("UIMain/iThreshold",      ui.sliderThreshold->value());
}

void UIMain::readSettings()
{
   QSettings settings;
   resize(
      settings.value("UIMain/size", QSize(950, 400)).toSize()
   );
   move(
      settings.value("UIMain/pos", QPoint(50, 250)).toPoint()
   );
   ui.cbPresets->setCurrentIndex(
      settings.value("UIMain/iTuning", 0).toInt()
   );
   ui.actionShowGrTuner->setChecked(
      settings.value("UIMain/bGraphicalTuner", true).toBool()
   );
   ui.actionShowNumTuner->setChecked(
      settings.value("UIMain/bNumericalTuner", false).toBool()
   );
   ui.sliderThreshold->setValue(
      settings.value("UIMain/iThreshold", 60).toInt()
   );
}

void UIMain::closeEvent(QCloseEvent *event)
{
   writeSettings();
   event->accept();
}

} // end namespace qTuner
