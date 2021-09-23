CXX = clang++
CXXAFL = afl-clang-fast++

SAN = -fsanitize=address,undefined

CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -pedantic
# CXXFLAGS += -Werror
CXXFLAGS += -Wmissing-declarations
CXXFLAGS += -fstack-protector-all

OPT = -Ofast

SRC_DIR=src
TEST_DIR=tests

DEBUG = -g

.PHONY: default
#default: fmt lint test build docs fuzz
default: fmt lint build docs fuzz

.PHONY: fmt
fmt: src/*.h src/*.cc
	@clang-format -i -style=file src/*.h src/*.cc

.PHONY: lint
lint: src/*
	@cppcheck --enable=all src/

.PHONY: build
build: src/*.h src/*.cc
	@$(CXX) $(CXXFLAGS) $(SAN) $(DEBUG) src/*.cc uilib/uilib.cc -o task.out

.PHONY: clean
clean:
	-@rm src/*.gch &> /dev/null || true
	-@rm *.out &> /dev/null || true
	-@rm tests/*.a tests/*.o tests/*.out &> /dev/null || true
	-@rm -r docs/ &> /dev/null || true
	-@rm -r output/ &> /dev/null || true

.PHONY: docs
docs: .doxygen.conf
	@doxygen .doxygen.conf
	@firefox docs/html/index.html &

.PHONY: fuzz
fuzz: fuzz-build fuzz-prepare fuzz-start

.PHONY: fuzz-build
fuzz-build:
	AFL_USE_ASAN=1 $(CXXAFL) $(OPT) src/*.cc uilib/uilib.cc -o fuzz.out

.PHONY: fuzz-prepare
fuzz-prepare:
	sudo afl-system-config

.PHONY: fuzz-start
fuzz-start:
	tmux new-session -d -s "fuzzing" afl-fuzz -i cases -o output -- ./fuzz.out

tests/test.o: test.o
test.o: $(TEST_DIR)/test.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $(TEST_DIR)/test.cc $(SAN) -o tests/test.o

test.out: tests/test.o ../include/gtest_main.a
	$(CXX) $(CXXFLAGS) $(SAN) $(DEBUG) -lpthread $^ src/curve.cc uilib/uilib.cc -o $@

test: test.out
	@./test.out

graph.dot: Makefile
	@make -Bnd | make2graph > graph.dot
