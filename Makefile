C = clang
CXX = clang++ -std=c++20
EXEC = ptegen
BUILD_DIR = .build
SRC_DIR = src

build:
	mkdir -p ${BUILD_DIR}
	${C} -c sqlite/sqlite3.c -o ${BUILD_DIR}/sqlite.o
	${CXX} -c ${SRC_DIR}/args.cpp -o ${BUILD_DIR}/args.o
	${CXX} -c ${SRC_DIR}/generator.cpp -o ${BUILD_DIR}/generator.o
	${CXX} ${SRC_DIR}/main.cpp ${BUILD_DIR}/sqlite.o ${BUILD_DIR}/args.o ${BUILD_DIR}/generator.o -o ${BUILD_DIR}/${EXEC}

run:
	./${BUILD_DIR}/${EXEC}

clean:
	rm -rf ${BUILD_DIR}
