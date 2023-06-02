SRC_PATH := $(PWD)/TCPchannel
INCLUDE_PATH := $(PWD)
BUILD_PATH := $(PWD)/build

CFLAGS := -O2 -w
LDFLAGS := -luring -lpthread
src_files := ./TCPchannel/EventLoop.cc \
			 ./TCPchannel/Channel.cc \
             ./TCPchannel/Ring.cc \
			 ./lib/syscall_wrap.c \
			 ./TCPchannel/main_EchoServer.cc

src_files2 := ./TCPchannel/EventLoop.cc \
			 ./TCPchannel/ChannelF.cc \
             ./TCPchannel/Ring.cc \
			 ./TCPchannel/main_FileChannel.cc



all: $(BUILD_PATH)/echo_server $(BUILD_PATH)/echo_client $(BUILD_PATH)/file_channel
	@echo Building complete.

$(BUILD_PATH)/echo_server:  $(SRC_PATH)/Channel.cc \
			  				$(SRC_PATH)/EventLoop.cc \
			 				$(SRC_PATH)/Ring.cc \
			  				$(SRC_PATH)/main_EchoServer.cc
	@mkdir -p build
	@echo ------------------------------------
	$(CXX) -I ./ $(CFLAGS) $(src_files) -o $(BUILD_PATH)/echo_server $(LDFLAGS)

$(BUILD_PATH)/echo_client: $(PWD)/things/echo_client.c
	@mkdir -p build
	@echo ------------------------------------
	$(CC) -I ./ $(CFLAGS) ./lib/syscall_wrap.c ./things/echo_client.c -o $(BUILD_PATH)/echo_client -lpthread
	

$(BUILD_PATH)/file_channel: $(SRC_PATH)/main_FileChannel.cc
	@mkdir -p build
	@echo ------------------------------------
	$(CXX) -I ./ $(CFLAGS) $(src_files2) -o $(BUILD_PATH)/file_channel $(LDFLAGS)
