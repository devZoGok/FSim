#include"aircraftCarrier.h"
#include"structureData.h"
#include"aircraftCarrierData.h"
#include"abstractRunway.h"
#include"abstractHelipad.h"

using namespace vb01;
using namespace fsim::structureData;

namespace fsim{
	AircraftCarrier::AircraftCarrier(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot) : Unit(gm,id,faction,pos,rot){
		runway=new AbstractRunway(gm,id,faction);
		helipad=new AbstractHelipad(gm,id,faction);
		runway->setAbstractHelipad(helipad);
		helipad->setAbstractRunway(runway);
		runway->setStartPos(pos+aircraftCarrierData::startPos[id-(int)CHINESE_CARRIER]);
		runway->setStartRot(rot);
		helipad->setStartPos(pos+aircraftCarrierData::startPos[id-(int)CHINESE_CARRIER]);
		helipad->setStartRot(rot);
	}
	
	AircraftCarrier::~AircraftCarrier(){
	}

	void AircraftCarrier::update(){
		runway->setStartPos(pos+aircraftCarrierData::startPos[id-(int)CHINESE_CARRIER]);
		runway->setStartRot(rot);
		helipad->setStartPos(pos+aircraftCarrierData::startPos[id-(int)CHINESE_CARRIER]);
		helipad->setStartRot(rot);
		Unit::update();
		runway->update();
		helipad->update();
	}
}
