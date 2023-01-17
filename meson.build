project(
  'unicode-ident4c', 'c',
  version : '0.1.0',
  default_options : ['warning_level=3', 'c_std=c11']
)

ident4c_include = include_directories('include')

# TODO: Support generating tables via meson?
# Seems overkill.....

ident4c_lib = static_library(
  'ident4c',
  sources : ['src/ident4c.c', 'src/tables.c'],
  include_directories : [ident4c_include]
)

ident4c_dep = declare_dependency(
  include_directories : [ident4c_include],
  link_with: [ident4c_lib]
)

