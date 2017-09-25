random_sources = files('''
        CRandomConstants.h
        CRandom.cpp
        CRandom.h
        CRandomTHash.cpp
        CRandomTHash.h
        CRandomTRandom.cpp
        CRandomTRandom.h
        CRandomWordConstants.h
        eNameSourceType.cpp
        eNameSourceType.h
'''.split())

random_headers = '''
        CRandom.h
        eNameSourceType.h
'''.split()

libpwx_headers += files(random_headers)

librandom = static_library(
        'random',
        random_sources,
        cpp_args : ['-DPWX_EXPORTS', '-fvisibility=default'],
        include_directories : includes,
        install : false)
