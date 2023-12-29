include ../../common/common.mk


SRC_FILES = $(wildcard *.c ../../common/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

TARGET = display-app

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: $(SRC_DIR)/%.c	
	$(CC) $(CFLAGS) -c -o $@ $<

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:	
	rm -f $(TARGET) $(OBJ_FILES)