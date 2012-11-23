#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "pwx/types/CException.h"
#include "pwx/general/macros.h"
#include <cstring>

namespace pwx {

/** @brief explicit constructor
  *
  * This constructor is to be used to create a pwx::CException. All relevant data
  * is set exactly once with this constructor and must not be changed apart from
  * the trace.
  *
  * You can use the macro PWX_THROW() from pwx/general/macros.h to throw this
  * kind of exceptions in a very convenient way. It'll fill in the positional
  * information for you. The macro PWX_THROW_STD_FURTHER from the same file
  * can be used to convert an std::exception into a pwx::CException with tracing
  * information.
  *
  * @param[in] name_ The name of the exception.
  * @param[in] what_ The text that is to be returned by the what() method.
  * @param[in] where_ The positional information where it was thrown.
  * @param[in] func_ Name of the function where this was thrown.
  * @param[in] desc_ Description of what wnet wrong.
  */
CException::CException (const char* const name_, const char* const what_, const char* const where_,
						const char* const func_, const char* const desc_) noexcept
	: std::exception(),
	  txtName  (strdup (name_)),
	  txtWhat  (strdup (what_)),
	  txtWhere (strdup (where_)),
	  txtFunc  (strdup (func_)),
	  txtDesc  (strdup (desc_)),
	  txtTrace ("Thrown from : ")
{
	PWX_TRY (txtTrace += where_)
	PWX_CATCH_AND_FORGET (std::exception)
}


/** @brief copy constructor
  *
  * @param[in] src the exception to copy
  */
CException::CException (const CException& src) noexcept
	: std::exception (src),
	  txtName  (strdup (src.txtName)),
	  txtWhat  (strdup (src.txtWhat)),
	  txtWhere (strdup (src.txtWhere)),
	  txtFunc  (strdup (src.txtFunc)),
	  txtDesc  (strdup (src.txtDesc)),
	  txtTrace (src.txtTrace)
{
	/* nothing to be done here */
}


/** @brief addToTrace
  *
  * This function adds a line break and then @a trace_ to
  * the exception trace. Exceptions from string operations
  * are ignored. It is better to have a broken trace than
  * a broken unwinding.
  */
void CException::addToTrace (const char* trace_) noexcept
{
	if (txtTrace.empty()) {
		PWX_TRY (txtTrace  = "Thrown from : "; txtTrace += txtWhere)
		PWX_CATCH_AND_FORGET (std::exception)
	}
	PWX_TRY (txtTrace += "\n"; txtTrace += trace_;)
	PWX_CATCH_AND_FORGET (std::exception)
}


/** @brief destructor
  *
  * As all C-Strings are malloc'd they have to be freed.
  */
CException::~CException() noexcept
{
	if (txtName)  delete const_cast<char*> (txtName);
	if (txtWhat)  delete const_cast<char*> (txtWhat);
	if (txtWhere) delete const_cast<char*> (txtWhere);
	if (txtFunc)  delete const_cast<char*> (txtFunc);
	if (txtDesc)  delete const_cast<char*> (txtDesc);
}


/// @brief Return the name of the exception
const char* CException::name() const noexcept
{
	return txtName;
}


/// @brief Return the exception reason
const char* CException::what() const noexcept
{
	return txtWhat;
}


/// @brief Return the original throwing position
const char* CException::where() const noexcept
{
	return txtWhere;
}


/// @brief Return the exception description
const char* CException::desc() const noexcept
{
	return txtDesc;
}


/// @brief Return the pretty function where the original throw occurred.
const char* CException::pfunc() const noexcept
{
	return txtFunc;
}


/// @brief Return the trace of the exceptions path
const char* CException::trace() const noexcept
{
	return txtTrace.c_str();
}

} // namespace pwx
