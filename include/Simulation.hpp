#pragma once

#include <vector>
#include <string>
#include <functional>

#include <Eigen/Core>
using namespace Eigen;

struct MetaData
{
	std::string Name;
	int Width;
	int Height;
	double TimeScale;
};

struct Particle
{
	Vector3d Location;
	Vector3d Velocity;
	double Mass;

	// Display
	double R;
	Vector3i Color;

	void ApplyForce(Vector3d Force, double dt)
	{
		this->Velocity += Force*dt / this->Mass;
	};

	void Update(double dt)
	{
		this->Location += this->Velocity*dt;
	};
};

class Simulation
{
public:
	double Time;
	MetaData MD;
	std::vector<Particle*> ParticleList;

	Simulation()
	{
		Time = 0;
	}

	virtual void Setup();
	virtual void PreUpdate(double dt);
	virtual void End();

	void StepTime(double dt)
	{
		PreUpdate(dt);
		for(Particle* P : this->ParticleList)
		{
			P->Update(dt);
		}
		this->Time += dt;
	}
};
