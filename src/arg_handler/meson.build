arg_sources = files('''
	CArgCallback.cpp
	CArgHandler.cpp
	sArgError.cpp
	TArgTarget.cpp
	VArgTargetBase.cpp
'''.split())

arg_headers = '''
	CArgCallback.h
	CArgHandler.h
	TArgTarget.h
	VArgTargetBase.h
	eArgErrorNumber.h
	eArgTargetType.h
	eArgType.h
	sArgError.h
'''.split()

libarghandler = static_library(
	'arghandler',
	arg_sources,
	cpp_args : ['-DPWX_EXPORTS'],
	include_directories : includes,
	install : false)

install_headers(
	arg_headers,
	subdir : 'pwxlib/arg_handler')
