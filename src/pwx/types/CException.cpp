#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include <pwx/types/CException.h>
#include <pwx/general/macros.h>
#include <cstring>

namespace pwx {

/** @brief CException
  *
  * @todo: document this function
  */
CException::CException(const char* const name_, const char* const what_, const char* const where_,
                       const char* const func_, const char* const desc_) noexcept
  : ::std::exception(), eName(strdup(name_)), txtWhat(strdup(what_)),
    txtWhere(strdup(where_)), txtFunc(strdup(func_)), txtDesc(strdup(desc_)),
    txtTrace("Thrown from : ")
{
  PWX_TRY(txtTrace += where_)
  PWX_CATCH_AND_FORGET(::std::exception)
}

/** @brief CException
  *
  * @todo: document this function
  */
CException::CException(const CException& e) noexcept
  : ::std::exception(e), eName(strdup(e.eName)), txtWhat(strdup(e.txtWhat)),
    txtWhere(strdup(e.txtWhere)), txtFunc(strdup(e.txtFunc)),
    txtDesc(strdup(e.txtDesc)), txtTrace(e.txtTrace)
{ /* nothing to be done here */ }

/** @brief addToTrace
  *
  * @todo: document this function
  */
void CException::addToTrace(const char* trace_) noexcept
{
 if (txtTrace.empty())
   {
     PWX_TRY(txtTrace  = "Thrown from : "; txtTrace += txtWhere)
     PWX_CATCH_AND_FORGET(::std::exception)
   }
  PWX_TRY(txtTrace += "\n"; txtTrace += trace_;)
  PWX_CATCH_AND_FORGET(::std::exception)
}

/** @brief ~CException
  *
  * @todo: document this function
  */
CException::~CException() noexcept
{
  if (eName)    delete const_cast<char*>(eName);
  if (txtWhat)  delete const_cast<char*>(txtWhat);
  if (txtWhere) delete const_cast<char*>(txtWhere);
  if (txtFunc)  delete const_cast<char*>(txtFunc);
  if (txtDesc)  delete const_cast<char*>(txtDesc);
}


} // namespace pwx
