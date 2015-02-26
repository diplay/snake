cmake_minimum_required (VERSION 2.6)
project (snake)

set(SRCFILES ../src/first.cpp ../src/entry_point.cpp ../src/Snake.cpp
	../src/Bonus.cpp ../src/Scoreboard.cpp ../src/Menu.cpp
	../src/Scene.cpp ../src/Game.cpp)

add_subdirectory(../../oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

add_subdirectory(../../oxygine-sound/ oxygine-sound)
include_directories(${OXYGINESOUND_INCLUDE_DIRS})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(snake  ${SRCFILES})

target_link_libraries(snake ${OXYGINE_CORE_LIBS} ${OXYGINESOUND_LIBS})

add_custom_command(TARGET snake PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ../data $<TARGET_FILE_DIR:snake>)
