TARGET_MODULE:=position

$(TARGET_MODULE)-objs := main.o device_file.o
obj-m := $(TARGET_MODULE).o
BUILDSYSTEM_DIR=/home/pi/kernel-4.19.50
PWD:=$(shell pwd)

all : 
# run kernel build system to make module
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

clean:
# run kernel build system to cleanup in current directory
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean

load:
	insmod ./$(TARGET_MODULE).ko

unload:
	rmmod ./$(TARGET_MODULE).ko
