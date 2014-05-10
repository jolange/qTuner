#ifndef NOTEINFO_HPP_
#define NOTEINFO_HPP_

#include <QObject>

namespace qTuner
{
   class NoteInfo;
   enum  SemiToneSymbol {A=0, Bb=1, B=2, C=3, Db=4, D=5, Eb=6,
                         E=7, F=8, Gb=9, G=10, Ab=11, err=12};
}

class qTuner::NoteInfo : public QObject
{
Q_OBJECT

public:
   static const double A440; // 440.0 Hz

   NoteInfo();
   NoteInfo(const NoteInfo& other);
   virtual ~NoteInfo();

   static double         semitone(double freq);
   static QString        getSymbolString(SemiToneSymbol sym);
   static SemiToneSymbol getSymbol(QString sym);
   
   SemiToneSymbol getSymbol()       const;
   QString        getSymbolString() const;
   double         getFrequency()    const;
   double         getRemainder()    const;
   double         getSemitone()     const; // relative to A440

   void setFromSemitone(double st); // relative to A440
   void setFromFrequency(double f); // relative to A440
   void setSymbol(SemiToneSymbol sym);
   void setRemainder(double remainder);

private:
   double         m_frequency; // 0 if not set from frequency
   double         m_semitone;
   SemiToneSymbol m_symbol;
   double         m_remainder;
};

#endif // NOTEINFO_HPP_
