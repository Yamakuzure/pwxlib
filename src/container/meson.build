container_sources = files('''
        CHashBuilder.cpp
        CHashBuilder.h
        CThreadElementStore.cpp
        CThreadElementStore.h
        TChainHash.h
        TDoubleElement.h
        TDoubleList.h
        TDoubleRing.h
        THashElement.h
        TOpenHash.h
        TQueue.h
        TSet.h
        TSingleElement.h
        TSingleList.h
        TSingleRing.h
        TStack.h
        TVarDeleter.h
        VContainer.cpp
        VContainer.h
        VElement.cpp
        VElement.h
        VTHashBase.h
'''.split())

container_headers = '''
        TChainHash.h
        TDoubleElement.h
        TDoubleList.h
        TDoubleRing.h
        THashElement.h
        TOpenHash.h
        TQueue.h
        TSet.h
        TSingleElement.h
        TSingleList.h
        TSingleRing.h
        TStack.h
'''.split()

libpwx_headers += files(container_headers)

libcontainer = static_library(
	'container',
	container_sources, funcs_sources,
        include_directories : includes,
        link_with : [librandom],
        install : false)
