# -------------------------------------------------
# --- Sources for the "functions" component     ---
# -------------------------------------------------

set( functions_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/pwx_set_func.h
     ${CMAKE_CURRENT_LIST_DIR}/pwx_set_fwd.h
     )

target_sources( functions PRIVATE
                ${functions_HEADERS}
                )

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
