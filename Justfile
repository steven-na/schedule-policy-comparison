build:
    g++ -ggdb -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion \
    -std=c++23 \
    -o main \
    main.cpp scheduler.cpp simulator.cpp \
    FIFO.cpp

build-release:
    g++ -O3 \
    -std=c++23 \
    -o main \
    main.cpp scheduler.cpp simulator.cpp\
    FIFO.cpp
