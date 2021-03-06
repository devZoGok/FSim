#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include<string>

namespace vb01{
	class Root;
}

namespace sf{
	class Music;
}

namespace fsim{
	class InputManager;
	class StateManager;

	class GameManager{
		public:
			struct Options{
				int width,height,textureLevel;
				float sfxVolume,musicVolume;
				std::string databaseUser;
			};

			GameManager();
			~GameManager();
			void update();
			void readOptions();
			inline vb01::Root* getRoot(){return root;}
			inline bool isRunning(){return running;}
			inline void setRunning(bool r){this->running=r;}
			inline int getWidth(){return width;}
			inline int getHeight(){return height;}
			inline StateManager* getStateManager(){return stateManager;}
			inline Options& getOptions(){return options;}
			inline sf::Music* getMusic(){return music;}
		private:
			sf::Music *music;
			Options options;
			bool running=false;
			int width,height;
			vb01::Root *root;
			InputManager *inputManager=nullptr;
			StateManager *stateManager=nullptr;
	};
}

#endif
