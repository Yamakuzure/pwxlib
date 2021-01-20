# ------------------------------------------------------------------------
# --- Test programs, all in one file.                                  ---
# ------------------------------------------------------------------------
if ( WITH_TESTS )
	set( debug_arg 0 )

	if ( CMAKE_BUILD_TYPE STREQUAL Debug )
		set( debug_arg 1 )
	endif ()

	add_custom_command( OUTPUT test-libpwx-sym.cpp
	                    COMMAND ${CMAKE_SOURCE_DIR}/test/generate-sym-test.py
	                    ${libpwx_SYM} ${debug_arg} ${pwxlib_h} > test-libpwx-sym.cpp
	                    MAIN_DEPENDENCY ${libpwx_SYM}
	                    DEPENDS ${pwxlib_h}
	                    COMMENT Generate a test for all API functions, calsses and structures.
	                    VERBATIM USES_TERMINAL
	                    )

	add_executable( test-libpwx-sym
	                test-libpwx-sym.cpp
	                ${pwxlib_h}
	                )
	target_include_directories( test-libpwx-sym PRIVATE ${CMAKE_SOURCE_DIR}/src )
	target_link_libraries( test-libpwx-sym PRIVATE pwx )
	add_test( test-libpwx-sym COMMAND test-libpwx-sym )

	add_executable( test_cluster
	                cluster_check.cpp
	                cluster_check.h
	                )
	target_include_directories( test_cluster PRIVATE ${CMAKE_SOURCE_DIR}/src )
	target_link_libraries( test_cluster PRIVATE pwx )
	add_test( test_cluster COMMAND test_cluster )

	add_executable( test_hash
	                hash_builder.cpp
	                hash_builder.h
	                )
	target_include_directories( test_hash PRIVATE ${CMAKE_SOURCE_DIR}/src )
	target_link_libraries( test_hash PRIVATE pwx )

	add_executable( test_name
	                namegen.cpp
	                )
	target_include_directories( test_name PRIVATE ${CMAKE_SOURCE_DIR}/src )
	target_link_libraries( test_name PRIVATE pwx )

	add_executable( test_lib
	                destroy.cpp
	                destroy.h
	                env.h
	                test_lib.cpp
	                test_lib.h
	                testColor.cpp
	                testColor.h
	                testHash.h
	                testListRing.h
	                testPAH.cpp
	                testPAH.h
	                testRNG.cpp
	                testRNG.h
	                testRNG_func.cpp
	                testRNG_func.h
	                testSCT.cpp
	                testSCT.h
	                testSet.h
	                testSpeed.h
	                testStackQueue.h
	                testThreads.h
	                )
	target_include_directories( test_lib PRIVATE ${CMAKE_SOURCE_DIR}/src )
	target_link_libraries( test_lib PRIVATE pwx )

	if ( ENABLE_TORTURE )
		add_executable( torture
		                torture.cpp
		                torture.h
		                )
		target_include_directories( torture PRIVATE ${CMAKE_SOURCE_DIR}/src )
		target_link_libraries( torture PRIVATE pwx )
	endif ()

	if ( ENABLE_INSTALL_TESTS )
		# Prefix with pwx_ when installing
		set_target_properties( test_cluster PROPERTIES OUTPUT_NAME pwx_test_cluster )
		set_target_properties( test_hash PROPERTIES OUTPUT_NAME pwx_test_hash )
		set_target_properties( test_lib PROPERTIES OUTPUT_NAME pwx_test_lib )
		set_target_properties( test_name PROPERTIES OUTPUT_NAME pwx_test_name )

		# Installations just moves to the bin subfolder
		install( TARGETS test_cluster DESTINATION bin COMPONENT pwx )
		install( TARGETS test_hash DESTINATION bin COMPONENT pwx )
		install( TARGETS test_lib DESTINATION bin COMPONENT pwx )
		install( TARGETS test_name DESTINATION bin COMPONENT pwx )

		if ( ENABLE_TORTURE )
			set_target_properties( torture PROPERTIES OUTPUT_NAME pwx_torture )
			install( TARGETS torture DESTINATION bin COMPONENT pwx )
		endif ()
	endif ()
endif ()
