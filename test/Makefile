# ===================================================================
# Turn of the "Entering/Leaving directory foo" funtionality
MAKEFLAGS += --no-print-directory

TARGET_hb   := ./hash_builder
TARGET_tl   := ./test_lib
SOURCES_all := $(wildcard *.cpp)
SOURCES_hb  := $(filter hash_builder.%, $(SOURCES_all))
SOURCES_tl  := $(filter-out hash_builder.%, $(SOURCES_all))
MODULES_hb  := $(SOURCES_hb:.cpp=.o)
MODULES_tl  := $(SOURCES_tl:.cpp=.o)
CXX         := $(shell which g++)

# Add src dir to VPATH and include dir
VPATH    = . ../src
INCDIR   = -I../src

# ------------------------------------
# Rules
# ------------------------------------
.PHONY: all clean hashbuilder testlib
.SUFFIXES: .cpp

%.o: %.cpp
	@echo "Compiling $@"
	@$(CXX) $(INCDIR) $(CXXFLAGS) -o $@ -c $<

all: $(TARGET_hb) $(TARGET_tl)

hashbuilder: depend $(TARGET_hb)

testlib: depend $(TARGET_tl)

$(TARGET_hb): $(MODULES_hb)
	@echo "Linking $(TARGET_hb) ..."
	@$(CXX) -o $(TARGET_hb) $(LDFLAGS) -L../lib -lpwx $(MODULES_hb)

$(TARGET_tl): $(MODULES_tl)
	@echo "Linking $(TARGET_tl) ..."
	@$(CXX) -o $(TARGET_tl) $(LDFLAGS) -L../lib -lpwx $(MODULES_tl)

clean:
	@echo "Cleaning $(TARGET_hb), $(TARGET_tl) and object files"
	@rm -f $(TARGET_hb) $(TARGET_tl) $(MODULES_hb) $(MODULES_tl)

# ------------------------------------
# Create dependencies
# ------------------------------------
depend: Makefile
	@echo "Checking dependencies"
	@$(CXX) -MM $(CFLAGS) $(CXXFLAGS) $(SOURCES_all) > Makefile.dep

# ------------------------------------
# Include dependencies
# ------------------------------------
-include Makefile.dep
