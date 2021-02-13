# -----------------------------------------------
# --- Sources for the "log" component         ---
# -----------------------------------------------

set( log_HEADERS
     ${CMAKE_CURRENT_LIST_DIR}/log.h
     ${CMAKE_CURRENT_LIST_DIR}/log_level.h
     )

target_sources( log PRIVATE
                ${log_HEADERS}
                _log_msg_id.cpp
                _log_msg_id.h
                _log_queue.cpp
                _log_queue.h
                _log_thread.h
                log.cpp
                )

target_compile_definitions( log PRIVATE PWX_EXPORTS )
target_compile_options( log PRIVATE -fvisibility=hidden )


# ------------------------------------------------
# --- Add source directory to the include list ---
# ------------------------------------------------
target_include_directories( log PRIVATE ${CMAKE_SOURCE_DIR}/src )


# ---------------------------------------------
# --- Installation of the container headers ---
# ---------------------------------------------
install( FILES
         ${log_HEADERS}
         DESTINATION include/pwxlib/log
         COMPONENT pwxlib
         )
