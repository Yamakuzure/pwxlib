math_helpers_sources = files('''
        CSinCosTable.cpp
        CSinCosTable.h
        MathHelpers.cpp
        MathHelpers.h
'''.split())

math_helpers_headers = '''
        CSinCosTable.h
        MathHelpers.h
'''.split()

libpwx_headers += files(math_helpers_headers)

libmath_helpers = static_library(
	'math_helpers',
	math_helpers_sources,
        include_directories : includes,
        link_with : [libbasic],
        install : false)
