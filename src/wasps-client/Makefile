include ../../common/common.mk

SRC_FILES = $(wildcard *.c ../../common/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

TARGET = wasps-client

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: $(SRC_DIR)/%.c	
	$(CC) $(CFLAGS) -c -o $@ $<

clean:	
	rm -f $(TARGET) $(OBJ_FILES)