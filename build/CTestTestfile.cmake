# CMake generated Testfile for 
# Source directory: /workspace
# Build directory: /workspace/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(LabOOPTests "/workspace/build/tests")
set_tests_properties(LabOOPTests PROPERTIES  _BACKTRACE_TRIPLES "/workspace/CMakeLists.txt;46;add_test;/workspace/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
