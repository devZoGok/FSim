#define MYSQLPP_MYSQL_HEADERS_BURIED

#include"playButton.h"
#include"okButton.h"
#include"mainMenuButton.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"textbox.h"
#include<node.h>
#include<mesh.h>
#include<material.h>

using namespace vb01;
using namespace std;

namespace fsim{
	string flags[]{"chineseInsignia","japaneseInsignia","koreanInsignia"};

	PlayButton::PlayButton(GameManager *gm,Vector2 pos, Vector2 size):Button(gm,pos,size,"Play",Mapping::PLAY){}

	void PlayButton::onClick(){
		class FactionButton : public Button{
			public:
				FactionButton(GameManager *gm, Vector2 pos, Vector2 size,OkButton *okButton, int faction):Button(gm,pos,size,to_string(faction),Mapping::Bind(Mapping::SELECT_CHINA+faction),true,PATH+"Textures/"+flags[faction]+".jpg"){
					this->okButton=okButton;
					this->faction=faction;
				}
				void onClick(){
					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					for(int i=0;i<3;i++){
						Button *b=guiState->getButton(to_string(i));
						if(i==faction)
							b->setImage(PATH+"Textures/Insignia/"+flags[i]+".jpg");
						else
							b->setImage(PATH+"Textures/Insignia/"+flags[i]+"Gray.jpg");
					}
					okButton->setFaction(faction);
				}
			private:
				int faction;
				OkButton *okButton;
		};

		GuiAppState *guiState=((GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE));
		Button *e[]{this};
		guiState->removeAllGUIElements(false);
		guiState->removeAllButtons(e,1);

		Textbox *t=new Textbox(gm,Vector2(300,550),Vector2(150,40));
		guiState->addTextbox(t);
		OkButton *okButton=new OkButton(gm,Vector2(480,550),Vector2(100,40),t);
		int width=200;
		for(int i=0;i<3;i++)
			guiState->addButton(new FactionButton(gm,Vector2(50+i*(width+10),10),Vector2(width,400),okButton,i));
		guiState->getButton("0")->onClick();
		guiState->addButton(okButton);
		guiState->addButton(new MainMenuButton(gm,Vector2(100,550),Vector2(100,40)));

		guiState->removeButton(this);
	}

	void PlayButton::onMouseOver(){
		Button::onMouseOver();	
		if(mouseOver){
			if(textNode)
				textNode->getText(0)->setColor(Vector4(.1,.1,.1,1));
			rectNode->getMesh(0)->getMaterial()->setDiffuseColor(Vector4(.8,.8,.8,1));
		}
	}

	void PlayButton::onMouseOff(){
		Button::onMouseOff();	
		if(!mouseOver){
			if(textNode)
				textNode->getText(0)->setColor(Vector4(1,1,1,1));
			rectNode->getMesh(0)->getMaterial()->setDiffuseColor(Vector4(.6,.6,.6,1));
		}
	}
}
