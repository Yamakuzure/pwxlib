#pragma once
#ifndef PWX_CSCTIMPL_H_INCLUDED
#define PWX_CSCTIMPL_H_INCLUDED 1

/** @internal
  * @file CSCTImpl.h
  *
  * @brief Implementations for CSinCosTables
  *
  * @see CSinCosTables.h for documentsation and history
  *
**/

#ifndef PWX_SINCOSTABLES_H_INCLUDED
# error "Do not include CSCTImpl.h, include CSinCosTable.h!"
#endif // Check for main file

/** @brief return cosine
  *
  * This method returns the cosine of @a degree as a double.
  * The return value defaults to double, because allowing T the given
  * type of @a degree to influent the return value would result in
  * a useless result if you use an integer type for @a degree.
  *
  * This is a template, so please make sure you use it with a sensible
  * type. It won't get checked!
  *
  * @param[in]  degree The degree the cosine is wanted for
  * @return The cosine to degree as a double.
**/
template<typename T> double CSinCosTables::cos(const T degree) const
{
  if (currPrec > 0)
    return (CosTable[SCTHelpers::getNormalizedDegree(degree, currPrec)]);
  else
    return (::std::cos(SCTHelpers::degToRad(degree)));
}


/** @brief return sine
  *
  * This method returns the sine of @a degree as a double.
  * The return value defaults to double, because allowing T the given
  * type of @a degree to influent the return value would result in
  * a useless result if you use an integer type for @a degree.
  *
  * This is a template, so please make sure you use it with a sensible
  * type. It won't get checked!
  *
  * @param[in]  degree The degree the sine is wanted for
  * @return The sine to degree as a double.
**/
template<typename T> inline double CSinCosTables::sin (const T degree) const
{
  if (currPrec > 0)
    return (SinTable[SCTHelpers::getNormalizedDegree(degree, currPrec)]);
  else
    return (::std::sin(SCTHelpers::degToRad(degree)));
}


/** @brief get sine an cosine at once.
  *
  * This is a method that sets @a aSin and @a aCos to the sine and cosine of
  * the submitted @a degree. All have the same types.
  *
  * This is a template, so please make sure you use it with a sensible
  * type. It won't get checked!
  *
  * @param[in]  degree The degree to calculate with
  * @param[out] aSin Will be set to the corresponding sine
  * @param[out] aCos Will be set to the corresponding cosine
**/
template<typename T> void CSinCosTables::sincos(const T degree, T &aSin, T &aCos) const
{
  aCos = static_cast<T>(this->cos(degree));
  aSin = static_cast<T>(this->sin(degree));
}


/** @brief resetPrecision
  *
  * This method (tries) to reset the precision used to the initial value
  * set by PWX_SCT_INITIALPRECISION.
  *
  * @return the precision finally used, as it could be different due to memory shortage
  *
**/
int32_t CSinCosTables::resetPrecision()
{
  if (initPrec != currPrec)
    return(this->setPrecision(initPrec));
  else
    return(currPrec);
}


/** @brief setPrecision
  *
  * Tries to set the precision to @a precision and returns the outcome of this try
  *
  * @param[in] precision the desired precision
  * @return the precision finally used, which might differ (be lower) if memory shortage occurs.
  *
**/
int32_t CSinCosTables::setPrecision(const int32_t precision)
{
  if (precision != currPrec)
    {
      // There are two possibilities:
      // a) we switch to 0, aka live calculation
      // b) we need to (re)initialize the tables
      if (-1 == precision)
        {
          // This is case a, so we just switch:
          lastPrec = currPrec;
          currPrec = precision;
        }
      else
        {
          // Now we have two other choices:
          // b 1) precision equals lastPrec and currPrec is currently -1 (And both tables present!)
          // b 2) otherwise there is work to be done!
          if ((precision == lastPrec) && (-1 == currPrec) && CosTable && SinTable)
            // Just switch back:
            currPrec = lastPrec;
          else
            {
              // Nah... the important stuff. Hope you know what you are doing!
              if (withOutput)
                {
                  ::std::cout << "Re-Initializing from precision " << currPrec;
                  ::std::cout << " to precision " << precision << ":" << ::std::endl;
                }
              lastPrec = currPrec;
              currPrec = precision;
              bool bIsFinished = false;

              while ((currPrec > -1) && !bIsFinished)
                {
                  tableSize = static_cast<int32_t>(pwx_pow(10, currPrec)) * 360;
                  double * temp = (double *)realloc(CosTable, tableSize * sizeof(double));

                  if (!temp)
                    {
                      if (withOutput)
                        {
                          // But notify user if wanted:
                          ::std::cerr << "Failed to allocate " << (tableSize * sizeof(double));
                          ::std::cerr << " bytes for the cosine table !" << ::std::endl;
                          ::std::cerr << " --> Reducing precision to " << (currPrec - 1) << " !" << ::std::endl;
                        }
                      --currPrec;
                    }
                  else
                    {
                      CosTable = temp;
                      temp = (double *)realloc(SinTable, tableSize * sizeof(double));
                      if (!temp)
                        {
                          if (withOutput)
                            {
                              // But notify user if wanted:
                              ::std::cerr << "Failed to allocate " << (tableSize * sizeof(double));
                              ::std::cerr << " bytes for the sine table !" << ::std::endl;
                              ::std::cerr << " --> Reducing precision to " << (currPrec - 1) << " !" << ::std::endl;
                            }
                          --currPrec;
                        }
                      else
                        {
                          SinTable    = temp;
                          temp        = NULL;
                          bIsFinished = true;
                        }
                    }
                } // End reallocation loop

              // Now we have to check, whether there is anything done:
              if (bIsFinished && (currPrec > -1) && CosTable && SinTable)
                {
                  // Yes. We can recalculate :
                  double  dPrec   = static_cast<double>(pwx_pow(10, currPrec));
                  int32_t i       = 0;
                  double  radiant = 0.0L;

                  while ( i < tableSize )
                    {
                      radiant  = SCTHelpers::degToRad(static_cast<double>(i) / dPrec);
                      SinTable[i]  =  ::std::sin ( radiant );
                      CosTable[i]  =  ::std::cos ( radiant );
                      i++;
                      if ( withOutput && ( i % ( tableSize / 10 ) == 0 ))
                        {
                          ::std::cout << " " << (i * 100 / tableSize) << "%";
                          ::std::cout.flush();
                        }
                    }
                  if (withOutput)
                    {
                      ::std::cout << ::std::endl << "Done with precision " << currPrec;
                      int32_t size	  =	sizeof(double) * 2 * tableSize;
                      double  mibSize	=	static_cast<double>(size) / 1024. / 1024.;
                      ::std::cout << ", size of the Tables: " << size << " bytes (";
                      ::std::cout << ::pwx::StreamHelpers::adjRight(0, 2) << mibSize << " MiB)" << ::std::endl << ::std::endl;
                    }
                } // End recalculation
              else
                {
                  // Huh? Now we have a problem. And currPrec needs to be zero:
                  if (withOutput)
                    ::std::cout << ::std::endl << "Failed! Changed to " << currPrec << ::std::endl;
                  assert (-1 == currPrec);
                  if (CosTable) free (CosTable);
                  if (SinTable) free (SinTable);
                  CosTable = NULL;
                  SinTable = NULL;
                  lastPrec = -1; // Just to be sure, as we can't switch back!
                  tableSize= 0;
                }
            } // End choice b 2
        } // End choice b
    } // End work (precision doesn't equal currPrec)

  return (currPrec);
}

#endif // PWX_CSCTIMPL_H_INCLUDED
