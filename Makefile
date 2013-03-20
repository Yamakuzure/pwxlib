VERSION = 0.8.99

# ===================================================================
# Set to YES to include debugging info
DEBUG := YES

# The following switch produces massive output and should
# not be enabled lightly. Make sure to disable it before
# running tools like helgrind or DRD on any pwxLib program!
THREADDEBUG := YES

# Note: You can add additional debugging options in pwx/functions/debug.h

# Set to YES to include profiling info
PROFILE := NO

# Set to YES to include graphite extensions on recent gcc versions
GRAPHITE := NO

# Set to YES to build a static version of the library
STATIC := NO

# ===================================================================
# Turn of the "Entering/Leaving directory foo" funtionality
MAKEFLAGS += --no-print-directory

# ===================================================================
# Location of sources and object files
# SRC_CON = $(wildcard src/pwx/container/*.cpp)
# SRC_GEN = $(wildcard src/pwx/general/*.cpp)
# SRC_TYP = $(wildcard src/pwx/types/*.cpp)
# SRC_WOR = $(wildcard src/pwx/worker/*.cpp)

SOURCES = $(wildcard src/pwx/*/*.cpp)
MODULES = $(SOURCES:.cpp=.o)

LIB_DYN = libpwx.so
LIB_STA = libpwx-s.a

TARGET = $(LIB_DYN)

ifeq ($(STATIC), YES)
  TARGET      := ${LIB_STA}
  LIBCXXFLAGS :=
  LIBLDFLAGS  :=
else
  TARGET      := ${LIB_DYN}
  LIBCXXFLAGS := -fPIC
  LIBLDFLAGS  := -shared
endif

# ===================================================================
# Compiler and linker flags for release, debugging and profiling

# DEBUG
DBG_CXXFLAGS = -ggdb -DLIBPWX_DEBUG
DBG_LDFLAGS  =

# RELEASE
RLS_CXXFLAGS = -march=native -pipe -DNDEBUG
RLS_LDFLAGS  = -Wl,--sort-common -Wl,--hash-style=gnu -Wl,-O1 -s
# Note: -Wl,--as-needed does _not_ work!

# GRAPHITE
GRP_CXXFLAGS = -floop-interchange -fgraphite-identity -ftree-loop-distribution -floop-parallelize-all -floop-strip-mine -floop-block

# ===================================================================
INSTDIR   = /usr/include
USRINST   = /usr/local/include
AR        = $(shell which ar)
ARFLAGS   = rcs
CXX       = $(shell which g++)
CXXFLAGS += -std=c++11 -Wall -Wextra -fexceptions -pedantic -O2 -pthread
LDFLAGS  += -lpthread
INSTALL   = $(shell which install)
DOXYGEN   = $(shell which doxygen)
LIBDIR    = lib
TESTDIR   = test
TOOLDIR   = tools
DOCFILES  = AUTHORS ChangeLog COPYING INSTALL README TODO
DOCDIR    = /usr/share/doc/pwxLib-$(VERSION)

# Add src dir to VPATH and include dir
VPATH    = src
INCDIR   = -Isrc

# ===================================================================
# Set flags according to settings:
ifeq (YES, ${DEBUG})
  CXXFLAGS := ${CXXFLAGS} ${DBG_CXXFLAGS}
  LDFLAGS  := ${LDFLAGS} ${DBG_LDFLAGS}
else
  CXXFLAGS := ${CXXFLAGS} ${RLS_CXXFLAGS}
  LDFLAGS  := ${LDFLAGS} ${RLS_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
  CXXFLAGS := ${CXXFLAGS} -pg -O2
  LDFLAGS  := ${LDFLAGS} -pg
endif

ifeq (YES, ${GRAPHITE})
  CXXFLAGS := ${CXXFLAGS} ${GRP_CXXFLAGS}
endif

ifeq (YES, ${THREADDEBUG})
  CXXFLAGS := ${CXXFLAGS} -DPWX_THREADDEBUG
endif


# ------------------------------------
# Rules
# ------------------------------------
.PHONY: clean testlib documentation docinstall hashbuilder install userinstall tools torture all library help
.SUFFIXES: .cpp

%.o: %.cpp
	@echo "Compiling $@"
	@$(CXX) $(INCDIR) $(CXXFLAGS) $(LIBCXXFLAGS) -o $@ -c $<

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
	@echo "  hashbuilder   - build hashbuilder test program (*)"
	@echo "  install       - install pwxLib in $(INSTDIR)"
	@echo "  library       - compile pwxLib (*)"
	@echo "  userinstall   - install pwxLib in $(USRINST)"
	@echo "  testlib       - build testlib test program (*)"
	@echo "  tools         - build tools in ./$(TOOLDIR) (*)"
	@echo "  torture       - build torture test program (*)"
	@echo ""
	@echo "(*): You can turn on debugging and profiling information or the use"
	@echo "     of the gcc graphite extension by editing this Makefile."
	@echo ""

documentation: Makefile Doxyfile
	@echo "generating documentation"
	@$(DOXYGEN) ./Doxyfile

docinstall: documentation
	@echo "Installing HTML Documentation into $(DOCDIR)"
	@$(INSTALL) -d $(DOCDIR)/html
	@$(INSTALL) doc/html/* $(DOCDIR)/html
	@echo "Installing information files into $(DOCDIR)"
	@$(INSTALL) $(DOCFILES) $(DOCDIR)

hashbuilder: depend library
	@echo "Making hash_builder in $(TESTDIR)"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" make -j8 -C $(TESTDIR) hashbuilder)

install: Makefile $(TARGET)
	@echo "Installing pwxLib into $(INSTDIR)"
	@$(INSTALL) -d $(INSTDIR)/pwx
	@$(INSTALL) -d $(INSTDIR)/pwx/container
	@$(INSTALL) -d $(INSTDIR)/pwx/general
	@$(INSTALL) -d $(INSTDIR)/pwx/types
	@$(INSTALL) -d $(INSTDIR)/pwx/worker
	@$(INSTALL) src/*.h $(INSTDIR)
	@$(INSTALL) src/pwx/*.h $(INSTDIR)/pwx
	@$(INSTALL) pwx/container/*.h $(INSTDIR)/pwx/container
	@$(INSTALL) pwx/general/*.h $(INSTDIR)/pwx/general
	@$(INSTALL) pwx/types/*.h $(INSTDIR)/pwx/types
	@$(INSTALL) pwx/worker/*.h $(INSTDIR)/pwx/worker

library: depend $(LIBDIR)/$(TARGET)

userinstall: Makefile $(TARGET)
	@echo "Installing pwxLib into $(USRINST)"
	@$(INSTALL) -d $(USRINST)/pwx
	@$(INSTALL) -d $(USRINST)/pwx/container
	@$(INSTALL) -d $(USRINST)/pwx/general
	@$(INSTALL) -d $(USRINST)/pwx/types
	@$(INSTALL) -d $(USRINST)/pwx/worker
	@$(INSTALL) src/*.h $(USRINST)
	@$(INSTALL) src/pwx/*.h $(USRINST)/pwx
	@$(INSTALL) pwx/container/*.h $(USRINST)/pwx/container
	@$(INSTALL) pwx/general/*.h $(USRINST)/pwx/general
	@$(INSTALL) pwx/types/*.h $(USRINST)/pwx/types
	@$(INSTALL) pwx/worker/*.h $(USRINST)/pwx/worker

testlib: depend library
	@echo "Making testlib in $(TESTDIR)"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" make -j8 -C $(TESTDIR) testlib)

torture: depend library
	@echo "Making torture in $(TESTDIR)"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" make -j8 -C $(TESTDIR) torture)

tools: depend library
	@echo "Making all in $(TOOLDIR)"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" FONT_PATH="$(FONT_PATH)" FONT_NAME="$(FONT_NAME)" make -C $(TOOLDIR))

clean:
	@echo "Cleaning library and object files"
	@rm -f $(LIB_DYN) $(LIB_STA) $(MODULES)
	@echo "Cleaning all in $(TESTDIR)"
	@make -C $(TESTDIR) clean
#	@echo "Cleaning all in $(TOOLDIR)"
#	@make -C $(TOOLDIR) clean

$(LIBDIR)/$(LIB_DYN): $(MODULES)
	@echo "Linking $(LIBDIR)/$(LIB_DYN)"
	@$(CXX) $(LDFLAGS) $(LIBLDFLAGS) -Wl,-soname,$(LIB_DYN).$(VERSION) -o $(LIBDIR)/$(LIB_DYN) $(MODULES)
	@ln -sf $(LIB_DYN) $(LIBDIR)/$(LIB_DYN).$(VERSION)

$(LIBDIR)/$(LIB_STA): $(MODULES)
	@echo "Linking $(LIBDIR)/$(LIB_STA)"
	@$(AR) $(ARFLAGS) $(LIBDIR)/$(LIB_STA) $(MODULES)

# ------------------------------------
# Create dependencies
# ------------------------------------
depend: Makefile
	@echo "Checking dependencies"
	@$(CXX) -MM $(INCDIR) $(CXXFLAGS) $(SOURCES) > Makefile.dep

# ------------------------------------
# Include dependencies
# ------------------------------------
-include Makefile.dep
