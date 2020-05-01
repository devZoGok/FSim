#include"helipad.h"
#include"structureData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"guiAppState.h"
#include"helicopterAppState.h"
#include"runway.h"
#include"aircraft.h"
#include"aircraftSelectionButton.h"

using namespace vb01;
using namespace std;
using namespace fsim::structureData;

namespace fsim{
	Helipad::Helipad(GameManager *gm, int faction, Vector3 pos) : Building(gm,HELIPAD,faction,pos,Quaternion(1,0,0,0)),AbstractHelipad(gm,HELIPAD,faction){
	}

	Helipad::~Helipad(){
	}

	void Helipad::update(){
		AbstractHelipad::pos=this->pos;
		AbstractHelipad::rot=this->rot;
	}
}
