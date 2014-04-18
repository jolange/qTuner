#include "Tuning.hpp"

#include <QStringList>
#include <QDebug>

namespace qTuner{

Tuning::Tuning(){}
Tuning::Tuning(QString stringNotes):
   m_sStringNotes(stringNotes)
{
   extractFromString();
}

Tuning::~Tuning(){}

void Tuning::extractFromString()
{
   m_lStringNotes.clear();
   QStringList slNotes= m_sStringNotes.split(",");
   SemiToneSymbol st;
   for (int i=0; i<slNotes.size(); i++){
      st=NoteInfo::getSymbol(slNotes[i]);
      if (st == err){
         // TODO message handler?
         qWarning() << "Warning:" << slNotes[i] << "is not a Note. No Tuning applied.";
         m_lStringNotes.clear();
         break;
      }else{
         m_lStringNotes.push_back(st);
      }
   }
}

} // end namespace qTuner
