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
        eArgSetType.h
        eArgTargetType.h
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
        subdir : 'pwxlib/pwx')

arg_wrapper = 'PArgHandler'
install_headers(
        arg_wrapper,
        subdir : 'pwxlib')

libpwx_wrapper_list += files(arg_wrapper)
