#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <iostream>
#include <math.h>

class Vertex
{
public:
	Vertex() {}

    	Vertex(float x, float y, float z);

    	float getX() const
	{
		return this->x;
	}

	void setX(float x)
	{
		this->x = x;
	}

	float getY() const
	{
		return this->y;
	}

	void setY(float y)
	{
		this->y = y;
	}

	float getZ() const
	{
		return this->z;
	}

	void setZ(float z)
	{
		this->z = z;
	}
	
	float getMagnitude();
	
	Vertex getNormalized();
	
	float dotProduct(Vertex other);
	
	Vertex crossProduct(Vertex other);
	
	void clear();
	
	Vertex operator+ (Vertex other);
    	
	Vertex operator- (Vertex other);

	float operator* (Vertex other);
	
	Vertex operator* (float scalar);

	friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex);

private:
	float x;
	float y;
	float z;
};

#endif  // VERTEX_H_INCLUDED
