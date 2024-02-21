#pragma	once 

#include "CubeMap.h"
#include "Ball.h"

struct Indices {
	int x;
	int y;
	int z;
};

struct ContactPoint {
	glm::vec3 point;
	glm::vec3 normal;
	GLfloat dt;
	Indices indices;
};

/* Collision class */
/* collision between ball and cube*/
/* cube has 6 side, 8 corners -> spheres, 12 edges */
class Collision {
public: 

	Collision() {	}

	Collision(CubeMap *cubeMap, Ball *ball) {
		this->map = cubeMap;
		this->ball = ball;
	}

	int getTotalCollisions(){
		return totalCollisions;
	}

	void collide() {
		updateBallIndices();
		contactPoint.dt = -1.0f;

		// check contact point of the ball collision with neighboring cubes
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				for (int k = -1; k < 2; k++) {
					updateCubeIndicesRelativeToBallIndices(i, j, k);
					updateCubePosToRecentIndices();
					if (ballCuttingExistingCube()) {
						updateContactPoint(getContactPointSphere());
						updateContactPoint(getContactPointCylinder());
						updateContactPoint(getContactPointPlane());
					}
				}
			}
		}
		if (contactPoint.dt != -1.0f) {
			Indices foundIndices = contactPoint.indices;

			if (indexInRange(foundIndices.x, foundIndices.y, foundIndices.z)) {
				(*map).cubeProducer[foundIndices.x][foundIndices.y][foundIndices.z] = (*map).noCube;
				totalCollisions += 1;
			}

			(*ball).velocity += 2.0f *glm::dot(-(*ball).velocity, contactPoint.normal) * contactPoint.normal;

			(*ball).virtualPos = contactPoint.point + contactPoint.dt * (*ball).velocity;
			collide();
		}
	}

	

private:
	CubeMap * map;
	Ball * ball;

	Indices ballIndices;
	Indices cubeIndices;
	glm::vec3 cubePos;
	ContactPoint contactPoint;

	int totalCollisions = 0;

	// sets ball indices to the cube which has contact point with center of the ball
	void updateBallIndices() {
		ballIndices.x = (int)floor((*ball).virtualPos.x + 0.5) + (*map).WIDTH / 2;
		ballIndices.y = (int)floor((*ball).virtualPos.y + 0.5) + (*map).HEIGHT / 2;
		ballIndices.z = (int)floor((*ball).virtualPos.z - (*map).VIEW_OFFSET + 0.5) + (*map).DEPTH;
	}

	// sets cube indices to neighboring cubes of the ball
	void updateCubeIndicesRelativeToBallIndices(int i, int j, int k) {
		cubeIndices.x = ballIndices.x + i;
		cubeIndices.y = ballIndices.y + j;
		cubeIndices.z = ballIndices.z + k;
	}

	bool ballCuttingExistingCube() {
		return cubeExists() && ballCuttingCube();
	}

	bool cubeExists() {
		int x = cubeIndices.x;
		int y = cubeIndices.y;
		int z = cubeIndices.z;
		if (x < 0 || y < 0 || z < 0) {
			return true;
		}
		if (x >= (*map).WIDTH || y >= (*map).HEIGHT) {
			return true;
		}
		if (indexInRange(x, y, z)) {
			return typeid(*(*map).cubeProducer[x][y][z]) != typeid(*(*map).noCube);
		}
		return false;
	}

	bool indexInRange(int x, int y, int z) {
		bool inRange = (x > -1 && x < (*map).WIDTH && y > -1 && y < (*map).HEIGHT && z > -1 && z < (*map).DEPTH);
		return inRange;
	}

	bool ballCuttingCube() {
		// check cut with sides of the cube
		for (int i = 0; i < 3; i++) {
			float x = (*ball).virtualPos[i] - cubePos[i];
			if (abs(x) < 1.0f) {
				int index_1 = (i + 1) % 3;
				int index_2 = (i + 2) % 3;
				float y = (*ball).virtualPos[index_1] - cubePos[index_1];
				float z = (*ball).virtualPos[index_2] - cubePos[index_2];
				if (abs(y) < 0.5f && abs(z) < 0.5f) {
					return true;
				}
			}
		}

		// check cut with spheres -> 8 corners of the cube
		for (int i = 0; i < 8; i++) {
			// get all possible combination of (sphere_x, sphere_y, sphere_z) with entries of -0.5 or 0.5
			float sphere_x = i % 2 - 0.5f;
			float sphere_y = (i / 2) % 2 - 0.5f;
			float sphere_z = (i / 4) - 0.5f;

			float x = (*ball).virtualPos.x - (cubePos.x + sphere_x);
			float y = (*ball).virtualPos.y - (cubePos.y + sphere_y);
			float z = (*ball).virtualPos.z - (cubePos.z + sphere_z);
			if (x * x + y * y + z * z < 0.25) {
				return true;
			}
		}

		//check for cylinders 12 edges
		for (int i = 0; i < 3; i++) {
			int index_1 = (i + 1) % 3;
			int index_2 = (i + 2) % 3;
			// 4 edges
			for (int j = 0; j < 4; j++) {
				float cylinder_x = j % 2 - 0.5;
				float cylinder_y = j / 2 - 0.5;

				float x = (*ball).virtualPos[index_1] - (cubePos[index_1] + cylinder_x);
				float y = (*ball).virtualPos[index_2] - (cubePos[index_2] + cylinder_y);
				if ((x * x + y * y) < 0.25) {
					float z = (*ball).virtualPos[i] - cubePos[i];
					if (abs(z) < 0.5) {
						return true;
					}
				}
			}
		}

		return false;
		}

	void updateContactPoint(ContactPoint point){
		if (contactPoint.dt < point.dt && point.dt > 0) {
			contactPoint = point;
			contactPoint.indices = cubeIndices;
		}
	}

	ContactPoint getContactPointCylinder() {
		ContactPoint point;

		for (int i = 0; i < 3; i++) {
			int index_1 = (i + 1) % 3;
			int index_2 = (i + 2) % 3;

			for (int j = 0; j < 4; j++) {
				float cylinder_x = j % 2 - 0.5;
				float cylinder_y = j / 2 - 0.5;

				float x = (*ball).virtualPos[index_1] - (cubePos[index_1] + cylinder_x);
				float y = (*ball).virtualPos[index_2] - (cubePos[index_2] + cylinder_y);

				float vel1 = -(*ball).velocity[index_1];
				float vel2 = -(*ball).velocity[index_2];

				float p = 2.0f * (x * vel1 + y * vel2) / (vel1 * vel1 + vel2 * vel2);
				float q = (x * x + y * y - 0.25) / (vel1 * vel1 + vel2 * vel2);
				float t = pq_equation(p, q);
				if (t > 0) {
					point.dt = t;
					glm::vec3 vpoint = (*ball).virtualPos - t * (*ball).velocity;
					if (abs(vpoint[i] - cubePos[i]) < 0.5) {
						point.point = vpoint;
						glm::vec3 normal;
						normal[index_1] = x;
						normal[index_2] = y;
						normal[i] = 0.0f;
						point.normal = glm::normalize(normal);
						return point;
					}
				}
			}
		}
		point.dt = -1.0;
		return point;
	}

	ContactPoint getContactPointSphere() {
		ContactPoint point;
		for (int i = 0; i < 8; i++) {

			float sphere_x = i % 2 - 0.5f;	
			float sphere_y = (i / 2) % 2 - 0.5f;
			float sphere_z = (i / 4) - 0.5f;

			float x = (*ball).virtualPos.x - (cubePos.x + sphere_x);
			float y = (*ball).virtualPos.y - (cubePos.y + sphere_y);
			float z = (*ball).virtualPos.z - (cubePos.z + sphere_z);

			float velocity_x = -(*ball).velocity.x;
			float velocity_y = -(*ball).velocity.y;
			float velocity_z = -(*ball).velocity.z;

			float p = 2.0f * (x * velocity_x + y * velocity_y + z * velocity_z) / (velocity_x * velocity_x + velocity_y * velocity_y + velocity_z * velocity_z);
			float q = (x * x + y * y + z * z - 0.25) / (velocity_x * velocity_x + velocity_y * velocity_y + velocity_z * velocity_z);

			float t = pq_equation(p, q);

			if (t >= 0) {
				point.dt = t;
				glm::vec3 vpoint = (*ball).virtualPos - t * (*ball).velocity;
				point.point = vpoint;
				glm::vec3 normal;
				normal[0] = vpoint.x - (cubePos.x + sphere_x);
				normal[1] = vpoint.y - (cubePos.y + sphere_y);
				normal[2] = vpoint.z - (cubePos.z + sphere_z);

				point.normal = glm::normalize(normal);
				return point;
			}
		}
		point.dt = -1.0;
		return point;
	}

	ContactPoint getContactPointPlane() {
		ContactPoint point; 
		point.dt = -1.0;	// if no contact point -> -1
		for (int i = 0; i < 3; i++) {
			setContactPointIfExists(point, cubePos[i] + 1, i);
			setContactPointIfExists(point, cubePos[i] - 1, i);
		}
		return point;
	}

	void setContactPointIfExists(ContactPoint &point, float planePos, int i) {
		int k = (i + 1) % 3;
		int l = (i + 2) % 3;
		float t = (planePos - (*ball).virtualPos[i]) / -(*ball).velocity[i];

		glm::vec3 contactPoint = (*ball).virtualPos - t * (*ball).velocity;

		if (abs(contactPoint[k] - cubePos[k]) < 0.5 && abs(contactPoint[l] - cubePos[l]) < 0.5) {
			if (t > 0) {
				point.normal = glm::vec3(0.0f);
				point.normal[i] = 1.0f;
				point.point = contactPoint;
				point.dt = t;
			}
		}
	}

	void updateCubePosToRecentIndices() {
		cubePos = (*map).getCubePosition(cubeIndices.x, cubeIndices.y, cubeIndices.z);
	}
	
	double pq_equation(double p, double q) {
		if ((p * p / 4.0f) - q >= 0) {
			return (-p / 2.0f) + sqrt((p * p / 4.0f) - q);
		}
		return -1.0;
	}
};