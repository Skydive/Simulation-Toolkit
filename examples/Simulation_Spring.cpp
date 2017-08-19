#include <Simulation.hpp>

#include <iostream>

extern "C" Simulation* create_object()
{
	return new Simulation();
}

extern "C" void destroy_object(Simulation* object)
{
	delete object;
}

static Particle* P;
static Vector3d SpringOrigin = {-1.0, 0.0, 0.0};
static double k = 1.0;
static Vector3d l = {1.0, 0.0, 0.0};

void Simulation::Setup()
{
	this->MD = {"Spring", 512, 256, 1.0};

	P = new Particle();
	this->ParticleList.push_back(P);

	P->R = 0.01;

	P->Color = {255, 255, 255};

	P->Mass = 1.0;
	P->Location = {1.0, 0.0, 0.0};
}

void Simulation::PreUpdate(double dt)
{
	Vector3d x = (P->Location - (SpringOrigin+l));
	P->ApplyForce(-k*x, dt);
}

void Simulation::End()
{
	for(Particle* P : this->ParticleList)
		delete P;
}
