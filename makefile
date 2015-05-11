CC              := g++
CFLAGS          := -I/usr/local/include/opencv -L/usr/local/lib -D_REENTERANT -Wl,-rpath,/usr/local/lib
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_flann -lpthread fopenmp

.PHONY: all clean

all: lr

lr: 
	$(CC) $(CFLAGS) -o lr1 lr1.cpp $(LIBRARIES)
	$(CC) $(CFLAGS) -o lr2 lr2.cpp $(LIBRARIES)
	mpicc $(CFLAGS) -o lr3 lr3.c $(LIBRARIES)
        
clean:
	rm -f *.o


