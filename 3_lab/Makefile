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

DEBUG = -g

SRC = src*/.h src*/*.cc

.PHONY: fmt
fmt: $(SRC) 
	@clang-format -i style=file $^

.PHONY: lint
lint: $(SRC)
	@cppcheck -enable=all $^

.PHONY: build1
build1.out: src1/*.cc uilib/uilib.cc
	@$(CXX) $(CXXFLAGS) $(SAN) $(DEBUG) $^ -o $@

.PHONY: clean
clean:
	-@rm *.out &> /dev/null || true
