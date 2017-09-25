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
        templates.h
        types.h
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
        cpp_args : ['-DPWX_EXPORTS', '-fvisibility=default'],
        include_directories : includes,
        dependencies : [ threads ],
        install : false)
