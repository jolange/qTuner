#include "NoteInfo.hpp"

#include "winMath.hpp"

#include <cmath>

namespace qTuner{

const double NoteInfo::A440=440.0; // 440.0 Hz

NoteInfo::NoteInfo():
   QObject(),
   m_frequency(0) // stays unset, if directly set from semitone
{}
NoteInfo::NoteInfo(const NoteInfo& other):
   QObject(),
   m_frequency(other.m_frequency),
   m_semitone(other.m_semitone),
   m_symbol(other.m_symbol),
   m_remainder(other.m_remainder)
{}
NoteInfo::~NoteInfo(){}

void NoteInfo::setFromSemitone(double st)
{
   // round
   int stR = round(st);
   m_remainder = st - stR;
   // unsigned modulo with right rounding
   m_semitone  = ((stR % 12 + 12) % 12);
   m_symbol    = (SemiToneSymbol)(int)m_semitone;
   m_semitone += m_remainder;

   // will be reset, if "setFromFrequency", otherwise it's 0
   m_frequency = 0;
}

void NoteInfo::setFromFrequency(double f)
{
   setFromSemitone(semitone(f));
   m_frequency = f;
}

void NoteInfo::setSymbol(SemiToneSymbol sym)
{
   m_symbol = sym;
   // some defaults
   m_remainder = 0;
   m_semitone  = 0.0;
   m_frequency = 0.0;
}

void NoteInfo::setRemainder(double remainder)
{
   m_remainder = remainder;
}


SemiToneSymbol NoteInfo::getSymbol()    const { return m_symbol; }
double         NoteInfo::getFrequency() const { return m_frequency; }
double         NoteInfo::getRemainder() const { return m_remainder; }
double         NoteInfo::getSemitone()  const { return m_semitone; }

// relative to A440
double NoteInfo::semitone(double freq)
{
   // f = 2^(n/12) * 440Hz
   return 12.0 * log2(freq/A440);
}

QString NoteInfo::getSymbolString() const 
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

QString NoteInfo::getSymbolString(SemiToneSymbol sym)
{
   QString symbol;
   switch (sym){
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

SemiToneSymbol NoteInfo::getSymbol(QString sym)
{
   if (sym=="A" ) return A;
   if (sym=="Bb") return Bb;
   if (sym=="B" ) return B;
   if (sym=="C" ) return C;
   if (sym=="Db") return Db;
   if (sym=="D" ) return D;
   if (sym=="Eb") return Eb;
   if (sym=="E" ) return E;
   if (sym=="F" ) return F;
   if (sym=="Gb") return Gb;
   if (sym=="G" ) return G;
   if (sym=="Ab") return Ab;
   return err;
}

} // end namespace qTuner
