random_sources = files('''
        CRandomConstants.h
        CRandom.cpp
        CRandomTHash.cpp
        CRandomTHash.h
        CRandomTRandom.cpp
        CRandomTRandom.h
        CRandomWordConstants.h
        eNameSourceType.cpp
'''.split())

random_headers = '''
        CRandom.h
        eNameSourceType.h
'''.split()

librandom = static_library(
        'random',
        random_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        install : false)

install_headers(
        random_headers,
        subdir : 'pwxlib/pwx')

random_wrappers = '''
        PRandom
'''.split()
install_headers(
        random_wrappers,
        subdir : 'pwxlib')

libpwx_wrapper_list += files(random_wrappers)
