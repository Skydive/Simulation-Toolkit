#include <iostream>
#include <sstream>

#include <map>
#include <string>
#include <functional>


#ifdef __linux__
	#include <dlfcn.h>
	#define SharedLibrary void*
	#define LoadSharedLibrary(lib) dlopen(lib, RTLD_NOW)
	#define FreeSharedLibrary(lib) dlclose(lib)
	#define LoadFunction(dllhandle, fstr) dlsym(dllhandle, fstr)
	#include <SDL.h>
#elif _WIN32
	#include <windows.h>
	#define SharedLibrary HMODULE
	#define LoadSharedLibrary(lib) LoadLibrary(lib)
	#define FreeSharedLibrary(lib) FreeLibrary(lib)
	#define LoadFunction(dllhandle, fstr) GetProcAddress(dllhandle, fstr)
	#include <SDL2/SDL.h>
#endif

#include <cstdlib>

#include <chrono>
#include <cmath>

#include <Simulation.hpp>


static void SDLG_RenderCircle(SDL_Renderer* renderer, int x, int y, int r)
{
	for(int i=-r; i<=r; i++)
		for(int j=-r; j<=r; j++)
			if(pow(i, 2) + pow(j, 2) < pow(r, 2)+1)
				SDL_RenderDrawPoint(renderer, x+i, y+j);
}

static SharedLibrary handle;
static Simulation* S;

static Simulation* CreateSimulation(const std::string& lib)
{
	std::cout << "Loading Shared Library: " << lib << std::endl;

	handle = LoadSharedLibrary(lib.c_str());
	if(handle == NULL)
	{
		std::cout << "\tError loading library: " << lib << std::endl;
		#ifdef __linux__
			std::cerr << dlerror() << std::endl;
		#endif
		exit(1);
	}
	std::cout << "\tLoading Symbols..." << std::endl;
	Simulation* (*create)() = (Simulation* (*)())LoadFunction(handle, "create_object");

	std::cout << "\tLibrary loading completed" << std::endl << std::endl;

	S = create();
	S->Setup();
	return S;
}


static void DestroySimulation(Simulation* s)
{
	void (*destroy)(Simulation*) = (void (*)(Simulation*))LoadFunction(handle, "destroy_object");
	destroy(s);
	FreeSharedLibrary(handle);
}

Vector2i LocationToScreen(Vector2d Location, int w, int h)
{
	Vector2d Screen = {w, h};
	int Dim = Screen.minCoeff();
	Vector2d ScaledLocation = Location * Dim/2;
	Vector2d MovedLocation = ScaledLocation + Screen/2;
	return Vector2i(MovedLocation(0), MovedLocation(1));
}

static void PerformSimulation()
{
	std::cout << "Performing Simulation..." << std::endl;

	int w = S->MD.Width;
	int h = S->MD.Height;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);
	SDL_SetWindowTitle(window, S->MD.Name.c_str());

	auto last = std::chrono::high_resolution_clock::now();
	bool q = false;
	while(!q)
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double>(now - last);
		double dt = duration.count()*S->MD.TimeScale;
		last = now;

		SDL_Event event;
		while(SDL_PollEvent(&event) != 0)
		{
			if(event.type == SDL_QUIT)
				q = true;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for(Particle* P : S->ParticleList)
		{
			Vector2i L = LocationToScreen(P->Location.head<2>(), w, h);
			SDL_SetRenderDrawColor(renderer, P->Color(0), P->Color(1), P->Color(2), 255);
			SDLG_RenderCircle(renderer, L(0), L(1), std::round(std::max(w, h)*P->R));
		}

		SDL_RenderPresent(renderer);
		S->StepTime(dt);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

	S->End();
	DestroySimulation(S);
}

using CommandMap = std::map<std::string, std::function<void(int&, char**)>>;
CommandMap COMMANDLET_LIST = {
	{"-execute",	[](int& i, char** argv) { S = CreateSimulation(std::string(argv[i+1])); PerformSimulation(); }}
};

int main(int argc, char** argv)
{
	for(int i=1; i<argc; i++)
		if(COMMANDLET_LIST.find(std::string(argv[i])) != COMMANDLET_LIST.end())
			COMMANDLET_LIST[std::string(argv[i])](i, argv);
}
