EXECUTABLE=buffer
TARGET=$(EXECUTABLE).a
CC=g++
FILEFORMAT=cpp
DFLAGS=-g3
LFLAGS=-Wall -Wextra # -Werror
RFLAGS=# -lm
OS=$(shell uname)

SRC_DIR=$(EXECUTABLE)_lib
BUILD_DIR=obj
BIN_DIR=bin

MODULES=$(shell find "$(SRC_DIR)" -name "*.$(FILEFORMAT)")

OBJECTS=$(MODULES:%.$(FILEFORMAT)=%.o)
MODULES_RUN = $(wildcard $(SRC_DIR)/*.$(FILEFORMAT)) main.$(FILEFORMAT)
OBJECTS_RUN = $(MODULES_RUN:%.c=%.o)

.PHONY: all run build rebuild clean $(TARGET)

all: rebuild 

debug: create_dir
	@$(CC) $(LFLAGS) $(DFLAGS) $(MODULES_RUN) -o $(BIN_DIR)/$@ $(RFLAGS)

build: $(TARGET)
	@$(CC) $(LFLAGS) main.$(FILEFORMAT) $(BUILD_DIR)/$(TARGET)  -o $(BIN_DIR)/$(EXECUTABLE) $(RFLAGS)

rebuild: clean $(TARGET)

$(TARGET): create_dir $(OBJECTS) style
	@ar rcs $(BUILD_DIR)/$(TARGET) $(OBJECTS:%.o=$(BUILD_DIR)/%.o)

%.o: %.$(FILEFORMAT)
	@$(CC) $(DFLAGS) $(LFLAGS) -c -o $(BUILD_DIR)/$@ $^ $(RFLAGS)

run: build
	@./$(BIN_DIR)/$(EXECUTABLE) 

style:
	@clang-format -i */*.hpp *.$(FILEFORMAT) */*.$(FILEFORMAT)

cppcheck:
	@cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedStructMember \
	          --suppress=checkersReport */*.hpp */*.$(FILEFORMAT) *.$(FILEFORMAT)

clean:
	@echo "Deleting unnecessary files..."
	@rm -rf obj bin log

run_and_valgrind: build
	@valgrind --track-origins=yes --leak-check=full --log-file="log/valgrind.log" \
	          --show-leak-kinds=all ./$(BIN_DIR)/$(EXECUTABLE) 

create_dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)