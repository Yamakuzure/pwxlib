#pragma once
#ifndef PWX_LIBPWX_TYPES_TVARDELETER_H_INCLUDED
#define PWX_LIBPWX_TYPES_TVARDELETER_H_INCLUDED 1

/** @file TVarDeleter.h
  *
  * @brief Definition of a template to delegate pointer deletions to external functions
  *
  * (c) 2007 - 2012 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/

namespace pwx {


/** @class TVarDeleter
  *
  * @brief Delegation of pointer deletion to an external function
  *
  * Types like std::shared_ptr offer the possibility to set an
  * individual deletion method. This deleter must handle the
  * deletion when operator()(Tp) is called. Further neither the
  * copy constructor nor the destructor of the deleter must throw.
  *
  * This template can be used as such a deleter. Its constructor
  * takes an optional parameter to an external function of the form
  * void (*destroy_)(data_t* data_). This function is called, if set,
  * when the operator() is called. Otherwise the operator invokes
  * the regular delete operator.
  *
  * Please note that all exceptions are caught and thrown out of
  * the window. So please make sure you provide a function that
  * is safe.
**/
template<typename data_t>
class TVarDeleter
{
public:
  /* ===============================================
   * === Public Constructors and destructors     ===
   * ===============================================
  */
  /** @brief default constructor
    *
    * The default constructor sets a destroy method that is used by operator()
    * to delete the given data.
    *
    * @param destroy_[in] A pointer to a function that is to be used to destroy the data
  **/
  TVarDeleter(void (*destroy_)(data_t* data_)) noexcept
  : destroy(destroy_)
    { }

  /** @brief empty constructor
    *
    * The empty constructor sets the destroy method to nullptr. The operator()
    * then uses the default delete operator to destroy the given data.
  **/
  TVarDeleter() noexcept
  : destroy(nullptr)
    { }

  /** @brief copy constructor
    *
    * This simply copies the destroy method.
  **/
  TVarDeleter(const TVarDeleter &src) noexcept
  : destroy(src.destroy)
    { }

  /// @brief destructor
  ~TVarDeleter() noexcept
    { }

  /* ===============================================
   * === Public operators                        ===
   * ===============================================
  */
  void operator()(data_t* data) noexcept
    {
      if (data)
        {
          if (destroy)
            {
              PWX_TRY(destroy(data))
              catch(...) { }
            }
          else
            {
              PWX_TRY(delete data)
              catch(...) { }
            }
        }
    }

private:
  /* ===============================================
   * === Private methods                         ===
   * ===============================================
  */
  void (*destroy)(data_t* data_);
}; // class TVarDeleter


} // namespace pwx
#endif // PWX_LIBPWX_TYPES_TVARDELETER_H_INCLUDED

