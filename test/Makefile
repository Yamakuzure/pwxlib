# ===================================================================
# Turn of the "Entering/Leaving directory foo" funtionality
MAKEFLAGS += --no-print-directory

SOURCES = $(wildcard *.cpp)
MODULES = $(SOURCES:.cpp=.o)
CXX     = $(shell which g++)
TARGET  = testlib

# Add src dir to VPATH and include dir
VPATH    = . ../src
INCDIR   = -I../src

# ------------------------------------
# Rules
# ------------------------------------
.PHONY: all clean
.SUFFIXES: .cpp

%.o: %.cpp
	@echo "Compiling $@"
	@$(CXX) $(INCDIR) $(CXXFLAGS) -o $@ -c $<

all: $(TARGET)

$(TARGET): depend $(MODULES)
	@echo "Linking $(TARGET) ..."
	@$(CXX) -o $(TARGET) $(LDFLAGS) -L../lib -lpwx $(MODULES)

clean:
	@echo "Cleaning $(TARGET) and object files"
	@rm -f $(TARGET) $(MODULES)

# ------------------------------------
# Create dependencies
# ------------------------------------
depend: Makefile
	@echo "Checking dependencies"
	@$(CXX) -MM $(CFLAGS) $(CXXFLAGS) $(SOURCES) > Makefile.dep

# ------------------------------------
# Include dependencies
# ------------------------------------
-include Makefile.dep
