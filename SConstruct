import os
import sys

sys.tracebacklimit = 0

# Try to detect the host platform automatically.
# This is used if no `platform` argument is passed
if sys.platform.startswith('linux'):
    host_platform = 'linux'
elif sys.platform == 'darwin':
    host_platform = 'osx'
elif sys.platform == 'win32' or sys.platform == 'msys':
    host_platform = 'windows'
else:
    raise ValueError(
        'Could not detect host platform automatically, please specify with '
        'host_platform=<platform>'
    )

options = Variables([], ARGUMENTS)
options.Add(EnumVariable(
    key='platform',
    help='Target Platform',
    default='none',
    allowed_values=['none', 'linux', 'android'],  # , 'windows'],
    ignorecase=2
))
options.Add(EnumVariable(
    key='bits',
    help='Target platform bits',
    default='64',
    allowed_values=['32', '64']
))
options.Add(PathVariable(
    key='gd_library_dir',
    help='Path to .a library generating from godot-cpp bindings.',
    default=None,
    validator=PathVariable.PathIsDir
))
options.Add(PathVariable(
    key='gd_library_name',
    help="""Name of the .a library generating from godot-cpp headers. By
     default it uses name which is used in godot main SConstruct.""",
    default=None,
    validator=PathVariable.PathIsFile
))
options.Add(EnumVariable(
    key='android_arch',
    help='Target android architecture',
    default='armv7',
    allowed_values=['armv7', 'arm64v8', 'x86', 'x86_64']
))
options.Add(PathVariable(
    key='ANDROID_NDK_ROOT',
    help="""Path to your Android NDK installation. By default, uses
        ANDROID_NDK_ROOT from your environment variables.""",
    default=os.environ.get('ANDROID_NDK_ROOT', None),
    validator=PathVariable.PathIsDir
))
options.Add(EnumVariable(
    key='target',
    help='release or debug target',
    default='release',
    allowed_values=['release', 'debug']
))
options.Add(
    key='android_api_level',
    help='Target Android API level',
    default='18' if ARGUMENTS.get("android_arch", 'armv7') in [
        'armv7', 'x86'] else '21'
)
options.Add(PathVariable(
    key='gd_headers_dir',
    help='Path to godot includes.',
    default='/usr/local/include/godot/',
    validator=PathVariable.PathIsDir
))
options.Add(
	key='add_includes',
	help='Additional include directories to be added to compiler option. '
    'Separate multiple values by <:>',
	default=''
)
options.Add(
    key='output_dir',
    help='Path to desired out put foldel.',
    default='Bin/'
)

VariantDir('build/obj', 'src', duplicate=0)
env = Environment(CXXFLAGS="-std=c++17")
options.Update(env=env)
Help(options.GenerateHelpText(env))


if env['platform'] == 'none':
    raise ValueError("""\n\tYou must specify target platform. Specify it by 
        platform=<platform>. <platform> can only be 'linux', 'android'.""")

if 'gd_library_dir' not in env:
    raise ValueError("""\n\tYou must specify the path to the directory containing 
        godot .a library. Specify it by gd_library_dir=<dir>""")

if 'gd_library_name' not in env:
    arch_suffix = env['bits']
    if env['platform'] == 'android':
        arch_suffix = env['android_arch']
    env['gd_library_name'] = 'godot-cpp.{}.{}.{}'.format(
        env['platform'],
        env['target'],
        arch_suffix,
    )
if env['target'] == 'release':
    env.Append(CCFLAGS='-Os')
    env.Append(LINKFLAGS='-flto')
elif env['target'] == 'debug':
    env.Append(CCFLAGS='-g')

if env['platform'] == 'linux':
    if env['bits'] == '64':
        env.Append(CCFLAGS=['-m64'])
        env.Append(LINKFLAGS=['-m64'])
    elif env['bits'] == '32':
        env.Append(CCFLAGS=['-m32'])
        env.Append(LINKFLAGS=['-m32'])

if env['platform'] == 'android':
    print('*** Building for <android> ***')
    if 'ANDROID_NDK_ROOT' not in env:
        raise ValueError("""\n\tTo build for Android, ANDROID_NDK_ROOT must be
             defined. Please set ANDROID_NDK_ROOT to the root folder of your
             Android NDK installation.""")

    # Validate API level
    api_level = int(env['android_api_level'])
    if env['android_arch'] in ['x86_64', 'arm64v8'] and api_level < 21:
        print("""WARN: 64-bit Android architectures require an API level of at
             least 21; setting android_api_level=21""")
        env['android_api_level'] = '21'
        api_level = 21

    # Setup android toolchain
    if not env['ANDROID_NDK_ROOT'].endswith('/'):
        env['ANDROID_NDK_ROOT'] += '/'
    toolchain = env['ANDROID_NDK_ROOT'] + 'toolchains/llvm/prebuilt/'
    if host_platform == 'windows':
        toolchain += 'windows'
        import platform as pltfm
        if pltfm.machine().endswith('64'):
            toolchain += '-x86_64'
    elif host_platform == "linux":
        toolchain += "linux-x86_64/"

    # Get architecture info
    arch_info_table = {
        "armv7": {
            "march": "armv7-a", "compiler_path": "armv7a-linux-androideabi"
        },
        "arm64v8": {
            "march": "armv8-a", "compiler_path": "aarch64-linux-android"
        },
        "x86": {
            "march": "i686", "compiler_path": "i686-linux-android"
        },
        "x86_64": {
            "march": "x86-64", "compiler_path": "x86_64-linux-android"
        }
    }

    toolchain += ('bin/' + arch_info_table[env['android_arch']]['compiler_path']
                  + env['android_api_level'])

    env['CC'] = toolchain + '-clang'
    env['CXX'] = toolchain + '-clang++'
elif env['platform'] == 'linux':
    print('*** Building for linux ***')


outpath = env['output_dir']
if not outpath.endswith('/'):
	outpath += "/"
if env['platform'] == 'linux':
    outpath += 'X11/'
elif env['platform'] == 'android':
    outpath += 'Android/'
elif env['platform'] == 'windows':
    outpath += 'Windows/'

suffix = env['android_arch'] if env['platform'] == 'android' else env['bits']
outputFile = outpath + 'libfarsireshaper.{}.{}.{}.{}'.format(
    env['platform'],
    env['target'],
    suffix,
    'so'
)

sources = []
sources += Glob('build/obj/*.cpp')

list_of_add_includes = env['add_includes'].split(sep=':')
for incl in list_of_add_includes:
	env.Append(CPPPATH=incl+':')

workspaceIncludes = ['include:']
for incl in workspaceIncludes:
    env.Append(CPPPATH=incl)

env.Append(CPPPATH=env['gd_headers_dir']+':')

GodotLibrary = env.SharedLibrary(target=outputFile,
                                 source=sources,
                                 LIBS=[env['gd_library_name']],
                                 LIBPATH=[env['gd_library_dir']])
Default(GodotLibrary)
