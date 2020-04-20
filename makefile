FENNEC_INCLUDES := -I./include
CFLAGS := -std=c11 $(FENNEC_INCLUDES) -Wall -Wextra -Werror -O3

FENNEC_SRCS := $(wildcard src/*.c) $(wildcard src/*/*.c)
FENNEC_OBJ := $(addprefix build/obj/,$(FENNEC_SRCS:.c=.o))
FENNEC_OBJ_DIRS := $(dir $(FENNEC_OBJ))

$(info $(FENNEC_OBJ_DIRS))

all: build/lib/fennec.a

build:
	mkdir -p build

build/obj: build
	mkdir -p build/obj

build/lib: build
	mkdir -p build/lib

build/lib/fennec.a: $(FENNEC_OBJ) build/lib
	ar rcs $@ $(FENNEC_OBJ)

build/obj/%.o: %.c | build/obj
	mkdir -p $(dir $@)
	clang $(CFLAGS) $(FENNEC_INCLUDES) -I$(dir $<) -c $< -o $@

clean:
	rm -rf build
