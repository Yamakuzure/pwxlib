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
        pwxCRandom.h
        eNameSourceType.h
'''.split()

libpwx_headers += files(random_headers)

librandom = static_library(
        'random',
        random_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        install : false)
