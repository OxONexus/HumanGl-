#include "Camera.h"

Camera::Camera(Vec3 position, Vec3 targetPosition, int w, int h)
{
    pos = position;

    width = w;
    height = h;

	(void)targetPosition;
	Vec3 direction = (targetPosition - position);
	direction.normalize();
    mouseXpos = ((float)w / 2.0f);
    mouseYpos = ((float)h / 2.0f);

  verAngle = asinf(direction.y);
  horAngle = atan2(direction.x, direction.z);
    proj = projMatrix(45.0f * (M_PI / 180.0f), ((float)w /(float)h));
    update_matrix();
}

void Camera::update_matrix(){
    double currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);
    lastTime = (float)currentTime;
    if (mouseMoved) {
        horAngle += 0.5f * 0.0001 * (oldmouseXpos - mouseXpos);
        verAngle += 0.5f * 0.0001 * (oldmouseYpos - mouseYpos);
		if (horAngle > M_PI / 2.0f) {
			horAngle = M_PI / 2.0f;
		}
		if (horAngle < -(M_PI / 2.0f)) {
			horAngle = -M_PI / 2.0f;
		}
		if (verAngle > M_PI / 2.0f) {
			verAngle = M_PI / 2.0f;
		}
		if (verAngle < -(M_PI / 2.0f)) {
			verAngle = -M_PI / 2.0f;
		}
        mouseMoved = false;
    }
    dir = Vec3(cos(verAngle) * sin(horAngle),
               sin(verAngle),
               cos(verAngle) * cos(horAngle));
	dir.normalize();
    right = Vec3(sin(horAngle - 3.14f/2.0f),
                 0,
                 cos(horAngle - 3.14f/2.0f));
	right.normalize();
    up = right.cross(dir);
	up.normalize();
    view = viewMatrix(pos,
                      dir + pos,
                      up);
}

Matrix getMVP(Matrix model, Matrix view, Matrix proj){
    return (model * view * proj);
}

void Camera::update(Env &env) {
	if (env.inputHandler.keys[GLFW_KEY_LEFT_SHIFT]) {
		speed = 9.0f;
	} else {
		speed = 3.0f;
	}
    if (env.inputHandler.keys[GLFW_KEY_UP] || env.inputHandler.keys[GLFW_KEY_W]){
        Vec3 tmp = dir * speed * env.getDeltaTime();
        pos = pos + tmp;
    }
    if (env.inputHandler.keys[GLFW_KEY_DOWN] || env.inputHandler.keys[GLFW_KEY_S]){
        Vec3 tmp = dir * speed * env.getDeltaTime();
        pos = pos - tmp;
    }
    if (env.inputHandler.keys[GLFW_KEY_RIGHT] || env.inputHandler.keys[GLFW_KEY_D]){
        Vec3 right = up.cross(dir);
        Vec3 tmp = right * speed * env.getDeltaTime();
        pos = pos - tmp;
    }
    if (env.inputHandler.keys[GLFW_KEY_LEFT] || env.inputHandler.keys[GLFW_KEY_A]){
        Vec3 right = up.cross(dir);
        Vec3 tmp = right * speed * env.getDeltaTime();
        pos = pos + tmp;
    }
	if (env.inputHandler.mousex != mouseXpos || env.inputHandler.mousey != mouseYpos) {
		oldmouseXpos = mouseXpos;
		oldmouseYpos = mouseYpos;
		mouseXpos = env.inputHandler.mousex;
		mouseYpos = env.inputHandler.mousey;
		mouseMoved = true;
	}
    update_matrix();
}
