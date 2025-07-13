PROJECT_NAME = STMidi
BUILD_DIR = build
PROJECT_DIR = .
TARGET_BIN = build/$(PROJECT_NAME).bin
FLASH_ADDR = 0x08000000

COMPILE_COMMANDS = compile_commands.json

# ST-Link Tools
ST_FLASH := st-flash
ST_INFO := st-info
ST_RESET := $(ST_FLASH) reset
ST_PROBE := $(ST_INFO) --probe

all: setup build

setup:
	@mkdir -p $(BUILD_DIR)
	cmake -S $(PROJECT_DIR) -B $(BUILD_DIR)
	@cp $(BUILD_DIR)/$(COMPILE_COMMANDS) ./

build:
	cmake --build $(BUILD_DIR) -- -j$(nproc)

flash:
	$(ST_FLASH) write $(TARGET_BIN) $(FLASH_ADDR)

reset:
	$(ST_RESET)

probe:
	$(ST_PROBE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all setup build flash reset probe clean
