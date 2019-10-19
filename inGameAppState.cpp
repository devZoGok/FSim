#include"inGameAppState.h"
#include"map.h"
#include"defConfigs.h"

using namespace std;

namespace fsim{
	InGameAppState::InGameAppState(int faction){
		this->faction=(Faction)faction;
		string factionSuffix;
		switch(this->faction){
			case CHINA:
				factionSuffix="Ch/lvl01.cfg";
				break;
			case JAPAN:
				factionSuffix="Jp/lvl01.cfg";
				break;
			case KOREA:
				factionSuffix="Kr/lvl01.cfg";
				break;
		}
		map=new Map(PATH+"Models/Levels/"+factionSuffix);
	}

	InGameAppState::~InGameAppState(){}

	void InGameAppState::onAttached(){}

	void InGameAppState::onDettached(){}

	void InGameAppState::update(){}

	void InGameAppState::onAction(Mapping::Bind bind, bool isPressed){
		switch(bind){
		
		}
	}

	void InGameAppState::onAnalog(Mapping::Bind bind, float str){
		switch(bind){
		
		}
	}
}
