#ifndef ABSTRACT_APP_STATE_H
#define ABSTRACT_APP_STATE_H

#include"mapping.h"
#include<vector>

namespace fsim{
	class AbstractAppState{
		public:
			enum Type{GUI_STATE,IN_GAME_STATE};

			virtual void update();
			virtual void onAttached(){attached=true;}
			virtual void onDettached(){attached=false;}
			inline Type getType(){return type;}
			inline Mapping* getMapping(int id){return mappings[id];}
			inline std::vector<Mapping*>& getMappings(){return mappings;}
			inline int getNumMappings(){return mappings.size();}
			inline bool isAttached(){return attached;}
			virtual void onAction(Mapping::Bind,bool){}
			virtual void onAnalog(Mapping::Bind,float){}
		protected:
			AbstractAppState();
			~AbstractAppState();
			std::vector<Mapping*> mappings;
			Type type;
			bool attached=false;
	};
}

#endif
