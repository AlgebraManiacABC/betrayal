CC=gcc
CFLAGS=-Wall -Werror -lm -std=c17
TARGET=betrayal
ifeq ($(OS),Windows_NT)
	CFLAGS += -lmingw32 -llibSDL2_image 
#-Wl,--subsystem,windows
	TARGET := $(TARGET).exe
else
	CFLAGS += -lSDL_image
endif
	CFLAGS += -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer

LIB_DIR=lib
SRC_DIR=src
OBJ_DIR=build

LIBS=$(wildcard $(LIB_DIR)/*.h)
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(notdir $(SRCS:.c=.o))
OBJS:=$(addprefix $(OBJ_DIR)/,$(OBJS))

ICON=src/img/icon/boxart.res
RC=src/img/icon/boxart.rc

betrayal: $(OBJS)
ifeq ($(OS),Windows_NT)
	$(CC) $^ -o $@ $(CFLAGS) $(ICON)
else
	$(CC) $^ -o $@ $(CFLAGS)
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIBS) | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(ICON): $(RC)
ifeq ($(OS),Windows_NT)
	windres $^ -O coff -o $@
endif

.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) -r $(OBJ_DIR)
#Careful with the -r...
