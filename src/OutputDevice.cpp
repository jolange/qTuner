#include "OutputDevice.hpp"

#include <QDebug>
#include <QtEndian>

namespace qTuner{

OutputDevice::OutputDevice(const QAudioFormat &aFormat, QObject *parent):
   QIODevice(parent),
   m_audioFormat(aFormat)
{}

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
   QString output;
   const int sampleBytes = m_audioFormat.sampleSize() / 8;
   const int numSamples = len / sampleBytes;

   const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
   for (int i = 0; i < numSamples; ++i) {
      output += QString::number(qFromLittleEndian<qint16>(ptr)) + " ";
      ptr += sampleBytes;
   }
   qDebug() << output;

   return len;
}

} // end namespace qTuner
