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

static Particle* A, * B;

void Simulation::Setup()
{
	this->MD = {"Default", 512, 256, 1.0};

	A = new Particle();
	this->ParticleList.push_back(A);

	B = new Particle();
	this->ParticleList.push_back(B);

	A->R = 0.01;
	B->R = 0.01;

	A->Color = {255, 0, 0};
	B->Color = {0, 255, 0};

	A->Mass = 1.0;
	A->Location = {2.0, 0.0, 0.0};

	B->Mass = 1.0;
	B->Location = {-2.0, 0.0, 0.0};
}

void Simulation::PreUpdate(double dt)
{
	Vector3d AB = (B->Location - A->Location);
	Vector3d Direction = AB.normalized();
	A->ApplyForce(Direction*(A->Mass * B->Mass), dt);
	B->ApplyForce(-Direction*(A->Mass * B->Mass), dt);
}

void Simulation::End()
{
	for(Particle* P : this->ParticleList)
		delete P;
}
