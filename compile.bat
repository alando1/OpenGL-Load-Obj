g++ -std=c++11 objLoader.cpp Mesh.cpp SuppliedGlutFuncs.cpp -o run -O3 -I"./include" -L"./lib"  -lSDL2 -lSDL2main -lSDL2_mixer -lfreeglut -lSOIL -lopengl32 -lglu32 -Wno-strict-aliasing -Wno-comment

@echo off
echo(

