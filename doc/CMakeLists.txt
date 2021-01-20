find_package( Doxygen REQUIRED dot )
if ( DOXYGEN_FOUND )
	configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxyfile @ONLY )
	add_custom_target( doc
	                   ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxyfile
	                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	                   COMMENT "Generating documentation with Doxygen" VERBATIM
	                   )
	install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT pwxlib )
else ()
	message( "Doxygen with dot needs to be installed to generate the doxygen documentation" )
endif ()
