cmake_minimum_required(VERSION 3.14)
project(RPGGame)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 REQUIRED COMPONENTS Core Sql)

set(CMAKE_AUTOMOC ON)

set(SOURCES
    main.cpp
    Controller.cpp
    CaveFactory.cpp
    MonsterFactory.cpp
    Weapons.cpp
    Hero.cpp
    Monster.cpp
    DatabaseCommunication.cpp
)

set(HEADERS
    Controller.h
    CaveFactory.h
    MonsterFactory.h
    MonsterType.h
    Weapons.h
    Hero.h
    Monster.h
    DatabaseCommunication.h
)

add_executable(RPGGame ${SOURCES} ${HEADERS})

target_link_libraries(RPGGame Qt5::Core Qt5::Sql)

