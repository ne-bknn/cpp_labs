CXX = clang++
CXXAFL = afl-clang-fast++

SAN = -fsanitize=address,undefined

CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -pedantic
#CXXFLAGS += -Werror
CXXFLAGS += -Wmissing-declarations
CXXFLAGS += -fstack-protector-all

LIBFLAGS += -fPIC
LIBFLAGS += -shared

OPT = -Ofast

DEBUG = -g

SRC = *.cc

default: fmt lint build.out test

.PHONY: fmt
fmt: $(SRC) 
	@clang-format -i -style=file $^

.PHONY: lint
lint: $(SRC)
	@cppcheck -enable=all $^

.PHONY: build
build: build.out

build.out: *.cc uilib/uilib.cc
	@$(CXX) $(CXXFLAGS) $(SAN) $(DEBUG) $^ -o $@

test: test.out
	@./test.out

# filter-out does not work((
TEST_SRC := word_array.cc trie.cc
test.out: tests/test.o ../../include/gtest_main.a $(TEST_SRC) uilib/uilib.cc
	$(CXX) $(CXXFLAGS) $(SAN) $(DEBUG) -lpthread $^ -o $@

tests/test.o: tests/*.cc
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $^ $(SAN) -o $@

libwordarr.so: $(SRC)
	$(CXX) $(LIBFLAGS) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	-@rm *.out &> /dev/null || true
	-@rm tests/test.o &> /dev/null || true
	-@rm *.so &> /dev/null || true

