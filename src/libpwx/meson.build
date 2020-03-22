libpwx_internal_sources = files('''
	../pwxlib.h
	finish.cpp
	init.cpp
	worker_PAH.cpp
	worker_RNG.cpp
	worker_SCT.cpp
'''.split())

libpwx_headers = '''
	basic.h
	containers.h
	finish.h
	init.h
	tools.h
	utils.h
	worker_PAH.h
	worker_RNG.h
	worker_SCT.h
	workers.h
'''.split()

libpwx_sym = 'src/libpwx/pwxlib.sym'

libpwx_pc = configure_file(
	input : 'pwxlib.pc.in',
	output : 'pwxlib.pc',
	configuration : substs)
install_data(libpwx_pc,
	     install_dir : pkgconfiglibdir)

install_headers(
	libpwx_headers,
	subdir : 'pwxlib/libpwx')
