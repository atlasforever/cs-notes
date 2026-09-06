BUILD_DIR := build
COMPILE_COMMANDS := compile_commands.json

.PHONY: all configure clean rebuild

all: configure
	cmake --build $(BUILD_DIR)

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	ln -sf $(BUILD_DIR)/$(COMPILE_COMMANDS) $(COMPILE_COMMANDS)

clean:
	cmake --build $(BUILD_DIR) --target clean

rebuild:
	cmake --build $(BUILD_DIR) --clean-first
