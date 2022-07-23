# CMake generated Testfile for 
# Source directory: C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests
# Build directory: C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/tests/unittests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(unittests "C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/bin/unittests.exe")
  set_tests_properties(unittests PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests/CMakeLists.txt;42;add_test;C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(unittests "C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/bin/unittests.exe")
  set_tests_properties(unittests PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests/CMakeLists.txt;42;add_test;C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(unittests "C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/bin/unittests.exe")
  set_tests_properties(unittests PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests/CMakeLists.txt;42;add_test;C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(unittests "C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/bin/unittests.exe")
  set_tests_properties(unittests PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests/CMakeLists.txt;42;add_test;C:/Users/hatte/SELFGAMEENGINE/DirectX12Engine/DirectX12Engine/slang/tests/unittests/CMakeLists.txt;0;")
else()
  add_test(unittests NOT_AVAILABLE)
endif()
