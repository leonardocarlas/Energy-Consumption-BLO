cmake_minimum_required(VERSION 2.8)

project(blo)
set(SRC
        src/main.c
        src/ll.c
        src/plot.c
        src/read_input.c
        src/utils.c
        src/models/m1a.c
        include/models/m1.h
        src/models/m1.c
        include/models/m2ewh.h
        src/models/m2ewh.c
        include/models/m2ev.h
        src/models/m2ev.c
        include/models/mo.h src/models/mo.c include/models/m2sb.h src/models/m2sb.c src/models/m3.c include/models/m3.h src/ul.c include/ul.h include/pso.h src/pso.c src/sa.c include/sa.h src/ga.c include/ga.h src/lbs.c include/lbs.h src/csa.c include/csa.h)

add_executable(${PROJECT_NAME} ${SRC})

set(CPLEX /opt/ibm/ILOG/CPLEX_Studio201/cplex)

include_directories(include)
include_directories(include/modules)
target_link_libraries(${PROJECT_NAME} -L${CPLEX}/lib/x86-64_linux/static_pic)
target_link_libraries(${PROJECT_NAME} -lcplex -lm -lpthread -ldl)
target_include_directories(${PROJECT_NAME} PRIVATE ${CPLEX}/include)




