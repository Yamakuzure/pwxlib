# ===============================================================================================================
# === The PrydeWorX Library is free software under MIT License ; See src/pwxlib.h                             ===
# ===============================================================================================================
# === This Makefile is "just" a wrapper for cmake/ninja.                                                      ===
# === The main purpose is to offer convenient simple GNU make commands, to enable users and developers to     ===
# === easily build the PrydeWorX Library in several ways. It is also nice for people not fluent in CMake. ;-) ===
# === ------------------------------------------------------------------------------------------------------- ===
# === The main configuration of the various aspects of building the PrydeWorX Library is managed in the file  ===
# === Makefile.pwx, which was especially designed to be included in other Makefiles as well.                  ===
# === ------------------------------------------------------------------------------------------------------- ===
# === The following aspects can be configured:   ( By issuing "make DEBUG=NO" for example )                   ===
# === DEBUG             : Set to YES to enable debug build mode.                                              ===
# === PREFIX            : Where to install the library. Default is to install in $(PROJECT_DIR)/install.      ===
# === SANITIZE_ADDRESS  : Set to YES to use the address sanitizer. Ignored if the thread sanitizer is enabled.===
# === SANITIZE_LEAK     : Set to YES to use the leak sanitizer. Ignored if the thread sanitizer is enabled.   ===
# === SANITIZE_THREAD   : Set to YES to use the thread sanitizer. Disables both address and leak sanitizer.   ===
# === SANITIZE_UNDEFINED: Set to YES to use the undefined sanitizer. Plays nicely with an other sanitizer.    ===
# === ------------------------------------------------------------------------------------------------------- ===
# === For more variables to configure, see Makefile.pwx. ( (c) Sven Eden ; PrydeWorX ; 2007 - 2021 )          ===
# ===============================================================================================================
.PHONY: all clean distclean veryclean

export
DEBUG ?= NO

PROJECT_DIR := ${CURDIR}
PREFIX      ?= ${PROJECT_DIR}/install
TARGET_NAME := pwx
TARGET      := pwxlib

SANITIZE_ADDRESS   ?= NO
SANITIZE_LEAK      ?= NO
SANITIZE_THREAD    ?= NO
SANITIZE_UNDEFINED ?= NO

# These are used by Makefile.pwx, but we do not need the automatic functionalities
SUBDIRS_C   :=
SUBDIRS_CXX :=
SUBDIRS_INC :=

# Now include the PrydeWorX tooling Makefile
include $(PROJECT_DIR)/Makefile.pwx

# -----------
# Used tools
# -----------
CMAKE := $(shell which cmake)
MAKE  := $(shell which make)
MKDIR := $(shell which mkdir)
NINJA := $(shell which ninja)
RM    := $(shell which rm) -rf


all: $(TARGET)


$(CMAKE_DIR):
	$(MKDIR) -p "$(CMAKE_DIR)"

$(CMAKE_CONF): $(CMAKE_DIR) Makefile CMakeLists.txt
	@( cd "$(CMAKE_DIR)" && \
		$(CMAKE) -DCMAKE_BUILD_TYPE=$(CMAKE_TARGET) \
		-DCMAKE_SANITIZE=$(CMAKE_SANITIZE)          \
		-DCMAKE_VERBOSE_MAKEFILE=$(CMAKE_VERBOSE)   \
		-DCPPFLAGS="$(CPPFLAGS)"                    \
		-G Ninja $(PROJECT_DIR) -DCMAKE_INSTALL_PREFIX=$(PREFIX) )
	@echo "CPPFLAGS    : $(CPPFLAGS)"
	@echo "CFLAGS      : $(CFLAGS)"
	@echo "CXXFLAGS    : $(CXXFLAGS)"
	@echo "LDFLAGS     : $(LDFLAGS)"

$(TARGET_NAME): $(CMAKE_CONF)
	@echo "Building $@ ..."
	$(CMAKE) --build "$(CMAKE_DIR)" --target $@
	@echo "$@ built"

$(TARGET): $(TARGET_NAME)
	@echo "Installing $@ ..."
	$(CMAKE) -DCMAKE_BUILD_TYPE=$(CMAKE_TARGET) -DCOMPONENT=$@ \
		-P "$(CMAKE_DIR)"/cmake_install.cmake $(CMAKE_STRIP)
	@echo "$@ installed"

distcleantgt:
	@echo "Dist-Cleaning $(CMAKE_DIR)"
	( test -d ${CMAKE_DIR} && $(RM) -r $(CMAKE_DIR) || true )

distclean: veryclean
veryclean: clean
	@echo "distcleaning..."
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt DEBUG=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt DEBUG=NO )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt SANITIZE_ADDRESS=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt SANITIZE_ADDRESS=YES SANITIZE_LEAK=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt SANITIZE_ADDRESS=YES SANITIZE_LEAK=YES SANITIZE_UNDEFINED=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt SANITIZE_LEAK=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt SANITIZE_LEAK=YES SANITIZE_UNDEFINED=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt SANITIZE_THREAD=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt SANITIZE_THREAD=YES SANITIZE_UNDEFINED=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . distcleantgt SANITIZE_UNDEFINED=YES )
	@echo " --- done --- "

cleantgt:
	@echo "Cleaning $(CMAKE_DIR)"
	( test -d ${CMAKE_DIR} && $(CMAKE) --build $(CMAKE_DIR) --target clean || true )

clean:
	@echo "cleaning..."
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt DEBUG=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt DEBUG=NO )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt SANITIZE_ADDRESS=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt SANITIZE_ADDRESS=YES SANITIZE_LEAK=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt SANITIZE_ADDRESS=YES SANITIZE_LEAK=YES SANITIZE_UNDEFINED=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt SANITIZE_LEAK=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt SANITIZE_LEAK=YES SANITIZE_UNDEFINED=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt SANITIZE_THREAD=YES )
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt SANITIZE_THREAD=YES SANITIZE_UNDEFINED=YES)
	@( CMAKE_DIR="cmake-build" $(MAKE) -C . cleantgt SANITIZE_UNDEFINED=YES )
	@echo " --- done --- "
