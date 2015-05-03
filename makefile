CC              := g++
CFLAGS          := -I/usr/local/include/opencv -L/usr/local/lib -Wl,-rpath,/usr/local/lib
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_flann

.PHONY: all clean

all: test

test: 
	$(CC) $(CFLAGS) -o lr1 main.cpp $(LIBRARIES)
        
clean:
	rm -f *.o


