#ifndef OUTPUTDEVICE_HPP_
#define OUTPUTDEVICE_HPP_

#include <complex>

#include <QAudioInput>

namespace qTuner { class OutputDevice; }

class qTuner::OutputDevice : public QIODevice
{
Q_OBJECT

public:
   OutputDevice(const QAudioFormat &aFormat, QObject *parent);
   virtual ~OutputDevice();

   void start();
   void stop();

   qint64 readData(char *data, qint64 maxlen);
   qint64 writeData(const char *data, qint64 len);

private:
   const QAudioFormat m_audioFormat;
   int                m_iSampleBytes;
   int                m_iSamples;
   
   void fft(qint16               data[], int n);
   void fft(std::complex<double> data[], int n);
   
   void dump(qint16 data[], int n);

};

#endif /* OUTPUTDEVICE_HPP_ */ 
