# LizardsNRoaches/Makefile

all: install-dependencies protocol server lizard-client roach-client 

install-dependencies:
	sudo apt-get update
	sudo apt-get install -y build-essential
	sudo apt-get install -y libzmq3-dev
	sudo apt-get install -y protobuf-compiler
	sudo apt-get install -y python3-pip
	pip3 install pyzmq
	pip3 install protobuf

protocol:
	@$(MAKE) -C common

server: 
	@$(MAKE) -C src/server

lizard-client:
	@$(MAKE) -C src/lizard-client 

roach-client:
	@$(MAKE) -C src/roaches-client

getPid:
	lsof -i :5555

clean:
	@$(MAKE) -C common clean
	@$(MAKE) -C src/server clean
	@$(MAKE) -C src/lizard-client clean
	@$(MAKE) -C src/roaches-client clean
	@find . -type f -name '*.o' -delete
