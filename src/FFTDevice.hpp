#ifndef FFTDEVICE_HPP_
#define FFTDEVICE_HPP_

// do this before any other math include,
// because in Windows this is disabled by default
#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>

#include <QAudioInput>

namespace qTuner
{
   class FFTDevice; 
   enum  SemiToneSymbol {A=0, Bb=1, B=2, C=3, Db=4, D=5, Eb=6,
                         E=7, F=8, Gb=9, G=10, Ab=11};
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
   
   void setResolutionFactor(int res); // res \in [1,20]

private:
   const QAudioFormat m_audioFormat;
   int                m_iSampleBytes;
   int                m_iSamples;
   int                m_iResolutionFactor;
   int                m_iSpectrumSize; // legth of the array the FFT is performed on
   
   const double A440; // Hz
   
   void fft(qint16               data[], double spectr[]);
   void fft(std::complex<double> data[], int n);
   void calcSpectrumSize();
   int  maxPosition(double       data[], int n);
   double  frequencyAt(int pos);
   double  semitone(double freq);
   QString semitoneSymbol(double semitone);
   
   void dump(qint16 data[], int n);
   void dump(double data[], int n);
};

#endif /* FFTDEVICE_HPP_ */ 
