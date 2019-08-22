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
	VContainer.h
	VElement.h
	VTHashBase.h
'''.split()

libcontainer = static_library(
	'container',
	container_sources,
	funcs_sources,
	cpp_args : ['-DPWX_EXPORTS'],
	include_directories : includes,
	install : false)

install_headers(
	container_headers,
	subdir : 'pwxlib/container')
