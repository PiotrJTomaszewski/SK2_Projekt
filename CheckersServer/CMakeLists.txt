cmake_minimum_required(VERSION 3.12)
project(CheckersServer C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic)

add_executable(CheckersServer main.c game.c game.h test.c test.h server.c server.h room.h messages.h player.c player.h playersList.c playersList.h room.c circuralBuffer.c circuralBuffer.h serverClientCommunication.c serverClientCommunication.h messages.c serverGame.c serverGame.h)


find_package(Threads REQUIRED)

target_link_libraries(CheckersServer
        ${CMAKE_THREAD_LIBS_INIT}
        )