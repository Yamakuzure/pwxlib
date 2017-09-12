wavecolor_sources = files('''
        CWaveColor.cpp
        CWaveColor.h
'''.split())

wavecolor_headers = '''
        CWaveColor.h
'''.split()

libpwx_headers += files(wavecolor_headers)

libwavecolor = static_library(
	'wavecolor',
	wavecolor_sources,
        include_directories : includes,
        link_with : [libcontainer],
        install : false)
