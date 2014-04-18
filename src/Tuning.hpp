#ifndef TUNING_HPP_
#define TUNING_HPP_

#include "NoteInfo.hpp"

#include <QList>
#include <QString>

namespace qTuner
{
   class Tuning;
}

class qTuner::Tuning
{
public:
   Tuning(); // TODO allow?
   /*!
    * \param stringNotes String with semitones separated by comma
    */
   Tuning(QString stringNotes);
   virtual ~Tuning();
   
private:
   QString               m_sName;
   QString               m_sStringNotes;
   QList<SemiToneSymbol> m_lStringNotes;

   void extractFromString();
};

#endif // TUNING_HPP_ 
