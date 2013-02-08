
#include "Ray.h"

/*Ray::Ray()
{

}*/

Ray::Ray(Vector3 origin, Vector3 direction)
{
	this->origin = origin;
	this->direction = direction;
	//this->directionInverse = Vector3(1/direction.x, 1/direction.y, 1/direction.z);
}

/*
void Ray::plotRay() const
{

	glBegin(GL_POINTS);
	glPointSize(4);
	QColor(1,1,0);
	glColor3f(1,0,0);
	glVertex3d(point.x, point.y, point.z);
	glEnd();
	glPointSize(2);
	glBegin(GL_LINES);
	glColor3f(1,1,0);
	glVertex3d(point.x, point.y, point.z);
	glVertex3d(point.x + dirRay.x, point.y + dirRay.y, point.z + dirRay.z);
	glEnd();
};
*/

const Vector3 & Ray::getDirection() const
{
	return direction;
}

const Vector3 & Ray::getOrigin() const
{
	return origin;
}
/*
const Vector3& Ray::getDirectionInverse() const
{
	return directionInverse;
}
*/