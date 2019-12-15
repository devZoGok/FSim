#ifndef ABSTRACT_APP_STATE_H
#define ABSTRACT_APP_STATE_H

#include"defConfigs.h"
#include"mapping.h"
#include<vector>

namespace fsim{
	class GameManager;

	class AbstractAppState{
		public:
			enum Type{GUI_STATE,IN_GAME_STATE,JET_STATE,HELICOPTER_STATE};

			virtual void update();
			virtual void onAttached();
			virtual void onDettached();
			inline Type getType(){return type;}
			inline Mapping* getMapping(int id){return mappings[id];}
			inline std::vector<Mapping*>& getMappings(){return mappings;}
			inline int getNumMappings(){return mappings.size();}
			inline bool isAttached(){return attached;}
			void removeMapping(Mapping::Bind);
			virtual void onAction(Mapping::Bind,bool){}
			virtual void onAnalog(Mapping::Bind,float){}
			virtual void onRawKeyButton(short){}
			virtual void onRawMouseButton(short){}
			virtual void onRawJoystickAxis(short,float){}
			virtual void onRawJoystickButton(short){}
		protected:
			AbstractAppState(GameManager*);
			~AbstractAppState();
			GameManager *gm;
			std::vector<Mapping*> mappings;
			Type type;
			bool attached=false;
	};
}

#endif
