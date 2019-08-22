cmake_minimum_required(VERSION 3.14)
project(pubsb2)

set(CMAKE_CXX_STANDARD 17)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

add_executable(pubsb2 test1.cc src/event_loop.h src/event_source.h src/publisher.h src/replier.h src/requester.h src/subscriber.h src/timer.h src/udp_source.h
        src/event_loop.cc src/event_source.cc src/publisher.cc src/replier.cc src/requester.cc src/subscriber.cc src/timer.cc src/udp_source.cc test2.cc)

target_link_libraries(pubsb2 ${CONAN_LIBS})