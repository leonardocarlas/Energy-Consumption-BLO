cmake_minimum_required(VERSION 2.8)

project(blo)
set(SRC
        src/main.c
        src/blo.c
        src/plot.c
        src/read_input.c
        src/utils.c
        src/models/m1a.c include/models/m1.h src/models/m1.c)

add_executable(${PROJECT_NAME} ${SRC})

set(CPLEX /opt/ibm/ILOG/CPLEX_Studio201/cplex)

include_directories(include)
include_directories(include/modules)
target_link_libraries(${PROJECT_NAME} -L${CPLEX}/lib/x86-64_linux/static_pic)
target_link_libraries(${PROJECT_NAME} -lcplex -lm -lpthread -ldl)
target_include_directories(${PROJECT_NAME} PRIVATE ${CPLEX}/include)




