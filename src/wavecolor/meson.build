wavecolor_sources = files('''
        CWaveColor.cpp
        CWaveColor.h
'''.split())

libwavecolor = static_library(
	'wavecolor',
	wavecolor_sources,
        include_directories : includes,
        link_with : [libcontainer],
        install : false)
