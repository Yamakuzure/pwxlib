arg_sources = files('''
        CArgCallback.cpp
        CArgCallback.h
        CArgHandler.cpp
        CArgHandler.h
        eArgErrorNumber.h
        eArgSetType.h
        eArgTargetType.h
        sArgError.cpp
        sArgError.h
        TArgTarget.cpp
        TArgTarget.h
        VArgTargetBase.cpp
        VArgTargetBase.h
'''.split())

arg_headers = '''
        CArgCallback.h
        CArgHandler.h
        sArgError.h
        TArgTarget.h
'''.split()

libpwx_headers += files(arg_headers)

libarg = static_library(
	'arg',
	arg_sources,
        include_directories : includes,
        dependencies : [threads],
        link_with : [libcontainer],
        install : false)
