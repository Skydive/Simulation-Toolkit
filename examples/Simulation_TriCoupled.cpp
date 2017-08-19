#include <Simulation.hpp>

#include <iostream>

#include <Eigen/Geometry>

extern "C" Simulation* create_object()
{
	return new Simulation();
}

extern "C" void destroy_object(Simulation* object)
{
	delete object;
}

Vector3d FromPolar(Vector3d in, double cwiseangle)
{
	Rotation2Dd T(-cwiseangle * M_PI / 180);
	Vector2d out = T*in.head<2>();
	return Vector3d(out(0), out(1), in(2));
}

static Particle* P, * Q, * R;
static double k1 = 1.0;
static double k2 = 1.0;
static double k3 = 1.0;
static Vector3d PL = FromPolar({0.5, 0.0, 0.0}, 120.0*0.0);
static Vector3d QL = FromPolar({0.5, 0.0, 0.0}, 120.0*1.0);
static Vector3d RL = FromPolar({0.5, 0.0, 0.0}, 120.0*2.0);
static double l = 0.5*sqrt(3);

void Simulation::Setup()
{
	this->MD = {"Coupled Triangle", 512, 512, 1.0};

	P = new Particle();
	Q = new Particle();
	R = new Particle();
	this->ParticleList.push_back(P);
	this->ParticleList.push_back(Q);
	this->ParticleList.push_back(R);

	P->Mass = 1.0;
	Q->Mass = 1.0;
	R->Mass = 1.0;

	P->R = 0.01;
	Q->R = 0.01;
	R->R = 0.01;

	P->Color = {0, 255, 0};
	Q->Color = {255, 0, 0};
	R->Color = {0, 0, 255};

	P->Location = PL-Vector3d(0.2, 0.0, 0.0);
	Q->Location = QL;
	R->Location = RL;
}

void Simulation::PreUpdate(double dt)
{
	Vector3d PQ = (Q->Location - P->Location);
	Vector3d QR = (R->Location - Q->Location);
	Vector3d RP = (P->Location - R->Location);
	Vector3d PQd = PQ.normalized()*(PQ.norm() - l);
	Vector3d QRd = QR.normalized()*(QR.norm() - l);
	Vector3d RPd = RP.normalized()*(RP.norm() - l);

	P->ApplyForce(k1*PQd - k3*RPd, dt);
	Q->ApplyForce(k2*QRd - k1*PQd, dt);
	R->ApplyForce(k3*RPd - k2*QRd, dt);
}

void Simulation::End()
{
	for(Particle* P : this->ParticleList)
		delete P;
}
