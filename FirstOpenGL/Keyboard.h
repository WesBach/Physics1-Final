#ifndef _Keyboard_HG_
#define _Keyboard_HG_
#include <GLFW/glfw3.h>

class Keyboard {
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};


#endif // !_Keyboard_HG_

