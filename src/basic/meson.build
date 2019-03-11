basic_sources = files('''
        CException.cpp
        CLockable.cpp
        CLockGuard.cpp
        debug.cpp
'''.split())

_basic_headers = '''
        CException.h
        CLockable.h
        pwxCLockGuard.h
        pwx_compiler.h
        pwx_debug.h
        pwx_macros.h
        pwx_templates.h
        pwx_types.h
'''.split()

libpwx_headers += files(_basic_headers)

libbasic = static_library(
        'basic',
        basic_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        dependencies : [ threads ],
        install : false)
