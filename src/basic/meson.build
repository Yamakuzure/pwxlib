basic_sources = files('''
	CException.cpp
	CLockable.cpp
	CLockGuard.cpp
	debug.cpp
	trace_info.cpp
'''.split())

basic_headers = '''
	CException.h
	CLockable.h
	CLockGuard.h
	pwx_compiler.h
	pwx_debug.h
	pwx_macros.h
	pwx_templates.h
	pwx_trace_info.h
	pwx_types.h
'''.split()

libbasic = static_library(
	'basic',
	basic_sources,
	cpp_args : ['-DPWX_EXPORTS'],
	include_directories : includes,
	dependencies : [ threads ],
	install : false)

install_headers(
	basic_headers,
	subdir : 'pwxlib/basic')
