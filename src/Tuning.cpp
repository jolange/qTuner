#include "Tuning.hpp"

#include <QStringList>
#include <QDebug>

namespace qTuner{

Tuning::Tuning():
   QList<SemiToneSymbol>()
{}
Tuning::Tuning(QString stringNotes):
   QList<SemiToneSymbol>(),
   m_sStringNotes(stringNotes)
{
   extractFromString();
}

Tuning::~Tuning(){}

void Tuning::extractFromString()
{
   // TODO catch errors here?
   clear();
   QStringList slNotesAndName =  m_sStringNotes.split(";");
   if (slNotesAndName[0].isEmpty()){
      m_sName = slNotesAndName[1];
      return;
   }
   m_sName = "[" + slNotesAndName[0] +"] " + slNotesAndName[1];
   QStringList slNotes = slNotesAndName[0].split(",");
   SemiToneSymbol st;
   for (int i=0; i<slNotes.size(); i++){
      st=NoteInfo::getSymbol(slNotes[i]);
      if (st == err){
         // TODO message handler?
         qWarning() << "Warning:" << slNotes[i] << "is not a Note. No Tuning applied.";
         clear();
         m_sName = "Error";
         break;
      }else{
         append(st);
      }
   }
}

SemiToneSymbol Tuning::at(int i) const
{
   return QList<SemiToneSymbol>::at(i);
}
int Tuning::count(const SemiToneSymbol st) const
{
   return QList<SemiToneSymbol>::count(st);
}

bool Tuning::isEmpty() const
{
   return QList<SemiToneSymbol>::isEmpty();
}

int Tuning::size() const
{
   return QList<SemiToneSymbol>::size();
}

QString Tuning::getName() const
{
   return m_sName;
}


} // end namespace qTuner
