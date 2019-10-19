#include"exitButton.h"
#include"gameManager.h"

using namespace vb01;

namespace fsim{
	ExitButton::ExitButton(GameManager *gm, Vector2 pos, Vector2 size) : Button(gm,pos,size,"Exit",true){
		this->gm=gm;
	}

	void ExitButton::onClick(){gm->setRunning(false);}
}
