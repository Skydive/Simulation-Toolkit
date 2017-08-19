SET LIBS=-lmingw32 -lSDL2main -lSDL2
SET INCS=-Iinclude
SET FLAGS=%LIBS% %INCS% -std=c++1z -mstackrealign
g++ examples/Simulation_Default.cpp -o build/Simulation_Default.dll -shared %FLAGS%
g++ examples/Simulation_Spring.cpp -o build/Simulation_Spring.dll -shared %FLAGS%
g++ examples/Simulation_Coupled.cpp -o build/Simulation_Coupled.dll -shared %FLAGS%
