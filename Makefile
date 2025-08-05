EXECUTABLE=buffer
TARGET=$(EXECUTABLE).a
CC=g++
FILEFORMAT=cpp
DFLAGS=-g3
LFLAGS=-Wall -Wextra # -Werror
RFLAGS=-lm
OS=$(shell uname)
TEST_TARGET=test

TEST_DIR=tests
SRC_DIR=$(EXECUTABLE)_lib
BUILD_DIR=obj
BIN_DIR=bin

MODULES=$(shell find "$(SRC_DIR)" -name "*.$(FILEFORMAT)")

OBJECTS=$(MODULES:%.$(FILEFORMAT)=%.o)
MODULES_TEST=$(wildcard $(TEST_DIR)/*.$(FILEFORMAT))
OBJECTS_TEST=$(MODULES_TEST:%.c=%.o)
MODULES_RUN = $(wildcard $(SRC_DIR)/*.$(FILEFORMAT)) main.$(FILEFORMAT)
OBJECTS_RUN = $(MODULES_RUN:%.c=%.o)


IS_UBUNTU = $(shell grep -i 'ubuntu' /etc/os-release 2>/dev/null)
IS_DEBIAN = $(shell grep -i 'debian' /etc/os-release 2>/dev/null)

OS = $(shell uname)
ifeq ($(OS), Darwin)
	LDLIBS = -lcheck -lm -lpthread
else
	LDLIBS = -lcheck -lm -lsubunit -lpthread  -lrt 
endif

.PHONY: all run build rebuild clean test gcov_report valgrind_check $(TARGET)

all: rebuild # test gcov_report

debug: create_dir
	@$(CC) $(LFLAGS) $(DFLAGS) $(MODULES_RUN) -o $(BIN_DIR)/$@ $(RFLAGS)

debug_test: create_dir
	@$(CC)  $(DFLAGS) $(MODULES) $(MODULES_TEST) $(LDLIBS) -L. -o $(BIN_DIR)/$@ $(RFLAGS)

build: $(TARGET)

rebuild: clean $(TARGET)

$(TARGET): create_dir $(OBJECTS) style
	@ar rcs $(BUILD_DIR)/$(TARGET) $(shell find "$(BUILD_DIR)/$(SRC_DIR)" -name "*.o" -not -name "main.o")

%.o: %.$(FILEFORMAT)
	@$(CC) $(DFLAGS) $(LFLAGS) -c -o $(BUILD_DIR)/$@ $^ $(RFLAGS)

run: create_dir_run $(TARGET)
	@$(CC) $(LFLAGS) main.$(FILEFORMAT) $(BUILD_DIR)/$(TARGET)  -o $(BIN_DIR)/$(EXECUTABLE) $(RFLAGS)
	@./$(BIN_DIR)/$(EXECUTABLE) test_files/simple_recursive.csv

test: $(TARGET) $(OBJECTS_TEST)
	@$(CC) $(DFLAGS) $(LFLAGS) $(wildcard $(BUILD_DIR)/$(TEST_DIR)/*.o) $(BUILD_DIR)/$(TARGET) $(LDLIBS) -L. -o $(BIN_DIR)/$(TEST_TARGET)
	@./$(BIN_DIR)/$(TEST_TARGET)
	
gcov_report: $(TARGET)
	@$(CC) --coverage $(LFLAGS) $(wildcard $(TEST_DIR)/*.c) $(LDLIBS) $(MODULES) -o $(BIN_DIR)/$(TEST_TARGET)_gcov
	@./$(BIN_DIR)/$(TEST_TARGET)_gcov
	@lcov -t "test_coverage" -o test_coverage.info -c -d .
	@genhtml -o report test_coverage.info
	@open report/index.html
	@rm -rf */*.gcda */*.gcno report/test_coverage.info *.info

style:
	@clang-format -i */*.hpp *.$(FILEFORMAT) */*.$(FILEFORMAT)

valgrind_check: test
	@CK_FORK=no valgrind --vgdb=no --leak-check=full \
	--show-leak-kinds=all --track-origins=yes --log-file="valgrind.log" -v --verbose -q --quiet -s ./$(BIN_DIR)/$(TEST_TARGET)

run_and_valgrind: run
	@valgrind --track-origins=yes --leak-check=full --log-file="valgrind.log" --show-leak-kinds=all ./$(BIN_DIR)/$(EXECUTABLE) test_files/bad/arg.csv

clean:
	@echo "Deleting unnecessary files..."
	@rm -rf obj bin report *.a *.log  log
	@rm -rf **/*.dSYM **/*.dSYM **/*.log **/*.log test_coverage.info

# Установка check.h

install_check:
	@sudo apt install check

create_dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	@mkdir -p $(BUILD_DIR)/$(TEST_DIR)
	@mkdir -p $(BIN_DIR)

create_dir_run:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	@mkdir -p $(BIN_DIR)

cppcheck:
	@cppcheck --enable=all --suppress=missingIncludeSystem */*.$(FILEFORMAT) */*.h