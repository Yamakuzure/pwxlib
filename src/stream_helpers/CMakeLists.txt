# --------------------------------------------------
# --- Sources for the "stream_helpers" component ---
# --------------------------------------------------

set( stream_helpers_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/StreamHelpers.h
     )

target_sources( stream_helpers PRIVATE
                ${stream_helpers_HEADERS}
                StreamHelpers.cpp
                )
target_compile_definitions(stream_helpers PRIVATE PWX_EXPORTS)


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( stream_helpers PRIVATE ${CMAKE_SOURCE_DIR}/src )


# ---------------------------------------------
# --- Installation of the container headers ---
# ---------------------------------------------
install( FILES
         ${stream_helpers_HEADERS}
         DESTINATION include/pwxlib/stream_helpers
         COMPONENT pwxlib
         )
