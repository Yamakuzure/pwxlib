#ifndef PWX_MAKESIMPLEXTEXTURE_TEXTBOX_H_INCLUDED
#define PWX_MAKESIMPLEXTEXTURE_TEXTBOX_H_INCLUDED 1
#pragma once

struct ENVIRONMENT; // Forward declaration

#include "main.h"

// Font settings:
#if defined(_WIN32) || defined(_WIN64)
#  if !defined(FONT_PATH)
#    define FONT_PATH "C:\\WINDOWS\\Fonts"
#  endif // font path
#  if !defined(FONT_NAME)
#    define FONT_NAME "COUR.TTF"
#  endif // font
#  define FONT_SEP "\\"
#else
#  if !defined(FONT_PATH)
#    define FONT_PATH "/usr/share/fonts/freefont-ttf"
#  endif // font path
#  if !defined(FONT_NAME)
#    define FONT_NAME "FreeMono.ttf"
#  endif // font
#  define FONT_SEP "/"
#endif // Windows versus linux

// We need access to sfmlui:
#include "sfmlui.h"

/** @brief CTextBox
  *
  * A little base class for text boxes
  *
**/
class CTextBox: private pwx::Uncopyable
{
public:
  /** Default constructor **/
  explicit CTextBox();
  /** Default destructor **/
  ~CTextBox();
  /** Initializing method : **/
  virtual int32_t initialize(ENVIRONMENT * env) PWX_VIRTUAL_PURE;

  /** struct-simulating public members **/
  sf::Color     bgColor;    //!< Background color of the text
  sf::Color     txtColor;   //!< Foreground color of the text
  sf::Image   * txtBox;     //!< The background box to draw upon
  int32_t       txtSize;    //!< Number of lines generated
  sf::String  * txtContent; //!< dynamic array of the text lines

private:
  /* --- no copying allowed --- */
  CTextBox(CTextBox&);
  CTextBox &operator=(CTextBox&);
};

/** @brief CHelpText
  *
  * A little simple class that holds the help text for on screen display.
  * Instanciated by showOSHelp only, thus never if not used.
  *
**/
class CHelpText: public CTextBox
{
public:
  /** Initializing method : **/
  int32_t initialize(ENVIRONMENT * env);
  /** Generate help text : **/
  int32_t generate(ENVIRONMENT * env);
};

/** @brief CStatsText
  *
  * A little simple class that holds the stats text for on screen display.
  * Instanciated by showOSStats only, thus never if not used.
  * However, to correctly display the stats, generate() has to be called!
**/
class CStatsText: public CTextBox
{
public:
  /** Initializing method : **/
  int32_t initialize(ENVIRONMENT * env);
  /** Generate stats : **/
  int32_t generate(ENVIRONMENT * env);
};

#endif // PWX_MAKESIMPLEXTEXTURE_TEXTBOX_H_INCLUDED
