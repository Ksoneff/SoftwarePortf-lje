cmake_minimum_required(VERSION 3.10)
project(SoftwarePortfLje)

set(CMAKE_CXX_STANDARD 17)

# Add all .cpp files manually
add_executable(main_exec
    main.cpp
    ApeKing.cpp
    Controller.cpp
    Dragon.cpp
    Hero.cpp
    Hest.cpp
    Monster.cpp
    StrongerGoblin.cpp
    StrongGoblin.cpp
    TheStrongestGoblin.cpp
    Unicorn.cpp
    WeakGoblin.cpp
)

