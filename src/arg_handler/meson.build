arg_sources = files('''
        CArgCallback.cpp
        CArgHandler.cpp
        sArgError.cpp
        TArgTarget.cpp
        VArgTargetBase.cpp
'''.split())

arg_headers = '''
        pwxCArgCallback.h
        pwxCArgHandler.h
        pwxTArgTarget.h
        pwxVArgTargetBase.h
        eArgErrorNumber.h
        pwx_eArgSetType.h
        pwx_eArgTargetType.h
        pwx_sArgError.h
'''.split()

libpwx_headers += files(arg_headers)

libarghandler = static_library(
        'arghandler',
        arg_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        install : false)
