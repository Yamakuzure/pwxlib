# -------------------------------------------------
# --- Sources for the "libpwx" component        ---
# -------------------------------------------------

set( libpwx_SYM ${CMAKE_CURRENT_LIST_DIR}/pwxlib.sym PARENT_SCOPE)

set( libpwx_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/basic.h
     ${CMAKE_CURRENT_LIST_DIR}/containers.h
     ${CMAKE_CURRENT_LIST_DIR}/finish.h
     ${CMAKE_CURRENT_LIST_DIR}/init.h
     ${CMAKE_CURRENT_LIST_DIR}/tools.h
     ${CMAKE_CURRENT_LIST_DIR}/utils.h
     ${CMAKE_CURRENT_LIST_DIR}/worker_PAH.h
     ${CMAKE_CURRENT_LIST_DIR}/worker_RNG.h
     ${CMAKE_CURRENT_LIST_DIR}/worker_SCT.h
     ${CMAKE_CURRENT_LIST_DIR}/workers.h
     )

target_sources( libpwx PRIVATE
                ${libpwx_HEADERS}
                finish.cpp
                init.cpp
                worker_PAH.cpp
                worker_RNG.cpp
                worker_SCT.cpp
                )

# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( libpwx PRIVATE ${CMAKE_SOURCE_DIR}/src )


# ---------------------------------------------
# --- Installation of the container headers ---
# ---------------------------------------------
install( FILES
         ${libpwx_HEADERS}
         DESTINATION include/pwxlib/libpwx
         COMPONENT pwxlib
         )
