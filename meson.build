project('SysrepoListener', ['c', 'cpp'],
  version : '1.0',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])

libpcre = dependency('libpcre', required : true)
sysrepo = dependency('sysrepo', required : true)
libyang = dependency ('libyang', required : true)



subdir ('libyangextension/source')
subdir ('libyangcpp/source')
subdir ('libsysrepocpp/source')
subdir('src')

libyangextension = static_library(
    'libyangextension',
    sources : libyangextension_sources_with_path,
    include_directories : [
        libyangextension_includes
    ]
)

libyangextension_dep = declare_dependency (
    link_with :[libyangextension],
    dependencies : libpcre,
    include_directories : [
      libyangextension_includes
    ]
)

libyangcpp = static_library(
    'libyangcpp',
    sources : libyangcpp_sources_with_path,
    include_directories : [
        libyangcpp_includes
    ]
)

libyangcpp_dep = declare_dependency (
    link_with :[libyangcpp],
    dependencies : libyangextension_dep,
    include_directories : [
      libyangcpp_includes
    ]
)


libsysrepocpp = static_library(
    'libsysrepocpp',
    sources : libsysrepocpp_sources_with_path,
    include_directories : [
        libsysrepocpp_includes
    ]
)

libsysrepocpp_dep = declare_dependency (
    link_with :[libsysrepocpp],
    dependencies : libyangcpp_dep,
    include_directories : [
      libsysrepocpp_includes
    ]
)

NetConfAgent = static_library(
    'NetConfAgent',
    sources : src_with_path,
    include_directories : [
        inc
    ]
)

NetConfAgent_dep = declare_dependency (
    link_with :[NetConfAgent],
    dependencies : libsysrepocpp_dep,
    include_directories : [
      inc
    ]
)

#add_project_arduments('LIBS=-ldl', language : 'c')

exe = executable('Listener', 
sources : 'main.cpp',
dependencies : [sysrepo, libsysrepocpp_dep, NetConfAgent_dep],
  install : true,
  link_args: ['-lpthread', '-ldl'],
  cpp_args : '-pthread')
  
test('basic', exe)
