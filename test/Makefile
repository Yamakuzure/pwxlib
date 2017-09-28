# ===================================================================
# Turn of the "Entering/Leaving directory foo" funtionality
MAKEFLAGS += --no-print-directory

TARGET_cc   := ../clustercheck
TARGET_hb   := ../hashbuilder
TARGET_ng   := ../namegen
TARGET_tl   := ../testlib
TARGET_to   := ../torture
SOURCES_all := $(wildcard *.cpp)
SOURCES_cc  := $(filter cluster_check.%, $(SOURCES_all))
SOURCES_hb  := $(filter hash_builder.%, $(SOURCES_all))
SOURCES_ng  := $(filter namegen.%, $(SOURCES_all))
SOURCES_tl  := $(filter-out hash_builder.% namegen.% torture.% cluster_check.%, $(SOURCES_all))
SOURCES_to  := $(filter torture.%, $(SOURCES_all))
MODULES_cc  := $(SOURCES_cc:.cpp=.o)
MODULES_hb  := $(SOURCES_hb:.cpp=.o)
MODULES_ng  := $(SOURCES_ng:.cpp=.o)
MODULES_tl  := $(SOURCES_tl:.cpp=.o)
MODULES_to  := $(SOURCES_to:.cpp=.o)
CXX         := $(shell which g++)

# Add src dir to VPATH and include dir
VPATH    = . ../src
INCDIR   = -I../src

# ------------------------------------
# Rules
# ------------------------------------
.PHONY: all clean clustercheck hashbuilder namegen testlib torture
.SUFFIXES: .cpp

%.o: %.cpp
	@echo "Compiling $@"
	$(CXX) $(INCDIR) $(CXXFLAGS) -o $@ -c $<

all: $(TARGET_hb) $(TARGET_tl)

list:
	@echo "SOURCES_cc: $(SOURCES_cc)"
	@echo "SOURCES_hb: $(SOURCES_hb)"
	@echo "SOURCES_ng: $(SOURCES_ng)"
	@echo "SOURCES_tl: $(SOURCES_tl)"
	@echo "SOURCES_to: $(SOURCES_to)"

clustercheck: depend $(TARGET_cc)

hashbuilder: depend $(TARGET_hb)

namegen: depend $(TARGET_ng)

testlib: depend $(TARGET_tl)

torture: depend $(TARGET_to)

$(TARGET_cc): $(MODULES_cc)
	@echo "Linking $(TARGET_cc) ..."
	$(CXX) -o $(TARGET_cc) $(LDFLAGS) -L../lib -lpwx $(MODULES_cc)

$(TARGET_hb): $(MODULES_hb)
	@echo "Linking $(TARGET_hb) ..."
	$(CXX) -o $(TARGET_hb) $(LDFLAGS) -L../lib -lpwx $(MODULES_hb)

$(TARGET_ng): $(MODULES_ng)
	@echo "Linking $(TARGET_ng) ..."
	$(CXX) -o $(TARGET_ng) $(LDFLAGS) -L../lib -lpwx $(MODULES_ng)

$(TARGET_tl): $(MODULES_tl)
	@echo "Linking $(TARGET_tl) ..."
	$(CXX) -o $(TARGET_tl) $(LDFLAGS) -L../lib -lpwx $(MODULES_tl)

$(TARGET_to): $(MODULES_to)
	@echo "Linking $(TARGET_to) ..."
	$(CXX) -o $(TARGET_to) $(LDFLAGS) -L../lib -lpwx $(MODULES_to)

clean:
	@echo "Cleaning $(TARGET_cc) $(TARGET_hb), $(TARGET_tl), $(TARGET_to) and object files"
	@rm -f $(TARGET_cc) $(TARGET_hb) $(TARGET_tl) $(TARGET_to) $(MODULES_cc) $(MODULES_hb) $(MODULES_tl) $(MODULES_to)

# ------------------------------------
# Create dependencies
# ------------------------------------
depend: Makefile
	@echo "Checking dependencies"
	@$(CXX) -MM $(INCDIR) $(CFLAGS) $(CXXFLAGS) $(SOURCES_all) > Makefile.dep

# ------------------------------------
# Include dependencies
# ------------------------------------
-include Makefile.dep
