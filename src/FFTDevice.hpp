#ifndef FFTDEVICE_HPP_
#define FFTDEVICE_HPP_

// define this before any other math include,
// because in Windows this is disabled by default
#define _USE_MATH_DEFINES

#include "NoteInfo.hpp"


#include <cmath>
#include <complex>

#include <QAudioInput>

namespace qTuner
{
   class FFTDevice;
}

class qTuner::FFTDevice : public QIODevice
{
Q_OBJECT

public:
   FFTDevice(const QAudioFormat &aFormat, QObject *parent);
   virtual ~FFTDevice();

   void start();
   void stop();

   qint64 readData(char *data, qint64 maxlen);
   qint64 writeData(const char *data, qint64 len);

   // TODO get rid of this or change usage
   void setResolutionFactor(int res); // res \in [1,20]

private:
   const QAudioFormat m_audioFormat;
   int                m_iSampleBytes;
   int                m_iSamples;
   int                m_iResolutionFactor;
   int                m_iSpectrumSize;    // legth of the array the FFT is performed on
   int                m_iSignalThreshold; // signal only valid if avg absolute amplitude 
   NoteInfo           m_note;

   bool fft(qint16               data[], double spectr[]) const;
   void fft(std::complex<double> data[], int n)           const;
   void calcSpectrumSize();
   int  maxPosition(double       data[], int n)           const;
   double frequencyAt(int pos)                            const;

   void dump(qint16 data[], int n);
   void dump(double data[], int n);

public slots:
   void slotSetSignalThreshold(int value);

signals:
   void signalNoteUpdated(NoteInfo);
};

#endif /* FFTDEVICE_HPP_ */
