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

static Particle* P, * Q;
static Vector3d SpringOrigin1 = {-1.5, 0.0, 0.0};
static Vector3d SpringOrigin3 = {1.5, 0.0, 0.0};
static double k1 = 1.0;
static double k2 = 1.0;
static double k3 = 1.0;
static Vector3d l1 = {1.0, 0.0, 0.0};
static Vector3d l3 = {1.0, 0.0, 0.0};

void Simulation::Setup()
{
	this->MD = {"Coupled", 512, 256, 1.0};

	P = new Particle();
	this->ParticleList.push_back(P);

	Q = new Particle();
	this->ParticleList.push_back(Q);

	P->R = 0.01;
	Q->R = 0.01;

	P->Color = {0, 255, 0};
	Q->Color = {255, 0, 0};

	double dx = 0.5;
	P->Mass = 1.0;
	P->Location = {0.5+dx, 0.0, 0.0};

	Q->Mass = 1.0;
	Q->Location = {-0.5, 0.0, 0.0};
}

void Simulation::PreUpdate(double dt)
{
	Vector3d x1 = (Q->Location - (SpringOrigin1+l1));
	Vector3d x2 = (P->Location - (SpringOrigin3-l3));
	P->ApplyForce(-k3*x2 - k2*(x2-x1), dt);
	Q->ApplyForce(-k1*x1 + k2*(x2-x1), dt);
}

void Simulation::End()
{
	for(Particle* P : this->ParticleList)
		delete P;
}
