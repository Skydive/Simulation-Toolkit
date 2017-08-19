SET LIBS=-lmingw32 -lSDL2main -lSDL2
SET INCS=-Iinclude
SET FLAGS=%LIBS% %INCS% -std=c++1z -mstackrealign
g++ src/Main.cpp -o build/main.exe %FLAGS%
