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
	Vector2 okButtonPos=Vector2(280,400),okButtonSize=Vector2(100,20),defaultsButtonPos=Vector2(390,400),defaultsButtonSize=Vector2(100,20);

	OptionsButton::OptionsButton(GameManager *gm,Vector2 pos, Vector2 size) : Button(gm,pos,size,"Options",Mapping::OPTIONS){}

	void OptionsButton::onClick(){
		struct Options{
			vector<Listbox*> listboxes;
			vector<Textbox*> textboxes;
		};

		class TabButton : public Button{
			public:
				TabButton(GameManager *gm, Vector2 pos, Vector2 size, string name,Mapping::Bind bind) : Button(gm,pos,size,name,bind){}
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
					//defaultsButton=new DefaultsButton(gm,Vector2(390,400),Vector2(100,20));
					//okButton=new OkButton(gm,Vector2(280,400),Vector2(100,20));
					guiState->addButton(backButton);
					//guiState->addButton(okButton);
					//guiState->addButton(defaultsButton);
					Button *e[]{this,backButton};
					guiState->removeAllButtons(e,2);
				}
			protected:
		};

		class ControlsTab : public TabButton{
			public:
				ControlsTab(GameManager *gm) : TabButton(gm,Vector2(100,100),Vector2(100,20),"Controls",Mapping::CONTROLS){}
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
						if(data[0]==Mapping::JOYSTICK_AXIS){
							data[1]=data[2]%2>0?1:0;
							data[2]=(data[2]-(data[2]%2>0?1:0))/2;
						}
						lines[i]=bind+fromIntToString(data);
					}
					Listbox *listbox=new Listbox(gm,Vector2(400,200),Vector2(300,20),lines,numLines>10?10:numLines,Listbox::CONTROLS);
					guiState->addListbox(listbox);

					OkButton *okButton = new OkButton(gm);
					DefaultsButton *defaultsButton=new DefaultsButton(gm,listbox);
					guiState->addButton(okButton);
					guiState->addButton(defaultsButton);

					Options options;
					options.listboxes.push_back(listbox);
					okButton->setOptions(options);
					defaultsButton->setOptions(options);

					guiState->removeButton(this);
				}
			private:
				class OkButton : public Button{
					public:
						OkButton(GameManager *gm) : Button(gm,okButtonPos,okButtonSize,"OK"){}
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
						DefaultsButton(GameManager *gm,Listbox *listbox) : Button(gm,defaultsButtonPos,defaultsButtonSize,"Restore default"){
							this->listbox=listbox;
						}
						void onClick(){
							vector<string> lines;
							readFile(PATH+"../options.cfg",lines);	

							int lineId=0;
							for(int i=0;i<numStates;i++){
								for(int j=0;j<numBinds[i];j++){
									int colon=-1;
									for(int k=0;k<lines[lineId].length()&&colon==-1;k++)
										if(lines[lineId].c_str()[k]==':')
											colon=k;
									lines[lineId]=lines[lineId].substr(0,colon+1)+to_string(defaultBindTypes[i][j])+","+to_string(action[i][j])+","+to_string(defaultTriggers[i][j]);
									lineId++;
								}
							}
							writeFile(PATH+"../options.cfg",lines);

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
								if(data[0]==Mapping::JOYSTICK_AXIS){
									data[1]=data[2]%2>0?1:0;
									data[2]=(data[2]-(data[2]%2>0?1:0))/2;
								}
								lines[i]=bind+fromIntToString(data);
								listbox->changeLine(i,lines[i]);
							}
						}
						void setOptions(Options o){this->options=o;}
					private:
						Listbox *listbox;
						Options options;
				};
		};

		class VideoTab : public TabButton{
			public:
				VideoTab(GameManager *gm) : TabButton(gm,Vector2(210,100),Vector2(100,20),"Video",Mapping::VIDEO){}
				void onClick(){
					TabButton::onClick();	

					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					OkButton *okButton = new OkButton(gm);
					DefaultsButton *defaultsButton=new DefaultsButton(gm);
					guiState->addButton(okButton);
					guiState->addButton(defaultsButton);

					vector<string> l1,l2;
					l1.push_back("800,600");
					l1.push_back("1376,766");
					l1.push_back("1920,1080");
					l2.push_back("Low");
					l2.push_back("Medium");
					l2.push_back("High");
					Listbox *resolutionListbox=new Listbox(gm,Vector2(100,300),Vector2(100,20),l1,3);
					Listbox *textureListbox=new Listbox(gm,Vector2(210,300),Vector2(100,20),l2,3);
					guiState->addListbox(resolutionListbox);
					guiState->addListbox(textureListbox);

					Options options;
					options.listboxes.push_back(resolutionListbox);
					options.listboxes.push_back(textureListbox);
					okButton->setOptions(options);
					defaultsButton->setOptions(options);

					guiState->removeButton(this);
				}
			private:
				class OkButton : public Button{
					public:
						OkButton(GameManager *gm) : Button(gm,okButtonPos,okButtonSize,"OK"){}
						void onClick(){
							vector<string> lines;
							readFile(PATH+"../options.cfg",lines);	

							int sumBinds=0;
							for(int i=0;i<numStates;i++)
								sumBinds+=numBinds[i];

							for(int i=sumBinds;i<sumBinds+3;i++)
								for(int j=0;j<lines[i].length();j++)
									if(lines[i].c_str()[j]==':')
										lines[i]=lines[i].substr(0,j+1);

							int so1=options.listboxes[0]->getSelectedOption();
							int so2=options.listboxes[1]->getSelectedOption();
							int comma=0;
							string resolutionLine=options.listboxes[0]->getContents()[so1];
							for(int i=0;i<resolutionLine.length();i++)
								if(resolutionLine.c_str()[i]==',')
									comma=i;
							lines[sumBinds]+=resolutionLine.substr(0,comma);
							lines[sumBinds+1]+=resolutionLine.substr(comma+1,string::npos);
							lines[sumBinds+2]+=to_string(so2);

							writeFile(PATH+"../options.cfg",lines);
						}
						void setOptions(Options o){this->options=o;}
					private:
						Options options;
				};

				class DefaultsButton : public Button{
					public:
						DefaultsButton(GameManager *gm) : Button(gm,defaultsButtonPos,defaultsButtonSize,"Restore default"){}
						void onClick(){}
						void setOptions(Options o){this->options=o;}
					private:
						Options options;
				};
		};

		GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);

		ControlsTab *controls=new ControlsTab(gm);
		VideoTab *video=new VideoTab(gm);
		MainMenuButton *menuButton=new MainMenuButton(gm,Vector2(500,400),Vector2(100,40));

		guiState->addButton(controls);
		guiState->addButton(video);
		guiState->addButton(menuButton);

		Button *e[]{this,controls,video,menuButton};
		guiState->removeAllButtons(e,4);
		guiState->removeButton(this);
	}
}
