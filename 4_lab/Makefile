CXX=clang++
CXXSTD=c++17
SRCS=src/*.cc
HEADERS=src/*.h

SAN = -fsanitize=address,undefined
SAN += -fno-common
SAN += -fno-omit-frame-pointer

CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -pedantic
#CXXFLAGS += -Werror
CXXFLAGS += -Wmissing-declarations
CXXFLAGS += -fstack-protector-all

OPT = -Ofast

DEBUG = -g

GTKINCLUDE = $(shell pkg-config --cflags --libs gtkmm-3.0)
JSONINCLUDE = $(shell pkg-config nlohmann_json --cflags)
SIGCINCLUDE = $(shell pkg-config --cflags --libs sigc++-3.0)

.PHONY: all
all: fmt lint build test

.PHONY: fmt
fmt: $(SRCS) $(HEADERS)
	clang-format -i style=file $^

.PHONY: lint
lint: $(SRCS) $(HEADERS)
	@cppcheck --enable=all --language=c++ $^

.PHONY: build
build: $(SRCS)
	$(CXX) -o build.out -std=$(CXXSTD) $(SAN) $(CXXFLAGS) $(OPT) $(DEBUG) $(GTKINCLUDE) $(JSONINCLUDE) $(SIGCINCLUDE) $^

.PHONY: test
test: tests.out
	@./tests.out

tests.out: tests/test.o ../include/gtest_main.a src/gui.cc src/library.cc
	$(CXX) $(CXXFLAGS) -std=$(CXXSTD) $(DEBUG) $(SAN) $(GTKINCLUDE) $(JSONINCLUDE) -lpthread $^ -o $@

tests/test.o: tests/*.cc
	$(CXX) $(CXXFLAGS) -std=$(CXXSTD) $(DEBUG) $(SAN) $(GTKINCLUDE) $(JSONINCLUDE) -c $^ -o $@

.PHONY: docs
docs: doxygen.conf
	@doxygen doxygen.conf
	@firefox docs/html/index.html &

.PHONY: clean
clean:
	-@rm *.out &> /dev/null || true
	-@rm tests/test.o &> /dev/null || true
