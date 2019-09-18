project('pwxlib', 'cpp',
	version : '0.8.9',
	license : 'MIT',
	default_options: [
		'cpp_std=gnu++17',
		'prefix=/usr',
	],
	meson_version : '>= 0.48',
       )

conf = configuration_data()
conf.set_quoted('PACKAGE_STRING',  meson.project_name() + ' ' + meson.project_version())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('PACKAGE_URL',     'https://github.com/Yamakuzure/pwxlib')
m4_defines = []

# Set general directories
prefixdir = get_option('prefix')
if not prefixdir.startswith('/')
	error('Prefix is not absolute: "@0@"'.format(prefixdir))
endif
bindir = join_paths(prefixdir, get_option('bindir'))
libdir = join_paths(prefixdir, get_option('libdir'))
testsdir = join_paths(libdir, 'pwx')

includedir = join_paths(prefixdir, get_option('includedir'))
datadir = join_paths(prefixdir, get_option('datadir'))

pkgconfiglibdir = get_option('pkgconfiglibdir') == '' ? join_paths(libdir, 'pkgconfig') : get_option('pkgconfiglibdir')
pkgincludedir = join_paths(includedir, 'pwxlib')
pkgdatadir = join_paths(datadir, 'pwxlib')

docdir = '-'.join([join_paths(datadir, 'doc/pwxLib'),
			     meson.project_version()])

#####################################################################

substs = configuration_data()
substs.set('PACKAGE_NAME',    meson.project_name())
substs.set('PACKAGE_URL',     'https://prydeworx.com/pwxlib')
substs.set('PACKAGE_VERSION', meson.project_version())
substs.set('prefix',	  prefixdir)
substs.set('exec_prefix',     prefixdir)
substs.set('libdir',	  libdir)
substs.set('includedir',      includedir)

#####################################################################

cxx		     = meson.get_compiler('cpp')
cxx_id	             = cxx.get_id()
pkgconfig	     = import('pkgconfig')
check_compilation_sh = find_program('meson/check_compilation.sh')
buildtype	     = get_option('buildtype')
sanitize	     = get_option('b_sanitize')

############################################################

# Set debug options
annotations = get_option('annotations')
debug       = get_option('debug')
debugthread = get_option('debug-thread')
profile     = get_option('profile')
small_tests = get_option('small_tests')
spinlocks   = get_option('spinlocks')
yielding    = get_option('yielding')

if buildtype != 'debug'
	annotations = false
	debug       = false
	debugthread = false
endif

if profile
	annotations = false
	debug       = false
	debugthread = false
	add_project_arguments('-pg', language : 'cpp')
	add_project_link_arguments('-pg', language : 'cpp')
endif

if annotations
	debug = true
	conf.set('PWX_ANNOTATIONS', true)
else
	if yielding
		conf.set('PWX_USE_FLAGSPIN_YIELD', true)
		spinlocks = true
	endif
	if spinlocks
		conf.set('PWX_USE_FLAGSPIN', true)
	endif
endif

if debugthread
	debug = true
	conf.set('PWX_THREADDEBUG', true)
endif

if small_tests
	conf.set('PWX_SMALL_TESTS', 1)
else
	conf.set('PWX_SMALL_TESTS', 0)
endif

#####################################################################

possible_cxx_flags = [
	'-Wextra',
	'-pedantic',
	'-Wpedantic',
	'-Werror=missing-declarations',
	'-Werror=return-type',
	'-Werror=format=2',
	'-fdiagnostics-show-option',
	'-fstack-protector',
	'-fstack-protector-strong',
	'-pic',
	'-fPIC',
	'-fexceptions',
	'-fvisibility=hidden'
]

# --as-needed and --no-undefined are provided by meson by default,
# run mesonconf to see what is enabled
possible_link_flags = [
	'-Wl,-z,relro',
	'-Wl,-z,now',
	'-pie',
	'-fPIE',
]

if cxx.get_id() == 'clang'
	possible_cxx_flags += [
		'-Wno-typedef-redefinition',
		'-Wno-gnu-variable-sized-type-not-at-end',
	]
endif

if get_option('buildtype') != 'debug'
	possible_cxx_flags += [
		'-ffunction-sections',
		'-fdata-sections',
	]

	possible_link_flags += '-Wl,--gc-sections'
endif

if debug
	conf.set('LIBPWX_DEBUG', true)
	possible_cxx_flags += [
		'-ggdb3',
		'-O0'
	]
	possible_link_flags += [
		'-ggdb3',
		'-O0'
	]
endif

add_project_arguments(cxx.get_supported_arguments(possible_cxx_flags), language : 'cpp')

# "negative" arguments: gcc on purpose does not return an error for "-Wno-"
# arguments, just emits a warnings. So test for the "positive" version instead.
foreach arg : ['error=nonnull', # work-around for gcc 7.1 turning this on on its own

	       # Disable -Wmaybe-uninitialized, since it's noisy on gcc 8 with
	       # optimizations enabled, producing essentially false positives.
	       'maybe-uninitialized',
	      ]
	if cxx.has_argument('-W' + arg)
		add_project_arguments('-Wno-' + arg, language : 'cpp')
	endif
endforeach

if cxx.compiles('''
   #include <ctime>
   #include <cinttypes>
   typedef uint64_t usec_t;
   usec_t now(clockid_t clock);
   int main(void) {
	   struct timespec now;
	   return 0;
   }
''', name : '-Werror=shadow with local shadowing')
	add_project_arguments('-Werror=shadow', language : 'cpp')
endif

link_test_cxx = files('meson/link_test.cpp')

foreach arg : possible_link_flags
	have = run_command(check_compilation_sh,
			   cxx.cmd_array(), '-x', 'c++', arg,
			   '-include', link_test_cxx).returncode() == 0
	message('Linking with @0@ supported: @1@'.format(arg, have ? 'yes' : 'no'))
	if have
		add_project_link_arguments(arg, language : 'cpp')
	endif
endforeach

#####################################################################
# compilation result tests

conf.set('__SANE_USERSPACE_TYPES__', true)

decl_headers = '''
#include <cuchar>
'''
foreach decl : ['char16_t',
		'char32_t',
	       ]

	# We get -1 if the size cannot be determined
	have = cxx.sizeof(decl, prefix : decl_headers) > 0
	conf.set('HAVE_' + decl.underscorify().to_upper(), have)
endforeach

#####################################################################

find_src_sh = find_program('meson/find_source.sh')
mkdir_p     = 'mkdir -p $DESTDIR/@0@'

############################################################

threads = dependency('threads')

#####################################################################

tests = []

#####################################################################

config_h = configure_file(
	output : 'config.h',
	configuration : conf)

include_paths = '''
	src
'''.split()
includes = include_directories(include_paths)

add_project_arguments('-include', 'config.h', language : 'cpp')

libpwx_internal_sources  = []
libpwx_external_sources  = []
libpwx_wrapper_list      = []

subdir('src')

libpwx_sym_path = '@0@/@1@'.format(meson.current_source_dir(), libpwx_sym)
libpwx = shared_library(
	'pwx',
	libpwx_internal_sources,
	libpwx_external_sources,
	version : meson.project_version(),
	include_directories : includes,
	cpp_args : ['-DPWX_EXPORTS'],
	link_args : ['-shared',
		     '-Wl,--version-script=' + libpwx_sym_path],
	dependencies : [threads],
	link_with : [
		libarghandler,
		libbasic,
		libcontainer,
		libmathhelpers,
		librandom
	],
	link_depends : [ libpwx_sym ],
	install : true,
	install_dir : libdir)

############################################################

# binaries that have --help and are intended for use by humans.
public_programs = []

subdir('test')
subdir('doc')

############################################################

test_libpwx_sym = executable(
	'test-libpwx-sym',
	test_libpwx_sym_cpp,
	include_directories : includes,
	link_with : libpwx,
	install : false)
test('test-libpwx-sym',
	test_libpwx_sym,
	env : test_env,
	timeout : 30)

############################################################

foreach tuple : tests
	name    = tuple[0]
	sources = tuple[1]
	type    = tuple[2]
	deps    = tuple[3]

	exe = executable(
		name,
		sources,
		dependencies : deps,
		include_directories : includes,
		link_with : libpwx,
		install_rpath : libdir,
		install : install_tests,
		install_dir : testsdir)

	if type == ['manual']
		message('@0@ is a manual test'.format(name))
	else
		test(name, exe,
			env : test_env,
			timeout : 30)
	endif
endforeach


############################################################

install_data('AUTHORS',
	     'ChangeLog',
	     'code_of_conduct.md',
	     'CONTRIBUTING.md',
	     'INSTALL.md',
	     'LICENSE',
	     'NEWS.md',
	     'README.md',
	     'TODO.md',
	     install_dir : docdir)

############################################################

status = [
	'@0@ @1@'.format(meson.project_name(), meson.project_version()),

	'build type:  @0@'.format(buildtype),
	'prefix:      @0@'.format(prefixdir),
	'includedir:  @0@'.format(pkgincludedir),
	'lib dir:     @0@'.format(libdir),
	'bin dir:     @0@'.format(bindir),
	'data dir:    @0@'.format(datadir),
	'sanitizer:   @0@'.format(sanitize)]

message('\n	 '.join(status))
