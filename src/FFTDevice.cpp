#include "FFTDevice.hpp"

#include <iostream>

#include <QDebug>
#include <QtEndian>

namespace qTuner{

// lack of Windows math functions
#ifdef _WIN32
double log2(double d)
{
   return log(d)/log(2.);
}
double round(double d)
{
   return floor(d + 0.5);
}
#endif // _WIN32

FFTDevice::FFTDevice(const QAudioFormat &aFormat, QObject *parent):
   QIODevice(parent),
   m_audioFormat(aFormat),
   m_iResolutionFactor(20),
   A440(440.0) // Hz
{
   m_iSampleBytes = m_audioFormat.sampleSize() / 8;
}

FFTDevice::~FFTDevice(){}

void FFTDevice::start() { open(QIODevice::WriteOnly); }
void FFTDevice::stop()  { close(); }

qint64 FFTDevice::readData(char *data, qint64 maxlen)
{
   Q_UNUSED(data)
   Q_UNUSED(maxlen)
   return 0;
}

qint64 FFTDevice::writeData(const char *data, qint64 len)
{
   m_iSamples = len / m_iSampleBytes;

   qint16* values = new qint16[m_iSamples];
   const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
   for (int i = 0; i < m_iSamples; i++) {
      values[i] = qFromLittleEndian<qint16>(ptr);
      ptr += m_iSampleBytes;
   }

   calcSpectrumSize();
   double* spectrum = new double[m_iSpectrumSize];
   fft(values, spectrum);
   //qDebug() << "raw";  dump(values, m_iSamples);
   //qDebug() << "spec"; dump(spectrum,m_iSamples);
   double f = frequencyAt(maxPosition(spectrum, m_iSamples));
   //qDebug() <<  semitone(f) << f;
   //qDebug() << f;
   updateNote(semitone(f));
   emit signalNoteUpdated(m_note);
   
   delete[] spectrum;
   delete[] values;
   return len;
}

void FFTDevice::fft(qint16 data[], double spectr[])
{

   // create complex values for fft
   // and fill with zeros for higher precision
   std::complex<double>* cData = new std::complex<double>[m_iSpectrumSize];
   for (int i=0; i<m_iSamples; i++)
      cData[i] = std::complex<double>((double)data[i],0.0);
   // add zeros
   for (int i=m_iSamples; i<m_iSpectrumSize; i++)
      cData[i] = std::complex<double>(0.0,0.0);
   
   fft(cData,m_iSpectrumSize);
   // put back only first n values
   for (int i=0; i<m_iSpectrumSize; i++)
      spectr[i] = std::abs<double>(cData[i]);

   delete[] cData;
}

void FFTDevice::fft(std::complex<double> data[], int n)
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

void FFTDevice::calcSpectrumSize()
{
   // make sure that m_iSpectrumSize = 2^N
   m_iSpectrumSize = m_iResolutionFactor*m_iSamples;
   int N  = log2(m_iSpectrumSize);
   m_iSpectrumSize = pow((double)2,(double)N+1);
}

void FFTDevice::dump(qint16 data[], int n)
{
   QString output;
   for (int i = 0; i < n; i++) {
      output += QString::number(data[i]) + " ";
   }
   qDebug() << output;
}

void FFTDevice::dump(double data[], int n)
{
   QString output;
   for (int i = 0; i < n; i++) {
      output += QString::number(data[i]) + " ";
   }
   qDebug() << output;
}

int FFTDevice::maxPosition(double data[], int n)
{
   int iMax = 0;
   for (int i=0; i<n; i++){
      if (data[i] > data[iMax])
         iMax = i;
   }
   return iMax;
}

double FFTDevice::frequencyAt(int pos)
{
   return pos * (double)m_audioFormat.sampleRate() / (double)m_iSpectrumSize;
}

void FFTDevice::setResolutionFactor(int res)
{
   if (res <= 1)
      res = 1;
   else if (res >= 20)
      res = 20;
   
   m_iResolutionFactor = res;
}

// relative to A440
double FFTDevice::semitone(double freq)
{
   // f = 2^(n/12) * 440Hz
   return 12.0 * log2(freq/A440);
}

void FFTDevice::updateNote(double semitone)
{
   // round
   int st = round(semitone);
   double remainder = semitone - st;
   // unsigned modulo with right rounding
   SemiToneSymbol sym = (SemiToneSymbol)((st % 12 + 12) % 12);
   m_note.setSymbol(sym);
   m_note.setRemainder(remainder);
}
   
} // end namespace qTuner
