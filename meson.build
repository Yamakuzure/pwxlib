project('pwxlib', 'cpp',
        version : '0.8.9',
        license : 'MIT',
        default_options: [
                'cpp_std=c++17',
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

pkgconfigdatadir = join_paths(datadir, 'pkgconfig')
pkgconfiglibdir = join_paths(libdir, 'pkgconfig')
pkgincludedir = join_paths(includedir, 'pwxlib')

docdir = '-'.join([join_paths(datadir, 'doc/pwxLib'),
                             meson.project_version()])

#####################################################################

substs = configuration_data()
substs.set('PACKAGE_NAME',    meson.project_name())
substs.set('PACKAGE_URL',     'https://prydeworx.com/pwxlib')
substs.set('PACKAGE_VERSION', meson.project_version())
substs.set('prefix',          prefixdir)
substs.set('exec_prefix',     prefixdir)
substs.set('libdir',          libdir)
substs.set('includedir',      includedir)

#####################################################################

cxx                  = meson.get_compiler('cpp')
cxx_id               = cxx.get_id()
pkgconfig            = import('pkgconfig')
check_compilation_sh = find_program('meson/check_compilation.sh')
buildtype            = get_option('buildtype')

############################################################

# Set debug options
annotations = get_option('annotations')
debugextra  = get_option('debug-extra')
debugthread = get_option('debug-thread')
graphite    = get_option('graphite')
profile     = get_option('profile')
spinlocks   = get_option('spinlocks')
yielding    = get_option('yielding')

if buildtype != 'debug'
        annotations = false
        debugextra  = false
        debugthread = false
endif

if profile
        annotations = false
        debugextra  = false
        debugthread = false
        add_project_arguments('-pg', language : 'cpp')
        add_project_link_arguments('-pg', language : 'cpp')
endif

if annotations
        debugextra = true
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
        debugextra = true
        conf.set('PWX_THREADDEBUG', true)
endif

#####################################################################

possible_cxx_flags = [
        '-Wextra',
        '-pedantic',
        '-Wpedantic',
        '-Werror=missing-prototypes',
        '-Werror=implicit-function-declaration',
        '-Werror=missing-declarations',
        '-Werror=return-type',
        '-Werror=incompatible-pointer-types',
        '-Werror=format=2',
        '-fdiagnostics-show-option',
        '-fstack-protector',
        '-fstack-protector-strong',
        '-fPIC',
        '-fPIE',
        '-fexceptions',
	    '-fvisibility=hidden'
]

# --as-needed and --no-undefined are provided by meson by default,
# run mesonconf to see what is enabled
possible_link_flags = [
        '-Wl,-z,relro',
        '-Wl,-z,now',
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

if debugextra
        conf.set('LIBPWX_DEBUG', true)
        possible_cxx_flags += [
                '-ggdb3',
                '-Og'
        ]
        possible_link_flags += [
                '-ggdb3',
                '-Og'
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
        src/arg_handler
        src/basic
        src/container
        src/functions
        src/libpwx
        src/math_helpers
        src/random
        src/stream_helpers
        src/wavecolor
'''.split()
includes = include_directories(include_paths)

add_project_arguments('-include', 'config.h', language : 'cpp')

libpwx_headers           = []
libpwx_all_in_one_header = []
libpwx_internal_sources  = []
libpwx_external_sources  = []

subdir('src/basic')
subdir('src/functions')
subdir('src/math_helpers')
subdir('src/random')
subdir('src/stream_helpers')
subdir('src/container')
subdir('src/arg_handler')
subdir('src/wavecolor')
subdir('src/libpwx')

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

install_headers(
        libpwx_all_in_one_header)
install_headers(
        libpwx_headers,
        subdir : 'pwxlib')

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
        link_with : [libpwx],
        install : false)
test('test-libpwx-sym',
     test_libpwx_sym)

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

        'prefix:                            @0@'.format(prefixdir),
        'includedir:                        @0@'.format(pkgincludedir),
        'lib dir:                           @0@'.format(libdir),
        'bin dir:                           @0@'.format(bindir),
        'data dir:                          @0@'.format(datadir)]

message('\n         '.join(status))
