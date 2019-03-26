#include "MovingPlatform.h"

void MovingPlatform::setPatrolPoints(int point1, int point2) {
	if (point1 <= point2) {
		this->patrol1 = point1;
		this->patrol2 = point2;
	}
	else {
		this->patrol1 = point2;
		this->patrol2 = point1;
	}

}

