CXX = clang++
CXXAFL = afl-clang++

SAN = -fsanitize=address,undefined

CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -pedantic
# CXXFLAGS += -Werror
CXXFLAGS += -Wmissing-declarations
CXXFLAGS += -fstack-protector-all

OPT = -Ofast

GTEST_DIR=../googletest/googletest
GTEST_INCLUDE=../googletest/googletest/include
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

SRC_DIR=src
TEST_DIR=tests

GTEST_INCLUDE2 = ../googletest/googletest/include/gtest

DEBUG = -g

.PHONY: default
#default: fmt lint test build docs fuzz
default: fmt lint build docs fuzz

.PHONY: fmt
fmt: src/*.h src/*.cc
	@clang-format -i -style=file src/*.h src/*.cc
	@echo "============================================================================"

.PHONY: lint
lint: src/*
	@cppcheck --enable=all src/
	@echo "============================================================================"

.PHONY: build
build: src/*.h src/*.cc
	@$(CXX) $(CXXFLAGS) $(SAN) $(DEBUG) src/*.cc -o task.out
	@echo "============================================================================"

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
	@echo "============================================================================"

.PHONY: fuzz
fuzz: fuzz-build fuzz-prepare fuzz-start

.PHONY: fuzz-build
fuzz-build:
	AFL_USE_ASAN=1 $(CXXAFL) $(OPT) src/*.cc -o fuzz.out

.PHONY: fuzz-prepare
fuzz-prepare:
	sudo afl-system-config

.PHONY: fuzz-start
fuzz-start:
	tmux new-session -d -s "fuzzing" afl-fuzz -i cases -o output -- ./fuzz.out


# GTest facility
# Taken from https://gist.github.com/mihaitodor/bfb8e7ad908489fdf3ceb496573f306a
#
.PHONY: tests/gtest-all.o tests/gtest_main.o tests/gtest_main.a tests/test.o
tests/gtest-all.o: gtest-all.o
gtest-all.o: $(GTEST_SRCS_)
	$(CXX) -I $(GTEST_DIR) -I $(GTEST_INCLUDE) $(DEBUG) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc -o tests/$@

tests/gtest_main.o: gtest_main.o
gtest_main.o: $(GTEST_SRCS_)
	$(CXX) -I $(GTEST_DIR) $(CXXFLAGS) $(DEBUG) -c $(GTEST_DIR)/src/gtest_main.cc -o tests/$@

gtest.a: tests/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

tests/gtest_main.a: gtest_main.a
gtest_main.a: tests/gtest-all.o tests/gtest_main.o
	$(AR) $(ARFLAGS) tests/$@ $^
	@echo "gtest_main.a compiled successfully"

tests/test.o: test.o
test.o: $(TEST_DIR)/test.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE2) $(DEBUG) -c $(TEST_DIR)/test.cc $(SAN) -o tests/test.o

test.out: tests/test.o tests/gtest_main.a
	$(CXX) $(CXXFLAGS) $(SAN) $(DEBUG) -lpthread $^ -o $@

test: test.out
	@./test.out
	@echo "============================================================================"

graph.dot: Makefile
	@make -Bnd | make2graph > graph.dot
