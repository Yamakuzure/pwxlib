.PHONY: install userinstall help documentation docinstall test tools clean

VERSION = "0.8.6"

# ===================================================================
# Set to YES to include debugging info
DEBUG := NO

# Set to YES to include profiling info
PROFILE := NO

# Set to YES to include graphite extensions on recent gcc versions
GRAPHITE := NO

# ===================================================================
# Turn of the "Entering/Leaving directory foo" funtionality
MAKEFLAGS += --no-print-directory

# ===================================================================
# ---- Font configuration for makeSimplexTexture and gravMat
# Font path for makeSimplexTexture and gravMat:
# If you intent to compile and use makeSimplexTexture and gravMat, found in
# pwxLib/tools, please make sure that the two following settings are correct.
# Both show the defaults used. But if you either work on a different system
# or don't have the font installed like assumed, you might have to change this.
#
FONT_PATH := DEFAULT
FONT_NAME := DEFAULT
# Linux defaults:
# FONT_PATH := /usr/share/fonts/freefont-ttf
# FONT_NAME := FreeMono.ttf
# Windows defaults:
# FONT_PATH = \"C:\\WINDOWS\\Fonts\"
# FONT_NAME = \"COUR.TTF\"
#
# Note: Please make sure to set a mono step font, or the help and state texts
#       might look odd!
#
# ===================================================================
# Compiler and linker flags for release, debugging and profiling

# DEBUG
DBG_CXXFLAGS = -pedantic -g -ggdb -O0 -DPWXLIB_DEBUG
DBG_LDFLAGS  =

# RELEASE
RLS_CXXFLAGS = -march=native -O2 -pipe -DNDEBUG
RLS_LDFLAGS  = -Wl,--sort-common -Wl,--hash-style=gnu -Wl,-O1 -s
# Note: -Wl,--as-needed does _not_ work!

# GRAPHITE
GRP_CXXFLAGS = -floop-interchange -fgraphite-identity -ftree-loop-distribution -floop-parallelize-all -floop-strip-mine -floop-block

# ===================================================================
INSTDIR   = /usr/include
USRINST   = /usr/local/include
CXX       = g++
CXXFLAGS += -std=c++0x -I. -Wall -Wextra -fexceptions
INSTALL   = `which install`
DOXYGEN   = `which doxygen`
TESTDIR   = test
TOOLDIR   = tools
DOCFILES  = AUTHORS ChangeLog COPYING INSTALL README TODO
DOCDIR    = /usr/share/doc/pwxLib-$(VERSION)

# ===================================================================
# Set flags according to settings:
ifeq (YES, ${DEBUG})
  CXXFLAGS     := ${CXXFLAGS} ${DBG_CXXFLAGS}
  LDFLAGS      := ${LDFLAGS} ${DBG_LDFLAGS}
else
  CXXFLAGS     := ${CXXFLAGS} ${RLS_CXXFLAGS}
  LDFLAGS      := ${LDFLAGS} ${RLS_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
  CXXFLAGS := ${CXXFLAGS} -pg -O2
	LDFLAGS  := ${LDFLAGS} -pg
endif

ifeq (YES, ${GRAPHITE})
  CXXFLAGS := ${CXXFLAGS} ${GRP_CXXFLAGS}
endif

all:
	@echo "No build target specified!"
	@echo "Use \"make help\" for a list of build targets"

help: Makefile
	@echo "pwxLib Version $(VERSION)"
	@echo "------------------------"
	@echo "Use one of the following build targets:"
	@echo ""
	@echo "  documentation - use $(DOXYGEN) to build"
	@echo "                  documentation in ./doc/html"
	@echo "  docinstall    - install documentation into $(DOCDIR)"
	@echo "  clean         - remove object files, test programs and tools"
	@echo "  help          - this help"
	@echo "  install       - copy pwxLib to $(INSTDIR)"
	@echo "  userinstall   - copy pwxLib to $(USRINST)"
	@echo "  test          - build test programs in ./$(TESTDIR) (*)"
	@echo "  tools         - build tools in ./$(TOOLDIR) (*)"
	@echo ""
	@echo "(*): You can turn on debugging and profiling information or the use"
	@echo "     of the gcc graphite extension by editing this Makefile."
	@echo ""
	@echo "Note: It is not necessary to _install_ the library to be able to"
	@echo "      compile the test programs and tools."
	@echo ""
	@echo "To use pwxLib there is no build process necessary, as pwxLib is a"
	@echo "JBoH (Just a Bunch of Headers) library. All you have to do is to"
	@echo "copy the pwxLib sub-folder somewhere your compiler finds it, and"
	@echo "add an appropriate include statement to your sources:"
	@echo ""
	@echo "#include <pwxLib/CCnfFilehandler.h> // To use pwx::CFH"
	@echo "#include <pwxLib/CMemRingFactory.h> // To use pwx::MRF (*)"
	@echo "#include <pwxLib/CRandom.h>         // To use pwx::RNG"
	@echo "#include <pwxLib/CSinCosTables.h>   // To use pwx::SCT"
	@echo "#include <pwxClib/CWaveColor.h>     // To provide the CWaveColor class"
	@echo ""
	@echo "Additionally there are some tools you might want to take a look at:"
	@echo "#include <pwxLib/tools/Args.h>           // A program arguments parsing system"
	@echo "#include <pwxLib/tools/DefaultDefines.h> // helper macros and functions for"
	@echo "                                         // comparison, math and type traits"
	@echo "#include <pwxLib/tools/Exception.h>      // The trace adding pwx base exception"
	@echo "#include <pwxLib/tools/MathHelpers.h>    // Some mathematical helper functions"
	@echo "#include <pwxLib/tools/MRInterface.h>    // Use the thread safe interface to MRF"
	@echo "                                         // containers (*)"
	@echo "#include <pwxLib/tools/StreamHelpers.h>  // Some helper functions for streams"
	@echo ""
	@echo "(*): It doesn't matter whether you include MRInterface.h or CMemRingFactory.h,"
	@echo "     both will ensure the existence of another."

documentation: Makefile Doxyfile
	@echo "generating documentation"
	@$(DOXYGEN) ./Doxyfile

docinstall: documentation
	@echo "Installing HTML Documentation into $(DOCDIR)"
	@$(INSTALL) -d $(DOCDIR)/html
	@$(INSTALL) doc/html/* $(DOCDIR)/html
	@echo "Installing information files into $(DOCDIR)"
	@$(INSTALL) $(DOCFILES) $(DOCDIR)

install: Makefile
	@echo "Installing pwxLib into $(INSTDIR)"
	@$(INSTALL) -d $(INSTDIR)/pwxLib
	@$(INSTALL) -d $(INSTDIR)/pwxLib/internal
	@$(INSTALL) -d $(INSTDIR)/pwxLib/external
	@$(INSTALL) -d $(INSTDIR)/pwxLib/tools
	@$(INSTALL) pwxLib/*.h $(INSTDIR)/pwxLib
	@$(INSTALL) pwxLib/internal/*.h $(INSTDIR)/pwxLib/internal
	@$(INSTALL) pwxLib/external/*.h $(INSTDIR)/pwxLib/external
	@$(INSTALL) pwxLib/tools/*.h $(INSTDIR)/pwxLib/tools

userinstall: Makefile
	@echo "Installing pwxLib into $(USRINST)"
	@$(INSTALL) -d $(USRINST)/pwxLib
	@$(INSTALL) -d $(USRINST)/pwxLib/internal
	@$(INSTALL) -d $(USRINST)/pwxLib/external
	@$(INSTALL) -d $(USRINST)/pwxLib/tools
	@$(INSTALL) pwxLib/*.h $(USRINST)/pwxLib
	@$(INSTALL) pwxLib/internal/*.h $(USRINST)/pwxLib/internal
	@$(INSTALL) pwxLib/external/*.h $(USRINST)/pwxLib/external
	@$(INSTALL) pwxLib/tools/*.h $(USRINST)/pwxLib/tools


test:
	@echo "Making all in $(TESTDIR)"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" make -j8 -C $(TESTDIR))

tools:
	@echo "Making all in $(TOOLDIR)"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" FONT_PATH="$(FONT_PATH)" FONT_NAME="$(FONT_NAME)" make -C $(TOOLDIR))

clean:
	@echo "Cleaning all in $(TESTDIR)"
	@make -C $(TESTDIR) clean
	@echo "Cleaning all in $(TOOLDIR)"
	@make -C $(TOOLDIR) clean
