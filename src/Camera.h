
#pragma once
#include "Human.hpp"
#include "env.hpp"

class Camera {
public:
	Vec3    pos;
	Vec3    dir;
	Vec3    right;
	Vec3    up;
	Matrix  proj;
	Matrix  view;
	int     width;
	int     height;
	float	mouseXpos;
	float	mouseYpos;
	float 	oldmouseXpos;
	float   oldmouseYpos;

	Camera(Vec3 pos = Vec3(), Vec3 rot = Vec3(), int width = 0, int height = 0);
	void update(Env &env);
	void update_matrix();
private:
	//Vec3 target;
	bool  mouseMoved = false;
	float horAngle;
	float verAngle;
	float speed = 3.0f;
	float deltaTime;
	float lastTime;
};


