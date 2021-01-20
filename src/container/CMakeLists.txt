# -------------------------------------------------
# --- Sources for the "container" component     ---
# -------------------------------------------------

set( container_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/CHashBuilder.h
     ${CMAKE_CURRENT_LIST_DIR}/CThreadElementStore.h
     ${CMAKE_CURRENT_LIST_DIR}/TChainHash.h
     ${CMAKE_CURRENT_LIST_DIR}/TDoubleElement.h
     ${CMAKE_CURRENT_LIST_DIR}/TDoubleList.h
     ${CMAKE_CURRENT_LIST_DIR}/TDoubleRing.h
     ${CMAKE_CURRENT_LIST_DIR}/THashElement.h
     ${CMAKE_CURRENT_LIST_DIR}/TOpenHash.h
     ${CMAKE_CURRENT_LIST_DIR}/TQueue.h
     ${CMAKE_CURRENT_LIST_DIR}/TSet.h
     ${CMAKE_CURRENT_LIST_DIR}/TSingleElement.h
     ${CMAKE_CURRENT_LIST_DIR}/TSingleList.h
     ${CMAKE_CURRENT_LIST_DIR}/TSingleRing.h
     ${CMAKE_CURRENT_LIST_DIR}/TStack.h
     ${CMAKE_CURRENT_LIST_DIR}/TVarDeleter.h
     ${CMAKE_CURRENT_LIST_DIR}/VContainer.cpp
     ${CMAKE_CURRENT_LIST_DIR}/VContainer.h
     ${CMAKE_CURRENT_LIST_DIR}/VElement.cpp
     ${CMAKE_CURRENT_LIST_DIR}/VElement.h
     ${CMAKE_CURRENT_LIST_DIR}/VTHashBase.h
     )

target_sources( container PRIVATE
                ${container_HEADERS}
                CHashBuilder.cpp
                CThreadElementStore.cpp
                )
target_compile_definitions( container PRIVATE PWX_EXPORTS )
target_compile_options( container PRIVATE -fvisibility=hidden )


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( container PRIVATE ${CMAKE_SOURCE_DIR}/src )


# ---------------------------------------------
# --- Installation of the container headers ---
# ---------------------------------------------
install( FILES
         ${container_HEADERS}
         DESTINATION include/pwxlib/container
         COMPONENT pwxlib
         )
