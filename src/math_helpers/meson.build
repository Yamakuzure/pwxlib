mathhelpers_sources = files('''
	CSinCosTable.cpp
	MathHelpers.cpp
'''.split())

mathhelpers_headers = '''
	CSinCosTable.h
	MathHelpers.h
'''.split()

libmathhelpers = static_library(
	'mathhelpers',
	mathhelpers_sources,
	mathhelpers_headers,
	cpp_args : ['-DPWX_EXPORTS'],
	include_directories : includes,
	install : false)

install_headers(
	mathhelpers_headers,
	subdir : 'pwxlib/math_helpers')
