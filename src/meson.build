subdir('basic')
subdir('functions')
subdir('math_helpers')
subdir('random')
subdir('stream_helpers')
subdir('container')
subdir('arg_handler')
subdir('wavecolor')
subdir('libpwx')

pwxlib_h = files('''
        pwxlib.h
'''.split())

arg_wrap    = 'PArgHandler'
basic_wrap  = '''
        PException
        PLockable
        PLockGuard
'''.split()
cont_wrap   = '''
        PChainHash
        PDoubleList
        PDoubleRing
        POpenHash
        PQueue
        PSet
        PSingleList
        PSingleRing
        PStack
'''.split()
lib_wrap    = '''
        PAH
        PBasic
        PContainers
        PTools
        PUtils
        PWXlib
        PWorkers
        RNG
        SCT
        pwxlib.h
'''.split()
math_wrap   = '''
        PMath
        PSinCos
'''.split()
rand_wrap   = 'PRandom'
stream_wrap = 'PStreamHelpers'
wave_wrap   = 'PWaveColor'

install_headers(
        arg_wrap,
        basic_wrap,
        cont_wrap,
        lib_wrap,
        math_wrap,
        rand_wrap,
        stream_wrap,
        wave_wrap,
        subdir : 'pwxlib')

libpwx_wrapper_list += files(
        arg_wrap,
        basic_wrap,
        cont_wrap,
        lib_wrap,
        math_wrap,
        rand_wrap,
        stream_wrap,
        wave_wrap)
