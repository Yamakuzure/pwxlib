# -------------------------------------------------
# --- Sources for the "random" component        ---
# -------------------------------------------------

set( random_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/CRandom.h
     ${CMAKE_CURRENT_LIST_DIR}/CRandomConstants.h
     ${CMAKE_CURRENT_LIST_DIR}/CRandomTHash.h
     ${CMAKE_CURRENT_LIST_DIR}/CRandomTRandom.h
     ${CMAKE_CURRENT_LIST_DIR}/CRandomWordConstants.h
     ${CMAKE_CURRENT_LIST_DIR}/eNameSourceType.h
     )

target_sources( random PRIVATE
                ${random_HEADERS}
                CRandom.cpp
                CRandomTHash.cpp
                CRandomTRandom.cpp
                eNameSourceType.cpp
                )
target_compile_definitions( random PRIVATE PWX_EXPORTS )
target_compile_options( random PRIVATE -fvisibility=hidden )


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( random PRIVATE ${CMAKE_SOURCE_DIR}/src )


# ---------------------------------------------
# --- Installation of the container headers ---
# ---------------------------------------------
install( FILES
         ${random_HEADERS}
         DESTINATION include/pwxlib/random
         COMPONENT pwxlib
         )
