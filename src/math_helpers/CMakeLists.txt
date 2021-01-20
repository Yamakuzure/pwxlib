# -------------------------------------------------
# --- Sources for the "math_helpers" component  ---
# -------------------------------------------------

set( math_helpers_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/CSinCosTable.h
     ${CMAKE_CURRENT_LIST_DIR}/MathHelpers.h
     )

target_sources( math_helpers PRIVATE
                ${math_helpers_HEADERS}
                CSinCosTable.cpp
                MathHelpers.cpp
                )
target_compile_definitions( math_helpers PRIVATE PWX_EXPORTS )
target_compile_options( math_helpers PRIVATE -fvisibility=hidden )


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( math_helpers PRIVATE ${CMAKE_SOURCE_DIR}/src )


# ---------------------------------------------
# --- Installation of the container headers ---
# ---------------------------------------------
install( FILES
         ${math_helpers_HEADERS}
         DESTINATION include/pwxlib/math_helpers
         COMPONENT pwxlib
         )
