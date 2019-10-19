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
#include<iostream>

using namespace vb01;
using namespace std;
using namespace fsim;

int main(){
	GameManager *gm=new GameManager();
	GuiAppState *guiState=new GuiAppState(gm);
	PlayButton *playButton=new PlayButton(gm,Vector2(100,100),Vector2(100,50));
	LoadButton *loadButton=new LoadButton(gm,Vector2(100,160),Vector2(100,50));
	OptionsButton *optionsButton=new OptionsButton(gm,Vector2(100,220),Vector2(100,50));
	ExitButton *exitButton=new ExitButton(gm,Vector2(100,280),Vector2(100,50));
	guiState->addButton(playButton);
	guiState->addButton(loadButton);
	guiState->addButton(exitButton);
	guiState->addButton(optionsButton);
	gm->getStateManager()->attachState(guiState);

	while(gm->isRunning())
		gm->update();
	return 0;
}
