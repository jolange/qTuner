#ifndef FFTDEVICE_HPP_
#define FFTDEVICE_HPP_

#include <complex>

#include <QAudioInput>

namespace qTuner { class FFTDevice; }

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

private:
   const QAudioFormat m_audioFormat;
   int                m_iSampleBytes;
   int                m_iSamples;
   int                m_iFrequencyResolution;
   
   void fft(qint16               data[], double spectr[], int n);
   void fft(std::complex<double> data[], int n);
   int  maxPosition(double       data[], int n);
   double frequencyAt(int pos);
   
   void dump(qint16 data[], int n);
   void dump(double data[], int n);

};

#endif /* FFTDEVICE_HPP_ */ 
