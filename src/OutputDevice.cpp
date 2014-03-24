#include "OutputDevice.hpp"

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
   QString output;

   qint16* values = new qint16[m_iSamples];
   const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
   for (int i = 0; i < m_iSamples; ++i) {
      values[i] = qFromLittleEndian<qint16>(ptr);
      output += QString::number(values[i]) + " ";
      ptr += m_iSampleBytes;
   }
   qDebug() << output;

   fft(values,m_iSamples);
   delete[] values;
   return len;
}

void OutputDevice::fft(qint16 data[], int iSamples)
{
   Q_UNUSED(data)
   Q_UNUSED(iSamples)
   return;
}

} // end namespace qTuner
