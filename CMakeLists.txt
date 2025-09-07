cmake_minimum_required(VERSION 3.10.2)
project(echobot)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

# Укажи путь к vcpkg
set(CMAKE_PREFIX_PATH "C:/dev/vcpkg/installed/x64-windows")

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL REQUIRED)

# Включи директории
include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

# ОДНА команда add_executable (убрал дублирование)
add_executable(echobot 
    D:/VScode/projects/Tgbot/main.cpp
    database.cpp
    database.h
)

# Линковка всех библиотек включая SQLite
target_link_libraries(echobot 
    PRIVATE
    C:/dev/vcpkg/installed/x64-windows/lib/TgBot.lib
    unofficial::sqlite3::sqlite3
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OPENSSL_LIBRARIES} 
    ${Boost_LIBRARIES} 
    ${CURL_LIBRARIES}
)