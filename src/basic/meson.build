basic_sources = files('''
        CException.cpp
        CLockable.cpp
        CLockGuard.cpp
        debug.cpp
'''.split())

_basic_headers = '''
        CException.h
        CLockable.h
        CLockGuard.h
        compiler.h
        debug.h
        macros.h
        templates.h
        types.h
'''.split()

libpwx_headers += files(_basic_headers)

libbasic = static_library(
        'basic',
        basic_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        dependencies : [ threads ],
        install : false)
