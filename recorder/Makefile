CC=g++
CCOPTS=-O3 -Wall
RECORDER_SOURCE=LMRecorder.cpp
INCLUDEDIR=/home/oli/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/include
LIBDIR=/home/oli/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/lib/x64
LIBS=Leap

recorder:
	$(CC) $(CCOPTS) -I $(INCLUDEDIR) -L $(LIBDIR) -l$(LIBS) -o lm_recorder $(RECORDER_SOURCE) 

test:
	$(CC) $(CCOPTS) -c -I $(INCLUDEDIR) -L $(LIBDIR) -l$(LIBS) LMReader.cpp
	$(CC) $(CCOPTS) -o test LMReader.o test.cpp

clean:
	rm lm_recorder test LMReader.o
