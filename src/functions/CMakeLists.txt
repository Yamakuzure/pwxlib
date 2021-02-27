# -------------------------------------------------
# --- Sources for the "functions" component     ---
# -------------------------------------------------

set( functions_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/set_func.h
     ${CMAKE_CURRENT_LIST_DIR}/set_fwd.h
     )

target_sources( functions PRIVATE
                ${functions_HEADERS}
                )
target_compile_definitions( functions PRIVATE PWX_EXPORTS )
target_compile_options( functions PRIVATE -fvisibility=hidden )
set_target_properties( functions PROPERTIES LINKER_LANGUAGE CXX )


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( functions PRIVATE ${CMAKE_SOURCE_DIR}/src )


# ---------------------------------------------
# --- Installation of the container headers ---
# ---------------------------------------------
install( FILES
         ${functions_HEADERS}
         DESTINATION include/pwxlib/functions
         COMPONENT pwxlib
         )
