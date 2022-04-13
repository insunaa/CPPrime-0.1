project('CPPrime', 'cpp',
  default_options : [
    'cpp_std=c++17',
    'buildtype=release',
  ],
  version : '0.1.1')

if build_machine.system() == 'linux'
  add_project_arguments(['-march=native', '-mtune=native', '-fno-signed-zeros', '-freciprocal-math', '-ffp-contract=fast', '-funsafe-math-optimizations', '-ffinite-math-only', '-ffast-math', '-fno-trapping-math'], language: 'cpp')
endif

executable('CPPrime', 'CPPrime.cpp')
