basic_sources = files('''
        CException.cpp
        CLockable.cpp
        CLockGuard.cpp
        debug.cpp
'''.split())

basic_headers = '''
        CException.h
        CLockable.h
        CLockGuard.h
        pwx_compiler.h
        pwx_debug.h
        pwx_macros.h
        pwx_templates.h
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

basic_wrappers = '''
        PException
        PLockable
        PLockGuard
'''.split()
install_headers(
        basic_wrappers,
        subdir : 'pwxlib')

libpwx_wrapper_list += files(basic_wrappers)
