cmake_minimum_required(VERSION 3.15)
project(avs2)

set(CMAKE_CXX_STANDARD 17)

add_executable(mutex_counter task1/mutex_counter.cpp)
add_executable(atomic_counter task1/atomic_counter.cpp)
add_executable(dynamic_mutex_queue task2/dynamic_mutex_queue.cpp)
add_executable(fixed_size_mutex_queue task2/fixed_size_mutex_queue.cpp)
add_executable(fixed_size_atomic_queue task2/flixed_size_atomic_queue.cpp)