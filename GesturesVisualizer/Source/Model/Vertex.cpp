#include "Vertex.h"

Vertex::Vertex(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vertex::clear()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

float Vertex::getMagnitude()
{
	return sqrt(getX() * getX()
		+ getY() * getY()
		+ getZ() * getZ());
}

Vertex Vertex::getNormalized()
{
	float magnitude = getMagnitude();
	if (magnitude == 0.0)
		return *this;

	return Vertex(getX()/magnitude,
		getY()/magnitude,
		getZ()/magnitude);
}

float Vertex::dotProduct(Vertex other)
{
	return getX() * other.getX()
		+ getY() * other.getY()
		+ getZ() * other.getZ();
}
	
Vertex Vertex::crossProduct(Vertex other)
{
	return Vertex(getY() * other.getZ() - getZ() * other.getY(),
		getZ() * other.getX() - getX() * other.getZ(),
		getX() * other.getY() - getY() * other.getX());
}

Vertex Vertex::operator+ (Vertex other)
{
	return Vertex(getX() + other.getX(),
		getY() + other.getY(),
		getZ() + other.getZ());
}
    	
Vertex Vertex::operator- (Vertex other)
{
	return Vertex(getX() - other.getX(),
		getY() - other.getY(),
		getZ() - other.getZ());
}

float Vertex::operator* (Vertex other)
{
	return dotProduct(other);
}

std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
{
	os<<vertex.getX()<<";";
	os<<vertex.getY()<<";";
	os<<vertex.getZ();

	return os;
}
