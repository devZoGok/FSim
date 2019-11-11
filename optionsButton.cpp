#include"optionsButton.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"listbox.h"
#include"util.h"
#include"defConfigs.h"
#include"mainMenuButton.h"
#include<vector>

using namespace vb01;
using namespace std;

namespace fsim{
	OptionsButton::OptionsButton(GameManager *gm,Vector2 pos, Vector2 size) : Button(gm,pos,size,"Options",true){}

	void OptionsButton::onClick(){
		struct Options{
			vector<Listbox*> listboxes;
			vector<Textbox*> textboxes;
			vector<int> ids;
		};
		class OkButton : public Button{
			public:
				OkButton(GameManager *gm, Vector2 pos, Vector2 size) : Button(gm,pos,size,"OK"){}
				void onClick(){
					int sumBinds=0;
					for(int i=0;i<numStates;i++)
						sumBinds+=numBinds[i];
					vector<string> lines;
					readFile(PATH+"../options.cfg",lines);	
					for(int i=0;i<sumBinds;i++)
						for(int j=0;j<lines[i].length();j++)
							if(lines[i].c_str()[j]==':')
								lines[i]=lines[i].substr(0,j+1);
					int numListboxes=options.listboxes.size(),numTextboxes=options.textboxes.size();
					for(int i=0;i<numListboxes;i++){
						for(int j=0;j<options.listboxes[i]->getNumLines();j++)
							if(options.listboxes[i]->getType()==Listbox::CONTROLS){
								int colon=0;
								for(int k=0;k<lines[j].length();k++)
									if(lines[j].c_str()[k]==':')
										colon=k+1;
								int *data=fromStringToInt(options.listboxes[i]->getContents()[j].substr(colon,string::npos));
								lines[j]+=to_string(data[0])+","+to_string(data[1])+","+to_string(data[2]);
								delete data;
							}
					}
					writeFile(PATH+"../options.cfg",lines);
				}
				void setOptions(Options o){this->options=o;}
			private:
				Options options;
		};
		class DefaultsButton : public Button{
			public:
				DefaultsButton(GameManager *gm, Vector2 pos, Vector2 size) : Button(gm,pos,size,"Restore default"){}
				void onClick(){
				
				}
				void setOptions(Options o){this->options=o;}
			private:
				Options options;
		};

		class TabButton : public Button{
			public:
				TabButton(GameManager *gm, Vector2 pos, Vector2 size, string name) : Button(gm,pos,size,name){}
				void onClick(){
					class BackButton : public Button{
						public:
							BackButton(GameManager *gm,Vector2 pos,Vector2 size) : Button(gm,pos,size,"Back"){}
							void onClick(){
								GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
								guiState->removeAllGUIElements(false);
								Button *e[]{this};
								guiState->removeAllButtons(e,1);
								OptionsButton *optionsButton=new OptionsButton(gm,Vector2::VEC_ZERO,Vector2::VEC_ZERO);
								optionsButton->onClick();
								delete optionsButton;
								guiState->removeButton(this);
							}
						private:
					};
					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					guiState->removeAllGUIElements(false);
					BackButton *backButton=new BackButton(gm,Vector2(500,400),Vector2(100,20));
					defaultsButton=new DefaultsButton(gm,Vector2(390,400),Vector2(100,20));
					okButton=new OkButton(gm,Vector2(280,400),Vector2(100,20));
					guiState->addButton(backButton);
					guiState->addButton(okButton);
					guiState->addButton(defaultsButton);
					Button *e[]{this,okButton,defaultsButton,backButton};
					guiState->removeAllButtons(e,4);
				}
			protected:
				OkButton *okButton;
				DefaultsButton *defaultsButton;
		};

		class ControlsTab : public TabButton{
			public:
				ControlsTab(GameManager *gm) : TabButton(gm,Vector2(100,100),Vector2(100,20),"Controls"){}
				void onClick(){
					TabButton::onClick();

					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					vector<string> lines;
					int numLines=0;
					for(int i=0;i<numStates;i++)
						numLines+=numBinds[i];
					readFile(PATH+"../options.cfg",lines,0,numLines);
					for(int i=0;i<numLines;i++){
						int colon;
						for(int j=0;j<lines[i].length();j++)
							if(lines[i].c_str()[j]==':')
								colon=j;
						string bind=lines[i].substr(0,colon+1);
						int data[]{
							atoi(lines[i].substr(colon+1,1).c_str()),
							atoi(lines[i].substr(colon+3,1).c_str()),
							atoi(lines[i].substr(colon+5,string::npos).c_str())
						};
						/*
						*/
						if(data[0]==Mapping::JOYSTICK_AXIS){
							data[1]=data[2]%2>0?1:0;
							data[2]=(data[2]-(data[2]%2>0?1:0))/2;
						}
						lines[i]=bind+fromIntToString(data);
					}
					Listbox *listbox=new Listbox(gm,Vector2(400,200),Vector2(300,20),lines,numLines>10?10:numLines,Listbox::CONTROLS);
					guiState->addListbox(listbox);

					vector<Listbox*> listboxes;
					vector<int> ids;
					listboxes.push_back(listbox);
					ids.push_back(0);
					Options options;
					options.listboxes=listboxes;
					options.ids=ids;
					okButton->setOptions(options);
					defaultsButton->setOptions(options);

					guiState->removeButton(this);
				}
			private:
		};

		GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
		Button *e[]{this};
		guiState->removeAllButtons(e,1);
		guiState->addButton(new ControlsTab(gm));
		//guiState->addButton(new MainMenuButton(gm,Vector2(500,400),Vector2(100,40)));
		guiState->removeButton(this);
	}
}
