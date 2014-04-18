#ifndef TUNING_HPP_
#define TUNING_HPP_

#include "NoteInfo.hpp"

#include <QList>
#include <QString>

namespace qTuner
{
   class Tuning;
}

class qTuner::Tuning : private QList<SemiToneSymbol>
{
public:
   Tuning(); // TODO allow?
   /*!
    * \param stringNotes String with semitones separated by comma
    */
   Tuning(QString stringNotes);
   virtual ~Tuning();

   SemiToneSymbol at(int i) const;
   bool           isEmpty() const;
   int            size()    const;

   QString getName() const;

private:
   QString               m_sName;
   QString               m_sStringNotes;

   void extractFromString();
};

#endif // TUNING_HPP_ 
