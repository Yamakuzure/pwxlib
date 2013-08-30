# ===================================================================
# Turn of the "Entering/Leaving directory foo" funtionality
MAKEFLAGS += --no-print-directory

TARGET  = ../../drawwave
SOURCES = $(wildcard *.cpp)
MODULES = $(SOURCES:.cpp=.o)
CXX     = $(shell which g++)

# Add src dir to VPATH and include dir
VPATH   = . ../../src
INCDIR  = -I../../src

# SFML libraries to link
SFMLLIB = -lsfml-graphics -lsfml-window -lsfml-system

# ------------------------------------
# Rules
# ------------------------------------
.PHONY: all clean drawwave
.SUFFIXES: .cpp

all: drawwave

%.o: %.cpp
	@echo "Compiling $@"
	$(CXX) $(INCDIR) $(CXXFLAGS) -o $@ -c $<

$(TARGET): $(MODULES)
	@echo "Linking $(TARGET) ..."
	$(CXX) -o $(TARGET) $(LDFLAGS) -L../lib -lpwx $(SFMLLIB) $(MODULES)

drawwave: depend $(TARGET)
	@echo "test"

clean:
	@echo "Cleaning $(TARGET) and object files"
	@rm -f $(TARGET) $(MODULES)

# ------------------------------------
# Create dependencies
# ------------------------------------
depend: Makefile
	@echo "Checking dependencies"
	@$(CXX) -MM $(INCDIR) $(CFLAGS) $(CXXFLAGS) $(SOURCES) > Makefile.dep

# ------------------------------------
# Include dependencies
# ------------------------------------
-include Makefile.dep

