# -------------------------------------------------
# --- Sources for the "basic" component         ---
# -------------------------------------------------

set( basic_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/CException.h
     ${CMAKE_CURRENT_LIST_DIR}/CLockGuard.h
     ${CMAKE_CURRENT_LIST_DIR}/CLockable.h
     ${CMAKE_CURRENT_LIST_DIR}/_mem_map.h
     ${CMAKE_CURRENT_LIST_DIR}/_mem_wrapper.h
     ${CMAKE_CURRENT_LIST_DIR}/alloc_utils.h
     ${CMAKE_CURRENT_LIST_DIR}/mem_utils.h
     ${CMAKE_CURRENT_LIST_DIR}/pwx_compiler.h
     ${CMAKE_CURRENT_LIST_DIR}/pwx_debug.h
     ${CMAKE_CURRENT_LIST_DIR}/pwx_macros.h
     ${CMAKE_CURRENT_LIST_DIR}/pwx_templates.h
     ${CMAKE_CURRENT_LIST_DIR}/pwx_trace_info.h
     ${CMAKE_CURRENT_LIST_DIR}/pwx_types.h
     ${CMAKE_CURRENT_LIST_DIR}/string_utils.h
     )

target_sources( basic PRIVATE
                ${basic_HEADERS}
                CException.cpp
                CLockGuard.cpp
                CLockable.cpp
                _mem_map.cpp
                debug.cpp
                mem_utils.cpp
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
