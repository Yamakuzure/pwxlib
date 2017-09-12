basic_sources = files('''
        CException.cpp
        CException.h
        CLockable.cpp
        CLockable.h
        CLockGuard.cpp
        CLockGuard.h
        compiler.h
        debug.cpp
        debug.h
        macros.h
        StreamHelpers.cpp
        StreamHelpers.h
        templates.h
        types.h
'''.split())

libbasic = static_library(
	'basic',
	basic_sources,
        include_directories : includes,
        dependencies : [threads],
        install : false)
