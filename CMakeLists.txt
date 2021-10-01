cmake_minimum_required(VERSION 2.8)

project(blo)
set(SRC
        src/main.c
        src/blo.c
        src/plot.c
        src/read_input.c
        src/utils.c)

add_executable(${PROJECT_NAME} ${SRC})

include_directories(include)

set(CPLEX /home/leonardo/CPLEX_Studio129/cplex)

include_directories(include)
target_link_libraries(${PROJECT_NAME} -L${CPLEX}/lib/x86-64_linux/static_pic)
target_link_libraries(${PROJECT_NAME} -lcplex -lm -lpthread -ldl)
target_include_directories(${PROJECT_NAME} PRIVATE ${CPLEX}/include)




