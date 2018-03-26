#include "env.hpp"
#include "Renderer.hpp"

int main(int argc, char *argv[])
{
	Env env(1280, 720);
	Renderer render(env);
	Charactere test;

	while (glfwWindowShouldClose(env.window) == false) {
		env.update();
		glClearColor(0.f,0.f,0.f,1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render.update(env);
		glfwSwapBuffers(env.window);
		GL_DUMP_ERROR("main");
		if (glfwGetKey (env.window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(env.window, 1);
		}
        glfwPollEvents();
	}
    glfwTerminate();
	return 0;
}
