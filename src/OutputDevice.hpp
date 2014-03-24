#ifndef OUTPUTDEVICE_HPP_
#define OUTPUTDEVICE_HPP_

#include <QAudioInput>

class OutputDevice : public QIODevice
{
Q_OBJECT
public:
   OutputDevice(const QAudioFormat &aFormat, QObject *parent);
   ~OutputDevice();

   void start();
   void stop();

   qint64 readData(char *data, qint64 maxlen);
   qint64 writeData(const char *data, qint64 len);

private:
   const QAudioFormat m_audioFormat;

};

#endif /* OUTPUTDEVICE_HPP_ */ 
