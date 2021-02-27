# -------------------------------------------------
# --- Sources for the "basic" component         ---
# -------------------------------------------------

set( basic_HEADERS
     "${PROJECT_BINARY_DIR}/pwx_config.h"
     ${CMAKE_CURRENT_LIST_DIR}/CException.h
     ${CMAKE_CURRENT_LIST_DIR}/CLockGuard.h
     ${CMAKE_CURRENT_LIST_DIR}/CLockable.h
     ${CMAKE_CURRENT_LIST_DIR}/alloc_utils.h
     ${CMAKE_CURRENT_LIST_DIR}/compiler.h
     ${CMAKE_CURRENT_LIST_DIR}/debug.h
     ${CMAKE_CURRENT_LIST_DIR}/macros.h
     ${CMAKE_CURRENT_LIST_DIR}/mem_utils.h
     ${CMAKE_CURRENT_LIST_DIR}/string_utils.h
     ${CMAKE_CURRENT_LIST_DIR}/templates.h
     ${CMAKE_CURRENT_LIST_DIR}/trace_info.h
     ${CMAKE_CURRENT_LIST_DIR}/types.h
     )

target_sources( basic PRIVATE
                ${basic_HEADERS}
                CException.cpp
                CLockGuard.cpp
                CLockable.cpp
                _mem_map.cpp
                _mem_map.h
                mem_utils.cpp
                string_utils.cpp
                trace_info.cpp
                )
target_compile_definitions( basic PRIVATE PWX_EXPORTS )
target_compile_options( basic PRIVATE -fvisibility=hidden )


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( basic PRIVATE ${CMAKE_SOURCE_DIR}/src )


# -----------------------------------------
# --- Installation of the basic headers ---
# -----------------------------------------
install( FILES
         ${basic_HEADERS}
         DESTINATION include/pwxlib/basic
         COMPONENT pwxlib
         )
