#include<root.h>
#include<quad.h>
#include<box.h>
#include<node.h>
#include<text.h>
#include<string>
#include"gameManager.h"
#include"guiAppState.h"
#include"stateManager.h"
#include"exitButton.h"
#include"optionsButton.h"
#include"playButton.h"
#include"loadButton.h"
#include"util.h"

using namespace vb01;
using namespace std;
using namespace fsim;

int main(){
	GameManager *gm=new GameManager();
	GuiAppState *guiState=new GuiAppState(gm);
	gm->getStateManager()->attachState(guiState);
	makeTitleScreenButtons(gm);	
	while(gm->isRunning())
		gm->update();
	return 0;
}
