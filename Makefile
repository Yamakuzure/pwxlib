VERSION = 0.8.99

# ===================================================================
# Set to YES to include debugging info
DEBUG := YES

# The following switch produces massive output and should
# not be enabled lightly. Make sure to disable it before
# running tools like helgrind or DRD on any pwxLib program!
THREADDEBUG := NO

# If the following is set to YES, glibc is told to annotate inter thread
# synchronization events. This is needed if you do not want to get
# plenty of false positives in Helgrind.
# See <where is the link gone?>
# Note: You must have valgrind installed on your system and
#       valgrind/helgrind.h somewhere it can be found (like /usr/include)
ANNOTATIONS := YES

# Set to YES to include profiling info
PROFILE := NO

# Set to YES to include graphite extensions on recent gcc versions
GRAPHITE := NO

# Set to YES to build a static version of the library
STATIC := NO

# Set to NO to use mutex instead of atomic_flag spinlocks
# By default pwxLib uses yielding spinlocks (see below
# for an explanation), but there might be reasons to use
# mutexes instead.
# Using spinlocks is not only more lightweight (an atomic_flag instead
# of a full scale mutex), but it has a bit better performance, too.
# Examples: (200,000 random elements inserted/removed, 200 elements retireved, 8 threads)
# Note: The testmachine is a dual core i7, 1333MHz, HyperThreading, 8GiB RAM.
#                              yielding spinlocks     |        mutextes
#                            Add /   Search /   Clear |      Add /   Search /   Clear
#  Singly linked lists    756 ms /   821 ms /  126 ms |  1225 ms /  1207 ms /  147 ms
#  Doubly linked lists    761 ms /  1448 ms /  163 ms |  1206 ms /  1359 ms /  189 ms
#  Singly linked rings    783 ms /   777 ms /  129 ms |  1214 ms /  1634 ms /  187 ms
#  Doubly linked rings    758 ms /  1472 ms /  170 ms |  1204 ms /  1926 ms /  244 ms
#  Stacks                 715 ms /   817 ms /  130 ms |  1223 ms /  1759 ms /  204 ms
#  Queues                 616 ms /  1440 ms /  177 ms |  1203 ms /  2010 ms /  258 ms
#  Sets                   831 ms /     0 ms /  691 ms |   971 ms /     0 ms /  633 ms
#  Chained Hash Tables    388 ms /     0 ms /  269 ms |   758 ms /     0 ms /  414 ms
#  Open Hash Tables       497 ms /     0 ms /  155 ms |   807 ms /     0 ms /  181 ms
USE_SPINLOCK := YES

# The following settings lets threads do a yield() whenever
# they wait to get the spinlock. This setting is ignored
# if USE_SPINLOCK is set to NO
# By default threads do yield, as this results in a much better
# performance when inserting or removing elements from containers.
# The performance of retrievals, howerver, is only slightly worse
# without the yield.
# Examples: (200,000 random elements inserted/removed, 200 elements retireved, 8 threads)
# Note: The testmachine is a dual core i7, 1333MHz, HyperThreading, 8GiB RAM.
#                              yield() enabled        | yield() disabled
#                            Add /   Search /   Clear |      Add /   Search /   Clear
#  Singly linked lists    756 ms /   821 ms /  126 ms |  9649 ms /   786 ms /  121 ms
#  Doubly linked lists    761 ms /  1448 ms /  163 ms | 35160 ms /  2248 ms /  171 ms
#  Singly linked rings    783 ms /   777 ms /  129 ms |  6254 ms /   799 ms /  121 ms
#  Doubly linked rings    758 ms /  1472 ms /  170 ms | 26490 ms /  2053 ms /  162 ms
#  Stacks                 715 ms /   817 ms /  130 ms |  5975 ms /   833 ms /  122 ms
#  Queues                 616 ms /  1440 ms /  177 ms | 36107 ms /  2161 ms /  172 ms
#  Sets                   831 ms /     0 ms /  691 ms |   814 ms /     0 ms / 2299 ms
#  Chained Hash Tables    388 ms /     0 ms /  269 ms |  1459 ms /     0 ms /  180 ms
#  Open Hash Tables       497 ms /     0 ms /  155 ms |  1513 ms /     0 ms /  150 ms
USE_SPINLOCK_YIELD := YES

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
DBG_CXXFLAGS = -ggdb3 -DLIBPWX_DEBUG -D_DEBUG
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

ifeq (YES, ${ANNOTATIONS})
  CXXFLAGS := ${CXXFLAGS} -DPWX_ANNOTATIONS
endif

ifeq (YES, ${USE_SPINLOCK})
  CXXFLAGS := ${CXXFLAGS} -DPWX_USE_FLAGSPIN
  ifeq (YES, ${USE_SPINLOCK_YIELD})
    CXXFLAGS := ${CXXFLAGS} -DPWX_USE_FLAGSPIN_YIELD
  endif
endif

# ------------------------------------
# Rules
# ------------------------------------
.PHONY: clean cleanlibrary cleantools testlib documentation docinstall \
		clustercheck hashbuilder install namegen userinstall \
		test tools torture all library help
.SUFFIXES: .cpp

%.o: %.cpp
	@echo "Compiling $@"
	$(CXX) $(INCDIR) $(CXXFLAGS) $(LIBCXXFLAGS) -o $@ -c $<

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

clustercheck: depend library
	@echo "Making clustercheck in $(TESTDIR)"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" make -j8 -C $(TESTDIR) clustercheck)

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

namegen: depend library
	@echo "Making namegen in $(TESTDIR)"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" make -j8 -C $(TESTDIR) namegen)

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

cleanlibrary:
	@echo "Cleaning library and object files"
	@rm -f $(LIB_DYN) $(LIB_STA) $(MODULES)

cleantest:
	@echo "Cleaning all in $(TESTDIR)"
	@make -C $(TESTDIR) clean

clean: cleanlibrary cleantest
#	@echo "Cleaning all in $(TOOLDIR)"
#	@make -C $(TOOLDIR) clean

test: testlib hashbuilder namegen torture

$(LIBDIR)/$(LIB_DYN): $(MODULES)
	@echo "Linking $(LIBDIR)/$(LIB_DYN)"
	$(CXX) $(LDFLAGS) $(LIBLDFLAGS) -Wl,-soname,$(LIB_DYN).$(VERSION) -o $(LIBDIR)/$(LIB_DYN) $(MODULES)
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
