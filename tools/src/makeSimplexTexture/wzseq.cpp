#include "wzseq.h"

eSequenceZ operator++(eSequenceZ &seq, int32_t)
{
  eSequenceZ oldSeq = seq;
  seq = static_cast<eSequenceZ>((static_cast<int32_t>(seq) + 1) % eiZlast);
  return oldSeq;
}

eSequenceZ operator--(eSequenceZ &seq, int32_t)
{
  eSequenceZ oldSeq = seq;
  seq = static_cast<eSequenceZ>((static_cast<int32_t>(seq) - 1 + static_cast<int32_t>(eiZlast)) % eiZlast);
  return oldSeq;
}

eSequenceW operator++(eSequenceW &seq, int32_t)
{
  eSequenceW oldSeq = seq;
  seq = static_cast<eSequenceW>((static_cast<int32_t>(seq) + 1) % eiWlast);
  return oldSeq;
}

eSequenceW operator--(eSequenceW &seq, int32_t)
{
  eSequenceW oldSeq = seq;
  seq = static_cast<eSequenceW>((static_cast<int32_t>(seq) - 1 + static_cast<int32_t>(eiZlast)) % eiWlast);
  return oldSeq;
}
