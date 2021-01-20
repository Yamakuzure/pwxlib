# -------------------------------------------------
# ---  Sources for the "arg_handler" component  ---
# -------------------------------------------------

set( arg_handler_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/CArgHandler.h
     ${CMAKE_CURRENT_LIST_DIR}/TArgTarget.h
     ${CMAKE_CURRENT_LIST_DIR}/VArgTargetBase.h
     ${CMAKE_CURRENT_LIST_DIR}/eArgErrorNumber.h
     ${CMAKE_CURRENT_LIST_DIR}/eArgTargetType.h
     ${CMAKE_CURRENT_LIST_DIR}/eArgType.h
     ${CMAKE_CURRENT_LIST_DIR}/sArgError.h
     )

target_sources( arg_handler PRIVATE
                ${arg_handler_HEADERS}
                CArgHandler.cpp
                TArgTarget.cpp
                VArgTargetBase.cpp
                sArgError.cpp
                )
target_compile_definitions( arg_handler PRIVATE PWX_EXPORTS )
target_compile_options( arg_handler PRIVATE -fvisibility=hidden )


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( arg_handler PRIVATE ${CMAKE_SOURCE_DIR}/src )


# -----------------------------------------------
# --- Installation of the arg_handler headers ---
# -----------------------------------------------
install( FILES
         ${arg_handler_HEADERS}
         DESTINATION include/pwxlib/arg_handler
         COMPONENT pwxlib
         )
