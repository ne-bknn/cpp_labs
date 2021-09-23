CXXAFL=afl-clang++

.PHONY: fuzz
fuzz: fuzz-build fuzz-prepare fuzz-start

.PHONY: fuzz-build
fuzz-build:
	AFL_USE_ASAN=1 $(CXXAFL) $(OPT) *.cc -o fuzz.out

.PHONY: fuzz-prepare
fuzz-prepare:
	sudo afl-system-config

.PHONY: fuzz-start
fuzz-start:
	tmux new-session -d -s "fuzzing" afl-fuzz -i in -o out -- ./fuzz.out

