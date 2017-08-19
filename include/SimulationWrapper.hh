#pragma once

struct Vector2
{
	double x, y;
};

struct Vector3
{
	double x, y, z;
};

void VectorAdd(Vector2* out, Vector2* a, Vector2* b)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
}

void VectorSub(Vector2* out, Vector2* a, Vector2* b)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
}

double VectorDot(Vector2* a, Vector2* b)
{
	return a->x*b->x + a->y*b->y;
}

void VectorScale(Vector2* out, Vector2* a, double b)
{
	out->x = b*a->x;
	out->y = b*a->y;
}

void ApplyForce(Vector2* v, Vector2* force, double dt)
{
	v->x = force->x*dt;
	v->y = force->y*dt;
}

struct CParticle
{
	Vector2 Location;
	Vector2 Velocity;
	double Mass;

	double R;
	Vector3 Color;
};

struct MetaData
{
	std::string Name;
	int Width;
	int Height;
	double TimeScale;
};
