#
# PrydeWorX C/C++ Library ; (c) Sven Eden ; 2007 - 2021
# -----------------------------------------------------
#
cmake_minimum_required( VERSION 3.17 )
set( CMAKE_CONFIGURATION_TYPES Debug Release CACHE INTERNAL "Available configuration types" )

if ( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release )
endif ( NOT CMAKE_BUILD_TYPE )

set( PROJECT_NAME "pwxlib" )
set( PROJECT_DESCRIPTION "PrydeWorX library of common C/C++ tools" )
set( PROJECT_HOMEPAGE_URL "https://github.com/Yamakuzure/pwxlib" )
set( PROJECT_VERSION 0.8.9.2 )

project( ${PROJECT_NAME}
         VERSION ${PROJECT_VERSION}
         DESCRIPTION ${PROJECT_DESCRIPTION}
         HOMEPAGE_URL ${PROJECT_HOMEPAGE_URL}
         LANGUAGES C CXX
         )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_C_STANDARD_REQUIRED True )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED True )
# Always warn about everything!
add_compile_options( -pipe -ftree-vectorize -Wall -Wextra )

set( CMAKE_DEBUG_POSTFIX "_d" )

# ---------------------------------------------------
# --- Default directories if not set from outside ---
# --- ( or by GNUInstallDirs )                    ---
# ---------------------------------------------------
if ( UNIX AND NOT APPLE )
	include( GNUInstallDirs )
endif ()
if ( NOT DEFINED CMAKE_INSTALL_LIBDIR )
	set( CMAKE_INSTALL_LIBDIR "lib" )
endif ()
if ( NOT DEFINED CMAKE_INSTALL_DOCDIR )
	set( CMAKE_INSTALL_LIBDIR "doc" )
endif ()


# -----------------------
# --- Needed Packages ---
# -----------------------
find_package( Threads )


# -----------------------
# --- Needed Headers ---
# -----------------------
check_include_file( threads.h HAVE_THREADS_H )
if ( NOT HAVE_THREADS_H )
	set( HAVE_THREADS_H 0 )
endif ()


# --------------------
# --- Options List ---
# --------------------
# ( Allow projects including this to hard-code our options )
cmake_policy( SET CMP0077 NEW )

option( BUILD_SHARED_LIBS "Enable build of the shared libpwx. (Default: ON)" ON )
option( BUILD_STATIC_LIBS "Enable build of the static libpwx. (Default: OFF)" OFF )

string( CONCAT HELP_SANITIZE
        "Set to address, leak, thread, undefined or OFF (default). "
        "The thread sanitizer can not be combined with either the address or the leak sanitizer, and will also disable "
        "both ENABLE_SPINLOCKS AND ENABLE_YIELDING. "
        "Choosing the undefined sanitizer will also enable float-divide-by-zero, as we nowhere try to obtain "
        "infinities and NaNs by deviding floating point by zero. "
        "You can combine sanitizers by providing a list."
        )
option( CMAKE_SANITIZE "${HELP_SANITIZE}" OFF )

# By default pwxLib uses yielding spinlocks (see below for an explanation), but there might be reasons to use
# mutexes instead.
# Using spinlocks is not only more lightweight (an atomic_flag instead of a full scale mutex), but it has a bit better
# performance, too.
# Examples: (200,000 random elements inserted/removed, 200 elements searched, 8 threads)
# Note: The test machine was a dual core i7, 1333MHz, Hyper Threading, 8GiB RAM.
#                              yielding spinlocks     |        mutexes
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
option( ENABLE_SPINLOCKS "Use atomic_flag spinlocks instead of mutexes. (Default: ON)" ON )

# The following settings lets threads do a yield() whenever they wait to get the spinlock. This setting is ignored if
# mutexes are used, see above.
# By default threads do yield, as this results in a much better performance when inserting or removing elements from
# containers. The performance of retrievals, however, is only slightly worse than without the yield.
# Examples: (200,000 random elements inserted/removed, 200 elements searched, 8 threads)
# Note: The test machine was a dual core i7, 1333MHz, Hyper Threading, 8GiB RAM.
#                              yield() enabled        |        yield() disabled
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
option( ENABLE_YIELDING "Let spinlocks yield when they can not lock. (Default: ON)" ON )
option( ENABLE_INSTALL_TESTS "Install the pwxLib test programs (Default: OFF)" OFF )
option( ENABLE_TORTURE "Build \"torture\", a multi threading container burner (Default: OFF)" OFF )
option( ENABLE_SMALL_TESTS "Test only 1/100th elements in test_lib (Default: OFF)" OFF )

string( CONCAT HELP_ANNOTATIONS
        "Tell glibc annotate inter thread synchronization events. "
        "This is needed if you do not want to get plenty of false positives in Helgrind. "
        "You must have valgrind installed on your system and valgrind/helgrind.h somewhere it can be found. "
        "Enabling this option will disable both ENABLE_SPINLOCKS AND ENABLE_YIELDING. "
        "(Default: OFF)"
        )
option( WITH_ANNOTATIONS ${HELP_ANNOTATIONS} OFF )
option( WITH_DOC "Install general documentation like README.md (Default: ON" ON )
option( WITH_HTML "Use doxygen to build HTML documentation (Default: ON)" ON )
option( WITH_TESTS "Build the pwxLib test programs (Default: OFF)" OFF )

# -------------------------------------
# --- Options and settings handling ---
# -------------------------------------
set( SANITIZER_FLAGS "" )

# --- Check thread sanitizer first, as it will make error out both address and leak sanitizer selection ---
set( HAVE_THREAD_SANITIZER OFF )
if ( CMAKE_SANITIZE MATCHES "[Tt]hread" )
	set( HAVE_THREAD_SANITIZER ON )
	set( SANITIZER_FLAGS "-fsanitize=thread" )
endif ()

# --- Check address sanitizer, but error out if the thread sanitizer was also chosen ---
if ( CMAKE_SANITIZE MATCHES "[Aa]ddress" )
	if ( HAVE_THREAD_SANITIZER )
		error( "NOT enabling address sanitizer, because the thread sanitizer was enabled!" )
	else ()
		set( SANITIZER_FLAGS "-fsanitize=address" )
	endif ()
endif ()

# --- Check leak sanitizer, but error out if the thread sanitizer was also chosen ---
if ( CMAKE_SANITIZE MATCHES "[Ll]eak" )
	if ( HAVE_THREAD_SANITIZER )
		error( "NOT enabling leak sanitizer, because the thread sanitizer was enabled!" )
	else ()
		set( SANITIZER_FLAGS "${SANITIZER_FLAGS} -fsanitize=leak" )
	endif ()
endif ()

# --- check undefined sanitzer and also enable float-divide-by-zero if chosen ---
if ( CMAKE_SANITIZE MATCHES "[Uu]ndefined" )
	set( SANITIZER_FLAGS "${SANITIZER_FLAGS} -fsanitize=undefined -fsanitize=float-divide-by-zero" )
endif ()

# --- Disable spinlocks and yielding if annotations are enabled ---
if ( WITH_ANNOTATIONS )
	set( PWX_ANNOTATIONS 1 CACHE BOOL INTERNAL )
else ()
	set( PWX_ANNOTATIONS 0 CACHE BOOL INTERNAL )
endif ()

# --- Further options that need a configuration entry ---
if ( ENABLE_SMALL_TESTS )
	set( PWX_SMALL_TESTS 1 CACHE BOOL INTERNAL )
else ()
	set( PWX_SMALL_TESTS 0 CACHE BOOL INTERNAL )
endif ()

if ( ENABLE_SPINLOCKS AND NOT HAVE_THREAD_SANITIZER )
	set( PWX_USE_FLAGSPIN 1 CACHE BOOL INTERNAL )
	if ( ENABLE_YIELDING )
		set( PWX_USE_FLAGSPIN_YIELD 1 CACHE BOOL INTERNAL )
	else ()
		set( PWX_USE_FLAGSPIN_YIELD 0 CACHE BOOL INTERNAL )
	endif ()
else ()
	set( PWX_USE_FLAGSPIN 0 CACHE BOOL INTERNAL )
	set( PWX_USE_FLAGSPIN_YIELD 0 CACHE BOOL INTERNAL )
endif ()

configure_file( src/pwx_config.h.in pwx_config.h )


# --------------------------------------------------
# --- Add proper defines according to build type ---
# --------------------------------------------------
set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}     -D_DEBUG -DLIBPWX_DEBUG" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}   -D_DEBUG -DLIBPWX_DEBUG -fexceptions" )
set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}   -DNDEBUG" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -fexceptions" )
if ( UNIX )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -fPIC" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
endif ()


# ---------------------------------------------------------------------
# --- To be able to install to some test dir, and still have the    ---
# --- test programs find the appropriate lib, set some vars for it. ---
# ---------------------------------------------------------------------
# Do NOT_ skip the full RPATH for the build tree
set( CMAKE_SKIP_BUILD_RPATH FALSE )

# When building, don't use the install RPATH already (but later on when installing)
set( CMAKE_BUILD_WITH_INSTALL_RPATH FALSE )
set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )

# Add the automatically determined parts of the RPATH which point
# to directories outside the build tree to the install RPATH
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )

# The RPATH to be used when installing, but only if it's not a system directory
list( FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir )
if ( "${isSystemDir}" STREQUAL "-1" )
	set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
endif ()


# -------------------------------------------------------------------------
# --- All components have to search the binary dir first, so they find  ---
# --- the latest generated pwx_config.h                                 ---
# -------------------------------------------------------------------------
include_directories( "${PROJECT_BINARY_DIR}" )


# ----------------------------------------------------------
# --- Main library definitions, filled later in the subs ---
# ----------------------------------------------------------
if ( BUILD_SHARED_LIBS )
	add_library( pwx SHARED "" )
endif ()
if ( BUILD_STATIC_LIBS )
	add_library( pwx_static STATIC "" )
endif ()

if ( BUILD_SHARED_LIBS OR BUILD_STATIC_LIBS )
	# ----------------------------------------------------------
	# --- Include the src directory, the library lives there ---
	# ----------------------------------------------------------
	add_subdirectory( src )


	# ----------------------------------------------------------------
	# --- Include the test directory, the test programs live there ---
	# ----------------------------------------------------------------
	include( CTest )
	enable_testing()
	if ( BUILD_TESTING )
		add_subdirectory( test )

		if ( NOT MSVC )
			set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
		endif ()
	endif ()
endif ()


# ----------------------------------------------------------------
# --- Build documentation if wanted and if we have doxygen.    ---
# ----------------------------------------------------------------
if ( WITH_HTML )
	add_subdirectory( doc )
endif ()

if ( WITH_DOC )
	install( FILES
	         AUTHORS
	         ChangeLog
	         code_of_conduct.md
	         CONTRIBUTING.md
	         INSTALL.md
	         LICENSE
	         NEWS.md
	         README.md
	         TODO.md
	         DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT pwxlib
	         )
endif ()

