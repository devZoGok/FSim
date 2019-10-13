#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

namespace vb01{
	class Root;
}

namespace fsim{
	class InputManager;
	class StateManager;

	class GameManager{
		public:
			GameManager();
			~GameManager();
			void update();
			inline vb01::Root* getRoot(){return root;}
			inline StateManager* getStateManager(){return stateManager;}
		private:
			int width,height;
			vb01::Root *root;
			InputManager *inputManager=nullptr;
			StateManager *stateManager=nullptr;
	};
}

#endif
