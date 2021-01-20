# ------------------------------------------------------------------------
# --- List file for the library itself. Dependencies are handled here. ---
# ------------------------------------------------------------------------
set( pwxlib_h ${CMAKE_CURRENT_LIST_DIR}/pwxlib.h PARENT_SCOPE )

set( pwx_WRAPPERS
     ${CMAKE_CURRENT_LIST_DIR}/PAH
     ${CMAKE_CURRENT_LIST_DIR}/PArgHandler
     ${CMAKE_CURRENT_LIST_DIR}/PBasic
     ${CMAKE_CURRENT_LIST_DIR}/PChainHash
     ${CMAKE_CURRENT_LIST_DIR}/PContainers
     ${CMAKE_CURRENT_LIST_DIR}/PDoubleList
     ${CMAKE_CURRENT_LIST_DIR}/PDoubleRing
     ${CMAKE_CURRENT_LIST_DIR}/PException
     ${CMAKE_CURRENT_LIST_DIR}/PLockable
     ${CMAKE_CURRENT_LIST_DIR}/PLockGuard
     ${CMAKE_CURRENT_LIST_DIR}/PMath
     ${CMAKE_CURRENT_LIST_DIR}/POpenHash
     ${CMAKE_CURRENT_LIST_DIR}/PQueue
     ${CMAKE_CURRENT_LIST_DIR}/PRandom
     ${CMAKE_CURRENT_LIST_DIR}/PSet
     ${CMAKE_CURRENT_LIST_DIR}/PSinCos
     ${CMAKE_CURRENT_LIST_DIR}/PSingleList
     ${CMAKE_CURRENT_LIST_DIR}/PSingleRing
     ${CMAKE_CURRENT_LIST_DIR}/PStack
     ${CMAKE_CURRENT_LIST_DIR}/PStreamHelpers
     ${CMAKE_CURRENT_LIST_DIR}/PTools
     ${CMAKE_CURRENT_LIST_DIR}/PUtils
     ${CMAKE_CURRENT_LIST_DIR}/PWaveColor
     ${CMAKE_CURRENT_LIST_DIR}/PWorkers
     ${CMAKE_CURRENT_LIST_DIR}/PWXLib
     ${CMAKE_CURRENT_LIST_DIR}/RNG
     ${CMAKE_CURRENT_LIST_DIR}/SCT
     )

string( REPLACE ";" " " pwx_WRAPPERS_STR "${pwx_WRAPPERS}" )
set( PROJECT_WRAPPERS "${pwx_WRAPPERS_STR}" PARENT_SCOPE )

set( pwx_obj_HEADERS
     ${pwx_WRAPPERS}
     ${pwxlib_h}
     "${PROJECT_BINARY_DIR}/pwx_config.h"
     )

add_library( pwx_obj OBJECT
             ${pwx_obj_HEADERS}
             pwxlib.cpp
             )
target_compile_definitions(pwx_obj PRIVATE PWX_EXPORTS)

# We need at least one file for the real libraries
if ( BUILD_SHARED_LIBS )
	target_sources( pwx PRIVATE pwxlib.h )
endif ()
if ( BUILD_STATIC_LIBS )
	target_sources( pwx_static PRIVATE pwxlib.h )
endif ()


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( pwx_obj PUBLIC ${CMAKE_SOURCE_DIR}/src )


# --------------------------------------
# --- Parts of the PrydeWorX library ---
# --------------------------------------
add_library( arg_handler OBJECT "" )
add_library( basic OBJECT "" )
add_library( container OBJECT "" )
add_library( functions OBJECT "" )
add_library( libpwx OBJECT "" )
add_library( math_helpers OBJECT "" )
add_library( random OBJECT "" )
add_library( stream_helpers OBJECT "" )
add_library( wavecolor OBJECT "" )
add_subdirectory( arg_handler )
add_subdirectory( basic )
add_subdirectory( container )
add_subdirectory( functions )
add_subdirectory( libpwx )
add_subdirectory( math_helpers )
add_subdirectory( random )
add_subdirectory( stream_helpers )
add_subdirectory( wavecolor )

# Propagate the symbols file up
set( libpwx_SYM ${libpwx_SYM} PARENT_SCOPE )

# --------------------------------------------
# --- Definitions of the PrydeWorX library ---
# --------------------------------------------
if ( BUILD_SHARED_LIBS )
	set_target_properties( pwx PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_SO_VERSION} )
	target_compile_definitions(pwx PRIVATE PWX_EXPORTS)
	target_link_libraries( pwx PRIVATE pwx_obj
	                       arg_handler
	                       basic
	                       container
	                       functions
	                       libpwx
	                       math_helpers
	                       random
	                       stream_helpers
	                       wavecolor
	                       PUBLIC ${CMAKE_THREAD_LIBS_INIT}
	                       )
	install( TARGETS pwx COMPONENT pwxlib )
endif ()

if ( BUILD_STATIC_LIBS )
	set_target_properties( pwx_static PROPERTIES VERSION ${PROJECT_VERSION} )
	target_compile_definitions(pwx_static PRIVATE PWX_EXPORTS)
	target_link_libraries( pwx_static PRIVATE pwx_obj
	                       arg_handler
	                       basic
	                       container
	                       functions
	                       libpwx
	                       math_helpers
	                       random
	                       stream_helpers
	                       wavecolor
	                       PUBLIC ${CMAKE_THREAD_LIBS_INIT}
	                       )
	install( TARGETS pwx_static DESTINATION lib COMPONENT pwxlib )
endif ()

# -----------------------------------------------------
# --- Installation of the PrydeWorX library headers ---
# -----------------------------------------------------
install( FILES
         ${pwx_obj_HEADERS}
         DESTINATION include/pwxlib
         COMPONENT pwxlib
         )
