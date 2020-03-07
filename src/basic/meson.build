basic_sources = files('''
        _mem_map.cpp
        _mem_map.h
	CException.cpp
	CLockable.cpp
	CLockGuard.cpp
	debug.cpp
	mem_utils.cpp
	trace_info.cpp
'''.split())

basic_headers = '''
	alloc_utils.h
	CException.h
	CLockable.h
	CLockGuard.h
	mem_utils.h
	pwx_compiler.h
	pwx_debug.h
	pwx_macros.h
	pwx_templates.h
	pwx_trace_info.h
	pwx_types.h
	string_utils.h
'''.split()

libbasic = static_library(
	'basic',
	basic_sources,
	basic_headers,
	cpp_args : ['-DPWX_EXPORTS'],
	include_directories : includes,
	dependencies : [ threads ],
	install : false)

install_headers(
	basic_headers,
	subdir : 'pwxlib/basic')
