#include "NoteInfo.hpp"

namespace qTuner{

NoteInfo::NoteInfo(){}
NoteInfo::~NoteInfo(){};


SemiToneSymbol NoteInfo::getSymbol()   { return m_symbol; }
double         NoteInfo::getRemainder(){ return m_remainder; }

QString NoteInfo::getSymbolString()
{
   QString symbol;
   switch (m_symbol){
      case A : symbol = "A" ; break;
      case Bb: symbol = "Bb"; break;
      case B : symbol = "B" ; break;
      case C : symbol = "C" ; break;
      case Db: symbol = "Db"; break;
      case D : symbol = "D" ; break;
      case Eb: symbol = "Eb"; break;
      case E : symbol = "E" ; break;
      case F : symbol = "F" ; break;
      case Gb: symbol = "Gb"; break;
      case G : symbol = "G" ; break;
      case Ab: symbol = "Ab"; break;
      default: symbol = "?" ; break;
   }
   return symbol;
}

void NoteInfo::setSymbol(SemiToneSymbol sym)
{
   m_symbol = sym;
}
void NoteInfo::setRemainder(double remainder)
{
   m_remainder = remainder;
}

} // end namespace qTuner
