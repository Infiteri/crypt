BUILD_DIR = bin
OBJ_DIR   = bin-obj
SRC_DIR   = src

.PHONY: all scaffold

all: scaffold $(BUILD_DIR)/crypt

FILE_CPP   = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS_CPP = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(FILE_CPP))

# Final binary
$(BUILD_DIR)/crypt: $(OBJECTS_CPP)
	g++ $^ -o $@

# Compile .cpp -> .o into OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -c $< -o $@

# Create directories
scaffold:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

