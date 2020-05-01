#include"runway.h"
#include"structureData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"guiAppState.h"
#include"inGameAppState.h"
#include"jetAppState.h"
#include"helipad.h"
#include"aircraft.h"
#include"aircraftSelectionButton.h"

using namespace vb01;
using namespace std;
using namespace fsim::structureData;

namespace fsim{
	Runway::Runway(GameManager *gm, int faction, Vector3 pos, Quaternion rot) : Building(gm,RUNWAY,faction,pos,rot),AbstractRunway(gm,RUNWAY,faction){
	}

	Runway::~Runway(){
	}

	void Runway::update(){
		AbstractRunway::pos=Structure::pos;
		AbstractRunway::rot=Structure::rot;
	}
}
