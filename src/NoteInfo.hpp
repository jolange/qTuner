#ifndef NOTEINFO_HPP_
#define NOTEINFO_HPP_

#include <QObject>

namespace qTuner
{
   class NoteInfo;
   enum  SemiToneSymbol {A=0, Bb=1, B=2, C=3, Db=4, D=5, Eb=6,
                         E=7, F=8, Gb=9, G=10, Ab=11};
}

class qTuner::NoteInfo : public QObject
{
Q_OBJECT

public:
   NoteInfo();
   virtual ~NoteInfo();
   
   SemiToneSymbol getSymbol();
   QString        getSymbolString();
   double         getRemainder();

   void setSymbol(SemiToneSymbol sym);
   void setRemainder(double remainder);

private:
   SemiToneSymbol m_symbol;
   double         m_remainder;
};

#endif // NOTEINFO_HPP_
