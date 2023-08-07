#compiler macros
CC = gcc
CFLAGS = -I
TARGET = hello_app

#Path macros
BUILD_PATH := build
SRC_PATH := src
INC_PATH := include

INC_FILES = $(wildcard ./include/*.h)
OBJ_FILES = $(addprefix $(BUILD_PATH)/, $(notdir $(patsubst %.c, %.o, $(wildcard ./src/*.c))))

all: makedir $(OBJ_FILES)
	$(CC) -o $(BUILD_PATH)/$(TARGET) $(filter %.o, $^) $(CFLAGS) $(INC_PATH)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c $(INC_FILES)
	$(CC) -c -o $@ $< -DDEBUG $(CFLAGS) $(INC_PATH)

makedir: 
ifeq	($(shell find ./ -name 'build'), ./build)
	@echo "build folder have beean exist"
else
	$(shell mkdir build)
endif
clean:
	rm -rf ./build
install:
	rsync -avz $(BUILD_PATH)/$(TARGET) debian@192.168.1.10:/home/debian/app_cross_compile

