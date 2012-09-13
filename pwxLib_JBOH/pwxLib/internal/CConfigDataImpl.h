#pragma once
#ifndef PWX_CCONFIGDATAIMPL_H_INCLUDED
#define PWX_CCONFIGDATAIMPL_H_INCLUDED

/** @internal
  * @file CConfigDataImpl.h
  *
  * @brief This file defines the methods of CConfigData used by CCnfFileHandler
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
  * @author Sven Eden, PrydeWorX - Hamburg, Germany
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
  * History and Changelog maintained in CCfhFileHandler.h.
**/

#ifndef PWX_CCNFFILEHANDLER_H_INCLUDED
# error "Do not include CConfigDataImpl.h, include CCnfFileHandler.h!"
#endif // Check for main file

namespace pwx {
namespace cfh {
// note: The namespaces are documented in CCnfFileHandler.h!

/* =========================================== */
/* === Implementations for CConfigData     === */
/* =========================================== */

/** @brief Add data and optional comment to the existing set
  *
  * @param[in] aData the data string to separate and add
  * @param[in] aComment the comment to add
  * @return the number of data items present in this key if @a aData is set, the length of the comment otherwise
**/
int32_t CConfigData::add(const char *aData, const char *aComment)
  {
    using ::pwx::StreamHelpers::tabToSpace;
    using ::pwx::StreamHelpers::trim;

    // First the comment, as it is easy:
    if (aComment && strlen(aComment))
      {
        ::std::string newComment = aComment;
        trim(newComment, getCommentSep());
        if (comment.size())
          comment += " ";
        comment += newComment;
      }

    // Now handle the data:
    if (aData && strlen(aData))
      {
        dataStrBuilt = false; // The old one is no longer valid

        // We have something to split, so go ahead:
        ::std::string line = aData;

        // First step: Tabs-to-Spaces:
        tabToSpace(line);

        // Second step: remove line limiter \" and \' if any
        trim(line, '"');
        trim(line, '\'');

        // Third step: Set data separator
        char dataSep = getDataSep();

        // Fourth step: Split the line, unless it is a single char:
        // Note: A single char that can be taken as a separator must not be erased!
        if (line.size() > 1)
          {
            size_t curPos  = line.find_first_not_of(dataSep);
            size_t nextPos = line.find_first_of(dataSep, curPos);

            while (curPos != line.npos)
              {
                data.push_back(line.substr(curPos, nextPos - curPos));
                curPos = nextPos;
                if (nextPos != line.npos)
                  {
                    curPos  = line.find_first_not_of(dataSep, nextPos);
                    if (curPos != line.npos)
                      nextPos = line.find_first_of(dataSep, curPos);
                    else
                      nextPos = curPos;
                  }
              }
            if ((dataFlags & cfSort) && data.size())
              data.sort();
          }
        else if (line.size())
          // Just a simple push back
          data.push_back(line);

      } // End of having data

    return aData ? data.size() : comment.size();
  }


/** @brief return the full data string
  *
  * @return the full data string with separated items
**/
const char *CConfigData::getData () const
  {
    if (!dataStrBuilt)
      buildDataStr();
    return dataStr.c_str();
  }


/** @brief return the data item nr @a nr as a C-String
  *
  * This method returns the data item with the number @a nr if
  * it exists, otherwise it returns an empty string.
  *
  * The first data item has the number 0, the last has the alternative
  * number -1 allowing a backwards count.
  *
  * @param[in] nr The number of the data item, the first item is item nr 0
  * @return the data item as a C-String or NULL if @a nr is out of range.
**/
const char * CConfigData::getEntry(int32_t nr) const
  {
    int32_t dataCnt = static_cast<int32_t>(data.size());

    if ((nr >= 0) && (nr < dataCnt))
      {
        // Count forwards
        ::std::list< ::std::string >::const_iterator iter = data.begin();
        while (nr--)
          ++iter;
        return (*iter).c_str();
      }
    else if ((nr < 0) && (abs(nr) <= dataCnt))
      {
        // Count backwards
        ::std::list< ::std::string >::const_iterator iter = data.end();
        while (nr++)
          --iter;
        return (*iter).c_str();
      }
    return "";
  }


/** @brief write this data onto an outstream
  *
  * This method writes the saved key-data-pair and comment
  * to the given ostream @a os.
  *
  * If the writing failed, a pwx::cfh::writeFailed exception is thrown.
  *
  * @param[in,out] os An ostream to write into.
  * @param[in] key The name of this key, data items do not know their "key name" otherwise.
  * @param[in] maxLineLength The maximum length of each line before a line break occurs.
**/
void CConfigData::save (::std::ostream &os, const char *key, uint32_t maxLineLength)
  {
    /* The workflow for saving data is the following:
     * I   If there is data, then the key + separator and the data is saved
     * II  If there is a comment, it is saved thereafter completing a begun line if any
     * III The final endl is saved last, to allow regular empty lines.
     */
    ::std::string line     = "";
    int32_t       startLen = strlen(key) + 2 ; // One for separator, one for quotation mark
    int32_t       dataCnt  = data.size();
    bool          newLine  = true;
    bool          hasOut   = false; // Set to true if anything is written to the file (and be it a blank line!)
    bool          doEncase = !(dataFlags & cfNoEncase);
    bool          doWrap   = !(dataFlags & cfNoWrap);
    bool          isSpecial= (strstr(key, CFC_BlankLine)
                            || strstr(key, CFC_PureComment)
                            || strstr(key, CFC_SourceEntry)) ? true : false;
    char          commSep  = getCommentSep();
    char          dataSep  = getDataSep();
    char          keySep   = getKeySep();

    // We save lines which are marked as pure comments, empty, sources or have data for their key:
    if (dataCnt || (dataFlags & cfWriteEmpty) || isSpecial)
      {
        hasOut = true;

        if (!isSpecial)
          {
            // Of course only regular keys are saved like this
            os << key << keySep;
            // Only add quotation marks if we are allowed to do so
            if (doEncase)
              line += "\"";
          }

        // If it is a source entry, we have to print the command first:
        if (strstr(key, CFC_SourceEntry))
          os << "source ";

        // First we add components into lines to write and push them onto the stream:
        for (int32_t nr = 0; (nr < dataCnt) && os.good() ; ++nr)
          {
            // Start new lines with spaces for a better readability by human eyes
            if (newLine)
              {
                os << line << " \\" << ::std::endl;
                line.assign(startLen, ' ');
                newLine = false;
              }

            // Add this item prior further testing:
            line += getEntry(nr);

            // Now if we await more items, we take a look whether to split or not:
            if ((nr + 1) < dataCnt)
              {
                line += dataSep;
                if ( doWrap // Having this first means no calculation if we never wrap lines.
                  && ((line.size() + strlen(getEntry(nr + 1 ))) > maxLineLength) )
                  newLine = true;
              }
            // Otherwise we have to close with a quotation mark if we are allowed to do so
            else if (doEncase && !isSpecial)
              line += "\"";
          } // end of looping through the items data
      } // End of having data to write

    // Second we write the comments (if any) and use the possibly filled line to continue with
    size_t        cSize   = comment.size();
    size_t        pos     = 0;

    // Add the delimiter of the comment:
    if (cSize && !newLine)
      {
        // If we have enough space left, we add the delimiters:
        size_t second = comment.find_first_of(' ', 7); // That's " # word" at least

        if (!isSpecial && ((second == comment.npos) || ((line.size() + second) > maxLineLength)))
          // No, this wouldn't do
          newLine = true;
        else
          {
            // As we can add a comment to the current line, it has to be started with a separator
            if (line.size())
              line += " ";
            line += commSep;
            if (comment[0] != commSep)
              line += " ";
          }
      }

    while (os.good() && cSize && (!pos || (pos < cSize)))
      {
        size_t next     = cSize;
        size_t newNext  = cSize;

        hasOut = true;

        // If we need a new line, end this and start with a commenter:
        if (newLine)
          {
            os << line << ::std::endl;
            line  = commSep;
            line += " ";
            newLine = false;
          }

        size_t target   = doWrap ? maxLineLength - line.size() : ::std::numeric_limits<int32_t>::max(); // The space we have left
        bool   needLess = (next - pos) > target ? true : false;

        // Now if the remaining comment is too large, find the last space that renders the substring suitable
        while (needLess)
          {
            // We want to fetch too many characters, so reduce:
            // First skip blank spaces where we currently are:
            while (comment[newNext] == ' ')
              --newNext;
            // Now find the next blank space:
            newNext = comment.find_last_of(' ', newNext);

            // Are we finished?
            if ((pos == newNext) || ((newNext - pos) <= target))
              needLess = false;

            if (next > pos)
              // If next is pos, there would be no substring left
              next = newNext;

          }

        // newNext now could have pointed to a blank space, which has to be skipped:
        while ((comment[next] == ' ') && (next > pos))
          // Note: If there are a lot of blank spaces, we must not reduce the substring to zero
          //       or it becomes an endless loop to try to save it.
          --next;

        // Add what we've got to the line
        line += comment.substr(pos, next - pos + 1);
        pos = next;

        // If we await more data, we need a new line:
        if (pos < cSize)
          {
            newLine = true;
            // pos now points to a char before the next blank space:
            pos = comment.find_first_not_of(' ', ++pos);
          }
      } // End of going through the comment

    // Now the final line and endl can be written out:
    if (os.good() && hasOut)
      os << line << ::std::endl;
  }


/** @brief Set data and optional comment to the given values
  *
  * Note: To replace data or comment only, the other simply has to be set to NULL.
  *
  * @param[in] aData the data string to separate and set
  * @param[in] aComment the comment to set
  * @return The number of data items present for this key or the length of the comment if @a aData is NULL
**/
int32_t CConfigData::set(const char *aData, const char *aComment)
  {
    dataStrBuilt = false; // The old one is no longer valid

    // Simply clear what we have and use add():
    if (aData)
      data.clear();
    if (aComment)
      comment.clear();
    return add(aData, aComment);
  }


/** @brief sort the data items alphanumerical
  * Note: This is done by hand, because std::list::sort() all of a sudden
  * fails to sort std::string data. No Idea why, though, but to be save
  * it is done by hand now.
**/
void CConfigData::sort()
  {
    size_t dSize = data.size();

    if (dSize > 1)
      {
        bool isSorted = false;

        while (!isSorted)
          {
            dataIter curr = data.begin();
            dataIter prev = curr++; // First assign, then move forward (order matters!)
            isSorted = true;
            while (curr != data.end())
              {
                if (*prev > *curr)
                  {
                    dataStrBuilt = false;
                    isSorted     = false;
                    (*prev).swap(*curr);
                  }
                prev = curr++;
              }
          } // End of while is not sorted
      } // End of size > 2
  }

/** @brief make all data items unique
  *
  * This method first sorts the data items and then erases doublettes.
  *
  * Please note that this method sorts the data whether it is created
  * as being a sorting config file or not.
  *
  * @return The number of data items this key holds after sorting and filtering.
**/
int32_t CConfigData::unique()
  {
    if (data.size())
      {
        dataStrBuilt = false; // The old one is no longer valid

        data.sort();

        dataIter curr = data.begin();
        dataIter prev = curr++;

        while (curr != data.end())
          {
            if (*prev == *curr)
              data.erase(prev, curr);
            prev = curr++;
          }
      }

    return data.size();
  }

} // End of namespace cfh
} // End of namespace pwx

#endif // PWX_CCONFIGDATAIMPL_H_INCLUDED


