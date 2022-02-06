
add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} zip pthread)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)