IDIR := ../include
TDIR := ../tools
LDIR :=../bin
CFLAGS :=-I $(IDIR) -I $(TDIR) -L $(LDIR) -g -std=c++11

vgg1: vgg1.cpp
	g++ $(CFLAGS) $^ -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o vgg1

.PHONY: clean

clean:
	rm -rf vgg1 *~
