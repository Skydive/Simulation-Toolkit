#include <SimulationWrapper.hh>

void SetMetadata(char* wname, int len, int* w, int* h, double* dt)
{
	sprintf(wname, "Default");
	*w = 512; *h = 256;
	*dt = 1.0;
}

static CParticle* A, *B;

void Setup()
{
	A = CParticle();
	A.
}

void Update(double dt)
{

}
