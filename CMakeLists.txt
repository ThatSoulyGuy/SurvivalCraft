

cmake_minimum_required(VERSION 3.8)

project("SurvivalCraft")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(SurvivalCraft "SurvivalCraft/SurvivalCraft.cpp" "glad.c") 

include_directories("SurvivalCraft/Include")
include_directories("Library/Include")

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if(WIN32)
    link_directories("${PROJECT_SOURCE_DIR}/Library/Lib/Windows")
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_libraries(SurvivalCraft "${PROJECT_SOURCE_DIR}/Library/Lib/Windows/glfw3d.lib")
        target_compile_definitions(SurvivalCraft PRIVATE _DEBUG=1)
    elseif(CMAKE_BUILD_TYPE MATCHES Release)
        target_link_libraries(SurvivalCraft "${PROJECT_SOURCE_DIR}/Library/Lib/Windows/glfw3.lib")
    endif()
elseif(UNIX AND NOT APPLE)
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
    target_link_libraries(SurvivalCraft "${PROJECT_SOURCE_DIR}/Library/Lib/Linux/libglfw3.a")
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_definitions(SurvivalCraft PRIVATE _DEBUG=1)
    endif()
endif()