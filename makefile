COMPILEFLAGS = -std=c++11 -I"./include" -O3
COMPILECMD = g++ -c $(COMPILEFLAGS)
LINKFLAGS =   
LINKCMD = g++ $(LINKFLAGS) $(OBJS)  $(LIBRARIES) -o $(TARGET)
LIBRARIES = -lSDL2main -lSDL2 -lGLU -lGL -lSOIL -lglut -lSDL2_mixer
#LIBRARIES = -lSDL2 -lSDL2main -lSDL2_mixer -lfreeglut -lSOIL -lopengl32 -lglu32 
OBJS = SuppliedGlutFuncs.o objLoader.o
TARGET = run

main:   $(OBJS)
		$(LINKCMD)

clean:
	rm *.o

SuppliedGlutFuncs.o: SuppliedGlutFuncs.cpp SuppliedGlutFuncs.h
	$(COMPILECMD) SuppliedGlutFuncs.cpp

objLoader.o: objLoader.cpp
	$(COMPILECMD) objLoader.cpp