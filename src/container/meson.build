container_sources = files('''
        CHashBuilder.cpp
        CThreadElementStore.cpp
        VContainer.cpp
        VElement.cpp
'''.split())

container_headers = '''
        CHashBuilder.h
        CThreadElementStore.h
        TChainHash.h
        TDoubleElement.h
        pwxTDoubleList.h
        pwxTDoubleRing.h
        pwxTHashElement.h
        pwxTOpenHash.h
        pwxTQueue.h
        pwxTSet.h
        pwxTSingleElement.h
        pwxTSingleList.h
        pwxTSingleRing.h
        pwxTStack.h
        pwxTVarDeleter.h
        pwxVContainer.h
        pwxVElement.h
        pwxVTHashBase.h
'''.split()

libpwx_headers += files(container_headers)

libcontainer = static_library(
        'container',
        container_sources,
	funcs_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        install : false)
