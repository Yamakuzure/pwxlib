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

libarg = static_library(
	'arg',
	arg_sources,
        include_directories : includes,
        dependencies : [threads],
        link_with : [libcontainer],
        install : false)
