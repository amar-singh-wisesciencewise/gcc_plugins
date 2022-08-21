#Where your build gcc is installed
GCC_DIR = /home/amar/AMAR/GCC/gcc_c_cpp_12/bin

CPP = $(GCC_DIR)/g++
CPP_FLAGS = -Wall -fno-rtti

# Add plugin directory to the flags
PLUGIN_DIR=$(shell $(CPP) -print-file-name=plugin)
CPP_FLAGS += -I$(PLUGIN_DIR)/include

#Build plugin shared library
all: plugin_001_bb_count.so

plugin_001_bb_count.so: plugin_001_bb_count.o
	$(CPP) -shared -o $@ $<

plugin_001_bb_count.o: plugin_001_bb_count.cpp
	$(CPP) $(CPP_FLAGS) -fPIC -c -o $@ $<

clean:
	rm -f plugin_001_bb_count.o plugin_001_bb_count.so my_test.o my_test.txt

test: plugin_001_bb_count.so my_test.cpp
	$(CPP) -fplugin=./plugin_001_bb_count.so -c my_test.cpp > my_test.txt
 
.PHONY: all clean check
