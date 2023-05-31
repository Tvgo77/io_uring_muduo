SRC_PATH := $(PWD)/TCPchannel
INCLUDE_PATH := $(PWD)
BUILD_PATH := $(PWD)/build

CFLAGS := -O2 -w
LDFLAGS := -luring
src_files := ./TCPchannel/EventLoop.cc \
			 ./TCPchannel/Channel.cc \
             ./TCPchannel/Ring.cc \
			 ./TCPchannel/main.cc



all: $(BUILD_PATH)/echo_server
	@echo Building complete.

$(BUILD_PATH)/echo_server:  $(SRC_PATH)/Channel.cc \
			  				$(SRC_PATH)/EventLoop.cc \
			 				$(SRC_PATH)/Ring.cc \
			  				$(SRC_PATH)/main.cc
	mkdir -p build
	$(CXX) -I ./ $(CFLAGS) $(src_files) -o $(BUILD_PATH)/echo_server1 $(LDFLAGS)
	
