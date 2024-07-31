objects := NVGen.o FileParser.o md5c.o TCharOpr.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o GenDeltaNVIndex.o pugixml.o
COMPILER=gcc
MAIN_SRC_DIR=../Source
INC=-I ../Source

all:NVGen

NVGen:$(objects)
	gcc -static -Os -o NVGen NVGen.o FileParser.o md5c.o TCharOpr.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o GenDeltaNVIndex.o pugixml.o -lstdc++
	sudo cp NVGen ../Bin/NVGen
	
NVGen.o:../Source/NVGen.cpp
	gcc -c -fno-strict-aliasing -Os -o NVGen.o ../Source/NVGen.cpp
FileParser.o:../Source/FileParser.cpp
	gcc -c -fno-strict-aliasing -Os -o FileParser.o ../Source/FileParser.cpp
md5c.o:../Source/md5c.c
	gcc -c -fno-strict-aliasing -Os -o md5c.o ../Source/md5c.c
TCharOpr.o:../Source/TCharOpr.cpp
	gcc -c -fno-strict-aliasing -Os -o TCharOpr.o ../Source/TCharOpr.cpp
tinystr.o:../Source/tinystr.cpp
	gcc -c -fno-strict-aliasing -Os -o tinystr.o ../Source/tinystr.cpp
tinyxml.o:../Source/tinyxml.cpp
	gcc -c -fno-strict-aliasing -Os -o tinyxml.o ../Source/tinyxml.cpp
tinyxmlerror.o:../Source/tinyxmlerror.cpp
	gcc -c -fno-strict-aliasing -Os -o tinyxmlerror.o ../Source/tinyxmlerror.cpp
tinyxmlparser.o:../Source/tinyxmlparser.cpp
	gcc -c -fno-strict-aliasing -Os -o tinyxmlparser.o ../Source/tinyxmlparser.cpp
GenDeltaNVIndex.o:../Source/GenDeltaNVIndex.cpp
	gcc -c -fno-strict-aliasing -Os -o GenDeltaNVIndex.o ../Source/GenDeltaNVIndex.cpp
pugixml.o:../Source/pugixml.cpp
	gcc -c -fno-strict-aliasing -Os -o pugixml.o ../Source/pugixml.cpp

clean:
	rm -f $(objects) NVGen ../Bin/NVGen