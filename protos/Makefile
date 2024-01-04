# Makefile for compiling protocol.proto	
PROTOC = protoc

# Define the .proto file
PROTO_FILE := protocol.proto

#Directory for generated code in Python
PYTHON_OUT_DIR = ./python_out

#Directory for generated code in C
C_OUT_DIR = ./c_out

.PHONY: all python c clean

all: python c

python:
	@mkdir -p $(PYTHON_OUT_DIR)
	$(PROTOC) --python_out=$(PYTHON_OUT_DIR) $(PROTO_FILE)

c:
	@mkdir -p $(C_OUT_DIR)
	$(PROTOC) --c_out=$(C_OUT_DIR) $(PROTO_FILE)

clean:
	rm -rf $(PYTHON_OUT_DIR) $(C_OUT_DIR)
	