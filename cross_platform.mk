ifeq ($(strip $(BUILD_PLATFORM)),)
$(error Must set BUILD_PLATFORM mac or wasm.)
endif

ifeq ($(BUILD_PLATFORM), mac)
PLATFORM_FILTER_LIST := $(wildcard src/*wasm.c) $(wildcard src/*/*wasm.c)
PLATFORM_SRC_FILES := $(filter-out $(PLATFORM_FILTER_LIST), $(wildcard src/*.c) $(wildcard src/*/*.c))
endif

ifeq ($(BUILD_PLATFORM), wasm)
PLATFORM_FILTER_LIST := $(wildcard src/*sdlgl.c) $(wildcard src/*/*sdlgl.c)
PLATFORM_SRC_FILES := $(filter-out $(PLATFORM_FILTER_LIST), $(wildcard src/*.c) $(wildcard src/*/*.c))
endif
