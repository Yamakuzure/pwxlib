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
        include_directories : includes,
        link_with : [libmath_helpers],
        install : false)
