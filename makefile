FENNEC_INCLUDES := -I./include
CFLAGS := -std=c11 $(FENNEC_INCLUDES) -Wall -Wextra -Werror -O3

FENNEC_SRCS := $(wildcard src/*.c) $(wildcard src/*/*.c)
FENNEC_OBJ := $(addprefix build/obj/,$(FENNEC_SRCS:.c=.o))
FENNEC_DEP_FILES := $(addprefix build/obj/,$(FENNEC_SRCS:.c=.d))

FENNEC_TESTS := dynamic_array_tests hashtable_tests path_tests string_tests
FENNEC_TEST_BINS := $(addprefix build/bin/tests/, $(FENNEC_TESTS))
FENNEC_TEST_SRCS := $(addsuffix .c, $(addprefix tests/, $(FENNEC_TESTS)))

all: build/lib/libfennec.a

build:
	@mkdir -p build

build/obj: build
	@mkdir -p build/obj

build/lib: build
	@mkdir -p build/lib

build/bin: build
	@mkdir -p build/bin

build/bin/tests: build/bin
	@mkdir -p build/bin/tests

build/lib/libfennec.a: $(FENNEC_OBJ) build/lib
	ar rcs $@ $(FENNEC_OBJ)

build/obj/%.o: %.c | build/obj $(FENNEC_DEP_FILES)
	@echo $@
	@clang $(CFLAGS) $(FENNEC_INCLUDES) -I$(dir $<) -c $< -o $@

build/obj/%.d: %.c
	@mkdir -p $(dir $@)
	@clang $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

test: $(FENNEC_TEST_BINS) $(FENNEC_TEST_SRCS)
	@for t in $(FENNEC_TEST_BINS); do ./$$t; done

build/bin/tests/%: tests/%.c build/lib/libfennec.a build/bin/tests
	@echo $@
	@clang $(CFLAGS) $(FENNEC_INCLUDES) $< -Lbuild/lib -lfennec -o $@

clean:
	@rm -rf build

# this includes the dependency files as targets after they are made.
-include $(FENNEC_DEP_FILES)
