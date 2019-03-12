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

libpwx_headers += files(arg_headers)

libarghandler = static_library(
        'arghandler',
        arg_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        install : false)
