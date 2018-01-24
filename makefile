FRAMEWORKS     = -framework Carbon
BUILD_PATH     = ./bin
BUILD_FLAGS    = -std=c99 -Wall -g -O0
QES_SRC        = ./src/qes.c
BINS           = $(BUILD_PATH)/qes

.PHONY: all clean install

all: clean $(BINS)

install: BUILD_FLAGS=-std=c99 -O3
install: clean $(BINS)

clean:
	rm -rf $(BUILD_PATH)

$(BUILD_PATH)/qes: $(QES_SRC)
	mkdir -p $(BUILD_PATH)
	clang $^ $(BUILD_FLAGS) $(FRAMEWORKS) -o $@
