#include"inGameAppState.h"
#include"gameManager.h"
#include"map.h"
#include"defConfigs.h"
#include"structure.h"
#include"unit.h"

using namespace std;

namespace fsim{
	InGameAppState::InGameAppState(GameManager *gm,int faction) : AbstractAppState(gm){
		type=AbstractAppState::IN_GAME_STATE;
		this->faction=(Faction)faction;

		string factionSuffix;
		switch(this->faction){
			case CHINA:
				factionSuffix="Ch/Level01/";
				break;
			case JAPAN:
				factionSuffix="Jp/Level01/";
				break;
			case KOREA:
				factionSuffix="Kr/Level01/";
				break;
		}
		map=new Map(gm,PATH+"Models/Levels/"+factionSuffix,this);
	}

	InGameAppState::~InGameAppState(){}

	void InGameAppState::onAttached(){
		AbstractAppState::onAttached();
	}

	void InGameAppState::onDettached(){
		AbstractAppState::onDettached();
	}

	void InGameAppState::update(){
		map->update();
		for(Structure *s : structures)
			s->update();
	}

	void InGameAppState::onAction(Mapping::Bind bind, bool isPressed){
		switch(bind){
		
		}
	}

	void InGameAppState::onAnalog(Mapping::Bind bind, float str){
		switch(bind){
		
		}
	}
}
