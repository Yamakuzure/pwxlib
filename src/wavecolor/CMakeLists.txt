# -------------------------------------------------
# --- Sources for the "wavecolor" component ---
# -------------------------------------------------

set( wavecolor_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/CWaveColor.h
     )

target_sources( wavecolor PRIVATE
                ${wavecolor_HEADERS}
                CWaveColor.cpp
                )
target_compile_definitions( wavecolor PRIVATE PWX_EXPORTS )
target_compile_options( wavecolor PRIVATE -fvisibility=hidden )


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( wavecolor PRIVATE ${CMAKE_SOURCE_DIR}/src )


# ---------------------------------------------
# --- Installation of the container headers ---
# ---------------------------------------------
install( FILES
         ${wavecolor_HEADERS}
         DESTINATION include/pwxlib/wavecolor
         COMPONENT pwxlib
         )
