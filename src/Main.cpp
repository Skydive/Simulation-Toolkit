#include <iostream>
#include <sstream>

#include <map>
#include <string>
#include <functional>

#include <dlfcn.h>
#include <cstdlib>

#include <chrono>
#include <cmath>
#include <SDL.h>

#include <Simulation.hpp>




static void SDLG_RenderCircle(SDL_Renderer* renderer, int x, int y, int r)
{
	for(int i=-r; i<=r; i++)
		for(int j=-r; j<=r; j++)
			if(pow(i, 2) + pow(j, 2) < pow(r, 2))
				SDL_RenderDrawPoint(renderer, x+i, y+j);
}

static void* handle;
static Simulation* S;

static Simulation* CreateSimulation(const std::string& lib)
{
	std::cout << "Loading Shared Library: " << lib << std::endl;
	handle = dlopen(lib.c_str(), RTLD_NOW);
	if(handle == nullptr)
	{
		std::cout << "\tError loading library: " << lib << std::endl;
		std::cerr << dlerror() << std::endl;
		exit(1);
	}
	std::cout << "\tLoading Symbols..." << std::endl;
	Simulation* (*create)() = (Simulation* (*)())dlsym(handle, "create_object");

	std::cout << "\tLibrary loading completed" << std::endl << std::endl;

	S = create();
	S->Setup();
	return S;
}


static void DestroySimulation(Simulation* s)
{
	void (*destroy)(Simulation*) = (void (*)(Simulation*))dlsym(handle, "destroy_object");
	destroy(s);
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
			Vector2d T = {w/2, h/2};
			Vector2d TL = P->Location.head<2>() + T;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDLG_RenderCircle(renderer, TL(0), TL(1), std::round(std::max(w, h)*P->R));
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
	{"-create",		[](int& i, char** argv) { S = CreateSimulation(std::string(argv[i+1])); }},
	{"-execute",	[](int& i, char** argv) { PerformSimulation(); }}
};

int main(int argc, char** argv)
{
	for(int i=1; i<argc; i++)
		if(COMMANDLET_LIST.find(std::string(argv[i])) != COMMANDLET_LIST.end())
			COMMANDLET_LIST[std::string(argv[i])](i, argv);
}
