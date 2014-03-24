#include "OutputDevice.hpp"

#include <complex>
#include <cmath>

#include <QDebug>
#include <QtEndian>

namespace qTuner{

OutputDevice::OutputDevice(const QAudioFormat &aFormat, QObject *parent):
   QIODevice(parent),
   m_audioFormat(aFormat)
{
   m_iSampleBytes = m_audioFormat.sampleSize() / 8;
}

OutputDevice::~OutputDevice(){}

void OutputDevice::start() { open(QIODevice::WriteOnly); }
void OutputDevice::stop()  { close(); }

qint64 OutputDevice::readData(char *data, qint64 maxlen)
{
   Q_UNUSED(data)
   Q_UNUSED(maxlen)
   return 0;
}

qint64 OutputDevice::writeData(const char *data, qint64 len)
{
   m_iSamples = len / m_iSampleBytes;

   qint16* values = new qint16[m_iSamples];
   const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
   for (int i = 0; i < m_iSamples; i++) {
      values[i] = qFromLittleEndian<qint16>(ptr);
      ptr += m_iSampleBytes;
   }
   dump(values, m_iSamples);

   fft(values, m_iSamples);
   delete[] values;
   return len;
}

void OutputDevice::fft(qint16 data[], int n)
{
   std::complex<double>* cData = new std::complex<double>[n];
   for (int i=0; i<n; i++)
      cData[i] = std::complex<double>((double)data[i],0.0);
   
   fft(cData,n);
   delete[] cData;
}

void OutputDevice::fft(std::complex<double> data[], int n)
{
   if (n == 1)
      return;
   
   // even and uneven partition
   std::complex<double>* ev = new std::complex<double>[n/2];
   std::complex<double>* ue = new std::complex<double>[n/2];
   for (int i=0; i<n/2; i++){
      ev[i] = data[2*i];
      ue[i] = data[2*i+1];
   }
   
   fft(ev, n/2);
   fft(ue, n/2);
   
   double phi;
   for (int k = 0; k < n/2-1; k++){
      phi = - 2.0 * M_PI * (double)k / (double)n;
      data[k]     = ev[k] + ue[k] * std::polar(1.0, phi);
      data[k+n/2] = ev[k] - ue[k] * std::polar(1.0, phi);
   }
   
   delete[] ev;
   delete[] ue;
}


void OutputDevice::dump(qint16 data[], int n)
{
   QString output;
   for (int i = 0; i < n; i++) {
      output += QString::number(data[i]) + " ";
   }
   qDebug() << output;
}
} // end namespace qTuner
