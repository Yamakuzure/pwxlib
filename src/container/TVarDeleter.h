#ifndef PWX_LIBPWX_TYPES_TVARDELETER_H_INCLUDED
#define PWX_LIBPWX_TYPES_TVARDELETER_H_INCLUDED 1
#pragma once

/** @file TVarDeleter.h
  *
  * @brief Definition of a template to delegate pointer deletions to external functions
  *
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * History and change log are maintained in pwxlib.h
**/


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"


/// @namespace pwx
namespace pwx {


/** @class TVarDeleter
  *
  * @brief Delegation of pointer deletion to an external function
  *
  * Types like std::shared_ptr offer the possibility to set an
  * individual deletion method. This deleter must handle the
  * deletion when `operator()(Tp)` is called. Further neither the
  * copy constructor nor the destructor of the deleter must throw.
  *
  * This template can be used as such a deleter. Its constructor
  * takes an optional parameter to an external function of the form
  * `void (*destroy_)(data_t* data_)`. This function is called, if set,
  * when the `operator()` is called. Otherwise the operator invokes
  * the regular delete operator.
  *
  * Please note that all exceptions are caught and thrown out of
  * the window. So please make sure you provide a function that
  * is safe.
**/
template<typename data_t>
class PWX_API TVarDeleter {
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor sets a destroy method that is used by `operator()`
	  * to delete the given data.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TVarDeleter ( void ( *destroy_ ) ( data_t* data_ ) ) noexcept
		: destroy ( destroy_ )
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor sets the destroy method to `nullptr`. The `operator()`
	  * then uses the default delete operator to destroy the given data.
	**/
	TVarDeleter() noexcept
		: destroy ( nullptr )
	{ }


	/** @brief copy constructor
	  *
	  * This simply copies the destroy method.
	**/
	TVarDeleter ( TVarDeleter const& src ) noexcept
		: destroy ( src.destroy )
	{ }


	/// @brief destructor
	~TVarDeleter() noexcept
	{ }


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief function operator
	  *
	  * this operator allows an instance to be used like a function.
	  * If @a data is not the `nullptr` and a destroy method was set,
	  * the destroy method will be used on @a data. With @a data but
	  * without a destroy method, the regular delete operator is
	  * used on @a data.
	  *
	  * All exceptions are thrown away, so please provide safe
	  * pointers and destroy methods only!
	  *
	  * @param[in] data pointer to the data to destroy.
	**/
	void operator() ( data_t* data ) noexcept {
		if ( data ) {
			if ( destroy ) {
				PWX_TRY ( destroy ( data ) )
				catch ( ... ) { }
			} else {
				PWX_TRY ( delete data )
				catch ( ... ) { }
			}
		}
	}


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	void ( *destroy ) ( data_t* data_ ); //!< Pointer to the provided destroy method
}; // class TVarDeleter


} // namespace pwx

#endif // PWX_LIBPWX_TYPES_TVARDELETER_H_INCLUDED

