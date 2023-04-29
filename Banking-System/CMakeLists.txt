cmake_minimum_required(VERSION 3.10)
project(BankingSystem C)

set(CMAKE_C_STANDARD 11)

# Add all source files
set(SOURCE_FILES main.c)

# Create the executable
add_executable(BankingSystem ${SOURCE_FILES})
