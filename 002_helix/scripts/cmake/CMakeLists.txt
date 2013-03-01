cmake_minimum_required(VERSION 2.8)

set(ROXLU_32BIT ON)
add_definitions("-DROXLU_GL_WRAPPER=1") 

include(${CMAKE_CURRENT_LIST_DIR}/../../../../../scripts/cmake/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../../roxlu/lib/build/cmake/CMakeLists.txt)

roxlu_add_addon("Buttons")

of_app_initialize("helix")

of_app_add_source_file(main.cpp)
of_app_add_source_file(testApp.cpp)
of_app_add_source_file(helix/Helix.cpp)
of_app_add_source_file(helix/HelixDrawer.cpp)
of_app_add_source_file(helix/HelixShaders.cpp)
of_app_add_source_file(helix/Particles.cpp)

of_add_source_files(roxlu_source_files)
of_add_source_files(roxlu_lib_source_files)

of_add_lib(roxlu_buttons)

of_install_app()

