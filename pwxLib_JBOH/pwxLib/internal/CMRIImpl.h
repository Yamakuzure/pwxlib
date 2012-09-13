#pragma once
#ifndef PWX_CMRIIMPL_H_INCLUDED
#define PWX_CMRIIMPL_H_INCLUDED 1

/** @internal
  * @file CMRIImpl.h
  *
  * @brief Implementations for CItem
  *
  * @see CMemRingFactory.h for documentation and history
  *
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
# error "Do not include CMRIImpl.h, include CMemRingFactory.h!!"
#endif // Check for main file


namespace pwx {
namespace mrf {
namespace base {

// Note: The namespaces are documented in CMemRingFactory.h

/** @brief Returns the id of the data.
**/
uint32_t CItem::getId () const
{
  return (id);
}

/** @brief Returns the name of the data.
**/
const char * CItem::getName () const
{
  return (name.c_str());
}

/** @brief Returns the number of the data.
**/
int32_t CItem::getNr () const
{
  return (nr);
}


/** @brief save the items core data in an ostream
  *
  * @param[in] os open ostream to write the data into
**/
void CItem::save (::std::ostream &os) const
{
  if (os.good())
    {
      os << ID_CITEM_START << ";";
      os << id << ";";
      os << (name.size() + 1) << ";" << name << ";";
    }
}

/** @brief Set all three core values
  *
  * Convenience method to set all three core values at once. This method
  * is meant to be used to set all three values by a container after
  * copying an item into its memory ring structure.
  *
  * @param[in] aNr the desired number
  * @param[in] aId the desired id
  * @param[in] aName the desired name. If set to NULL, the current name is erased.
**/
void CItem::setCoreValues (int32_t aNr, uint32_t aId, const char * aName)
{
  nr   = aNr;
  id   = aId;
  if (aName) name = aName;
  else       name = "";
}


/** @brief Sets a new id.
  *
  * @param[in] aId the desired id
**/
void CItem::setId (uint32_t aId)
{
  id = aId;
}


/** @brief Sets a new name.
  *
  * @param[in] aName the desired name. If set to NULL, the current name is erased.
**/
void CItem::setName (::std::string &aName)
{
  name = aName.size() ? aName : "";
}


/** @brief Sets a new name
  *
  * @param[in] aName the desired name. If set to NULL, the current name is erased.
**/
void CItem::setName (const char *aName)
{
  name = aName ? aName : "";
}


/** @brief assignment operator
  *
  * Assignement operator to copy the core data (nr, id and name) from a
  * @a source item. The copying container is responsible to adapt the copied
  * data to a usable state.
  *
  * @param[in] source the source data to copy from
 */
CItem& CItem::operator= (const CItem &source)
{
  if (&source != this)
    {
      nr   = source.getNr();
      id   = source.getId();
      name = source.getName();
    }
  return (*this);
}

} // End of namespace base

} // End of namespace mrf

} // End of namespace pwx


#endif // PWX_CMRIIMPL_H_INCLUDED
