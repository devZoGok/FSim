#include"textbox.h"
#include"gameManager.h"
#include<root.h>
#include<node.h>
#include<quad.h>
#include"defConfigs.h"
#include<material.h>
#include<sstream>
#include<ostream>
#include<algorithm>

using namespace vb01;
using namespace std;

namespace fsim{
	Textbox::TextboxButton::TextboxButton(Textbox *t, GameManager *gm,Vector2 pos, Vector2 size, string name) : Button(gm,pos,size,name,Mapping::NONE,false) {textbox = t;}

	void Textbox::TextboxButton::onClick(){
		if(!textbox->isEnabled())
			textbox->enable();
		else
			textbox->disable();
	}

	Textbox::Textbox(GameManager *gm,Vector2 pos,Vector2 size,string entry){
		this->pos = pos;
		this->size = size;

		textboxButton = new TextboxButton(this,gm,pos,size,"TextboxButton");

		guiNode=gm->getRoot()->getGuiNode();
		text=new Text(PATH+"Fonts/batang.ttf",entry);
		text->setScale(.5);
		textNode=new Node(Vector3(pos.x,pos.y+size.y,-.1));
		textNode->addText(text);
		guiNode->attachChild(textNode);

		cursorPosOffset=entry.length();
		cursorRect=new Quad(Vector3(cursorWidth,size.y,0),false);
		cursorNode=new Node(Vector3(pos.x+text->getLength(),pos.y,-.2));
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->setTexturingEnabled(false);
		mat->setDiffuseColor(Vector4(1,1,1,1));
		cursorRect->setMaterial(mat);
		cursorNode->attachMesh(cursorRect);
		cursorNode->setVisible(false);
		guiNode->attachChild(cursorNode);
	}
	
	Textbox::~Textbox(){
		guiNode->dettachChild(textNode);
		guiNode->dettachChild(cursorNode);
		delete textNode;	
		delete cursorNode;
	}

	void Textbox::update(){
		if(enabled){
			if(canChangeCursor()){
				canShowCursor=!canShowCursor;
				lastBlinkTime = getTime();
				cursorNode->setVisible(canShowCursor);
			}
		}
	}

	void Textbox::type(char c, bool capitalLetters){
		if(capitalLetters&&'a'<=c&&c<='z')
			c-=32;
		else if(!capitalLetters&&'A'<=c&&c<='Z')
			c+=32;
		entry+=c;
		text->setText(entry);
		moveCursor(false,text->getCharWidth(c));
	}

	void Textbox::moveCursor(bool left,float charWidth){
		Vector3 p=cursorNode->getPosition();
		if(left && entry.size() - cursorPosOffset> 0){
			cursorPosOffset--;
			cursorNode->setPosition(Vector3(p.x-charWidth,p.y,p.z));
		}
		else if(!left){
			cursorPosOffset++;
			cursorNode->setPosition(Vector3(p.x+charWidth,p.y,p.z));
		}
	}

	void Textbox::deleteCharacter(){
		if(entry.length()>0){
			char c=entry.c_str()[entry.length()-1];
			entry=entry.substr(0,entry.length()-1);
			text->setText(entry);
			moveCursor(true,text->getCharWidth(c));
		}
	}

	void Textbox::enable(){
		enabled = true;
		lastBlinkTime = getTime();
		cursorNode->setVisible(true);
	}

	void Textbox::disable(){
		enabled = false;
		cursorNode->setVisible(false);
	}

	void Textbox::setEntry(string entry){
		this->entry = entry;
		text->setText(entry);
		cursorNode->setPosition(Vector3(pos.x+text->getLength(),pos.y,1));
		cursorPosOffset=entry.length();
	}
}
