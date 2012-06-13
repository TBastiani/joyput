# -- File list ----------
FILE = joyput.c utils.c uinput.c joystick_read.c translate.c
C_OPTION = -std=c99 -Wall
CC=clang
OS = LINUX

# -- Flags ----------
C_DEBUG_FLAGS = #-g -DDEBUG
C_OPTIMISATION_FLAGS = -O3
C_ARCH_FLAGS =
C_OS_FLAGS = -D$(OS)

OBJ_PATH = obj
SRC_PATH = src
EXE_PATH = bin
INCLUDE_PATH = include

LIB_INC_PATH = -I$(INCLUDE_PATH)
LIB_LIB_PATH =

CFLAGS = $(C_OPTION) $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_OS_FLAGS) $(LIB_INC_PATH) $(C_DEBUG_FLAGS)
LDFLAGS = $(C_OPTION) $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_OS_FLAGS) $(LIB_LIB_PATH)

# -- src and obj List ----------
SRC = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))

# -- Final product ----------
PRODUCT = joyput

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#-----Main rule ----------
default : $(EXE_PATH)/$(PRODUCT)

$(EXE_PATH)/$(PRODUCT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIB_INC_PATH) $(LIB_LIB_PATH)

clean:
	rm -f $(EXE_PATH)/$(PRODUCT) $(OBJ_PATH)/*.o
