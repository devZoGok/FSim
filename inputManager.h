#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

class GLFWwindow;

namespace fsim{
	class GameManager;
	class StateManager;

	class InputManager{
		public:
			InputManager(GameManager*,GLFWwindow*);
			~InputManager();
			void update();
		private:
			GameManager *gm;
			StateManager *stateManager;
			GLFWwindow *window;
	};
}

#endif
