#include "form.h"
control *pressedControl;
int form::findChild(control *c){
	for(int i = 0; i < children.size(); i++) if(children[i]==c) return i;
	return -1;
}

form::form(int x, int y, int w, int h,form* parent):control(parent){
	this->setPos(x,y); this->setSize(w,h);
	leftBindedControl = -2; rightBindedControl = -2; middleBindedControl = -2;
	focusedControl = hoveredControl = -2;
	backColour.r = backColour.g = backColour.b = 50;
	renderStatred = false;
	foreColour = backColour;
}
void form::addChild(control *c){
	if(findChild(c)!=-1) return;
	children.push_back(c);
	c->setParent(this);
}
void form::removeChild(control *c){
	//doesn't dispose.
	int location = findChild(c);
	if(location==-1) return;
	for(int i = location; i <= children.size()-1; i++) children[i] = children[i+1];
	children.pop_back();
}

void form::handleMouseMove(SDL_Event &e){
		if(within(left,top,width,height,e.motion.x,e.motion.y)){
			this->mouseEnter(&e);
		}else{
			this->mouseLeave(&e);
			//return;
		}
		if(_DEGUGG) cout<<name<<"\t"<<"Before: "<<e.motion.x<<","<<e.motion.y<<" After: ";
		//MOTION. trickkky.--------------------------------------------
		int state = SDL_GetMouseState(NULL,NULL);
		//this part handles the case of pointers dragging into the window. In which, ignore.
		if((state&SDL_BUTTON_LMASK)&&leftBindedControl==-2) return;
		if((state&SDL_BUTTON_MMASK)&&middleBindedControl==-2) return;
		if((state&SDL_BUTTON_RMASK)&&rightBindedControl==-2) return;

		//this part handles the case of pointers leaving, releasing and then entering.
		SDL_Event sim;
		sim.type = SDL_MOUSEBUTTONUP;
		sim.button.state = SDL_RELEASED;
		sim.button.type = SDL_MOUSEBUTTONUP;
		sim.button.x = e.motion.x;
		sim.button.y = e.motion.y;
		sim.button.which = 1;

		if(!(state&SDL_BUTTON_LMASK)&&leftBindedControl!=-2){
			sim.button.button = SDL_BUTTON_LEFT;
			SDL_PushEvent(&sim); SDL_PushEvent(&e);
			return;
		}
		if(!(state&SDL_BUTTON_MMASK)&&middleBindedControl!=-2){
			sim.button.button = SDL_BUTTON_MIDDLE;
			SDL_PushEvent(&sim); SDL_PushEvent(&e);
			return;
		}
		if(!(state&SDL_BUTTON_RMASK)&&rightBindedControl!=-2){
			sim.button.button = SDL_BUTTON_RIGHT;
			SDL_PushEvent(&sim); SDL_PushEvent(&e);
			return;
		}
		//by this point, if there is a button down, there is one corresponding binded control. If there is a button down, send to the corresponding control only
		e.motion.x -= left;
		e.motion.y -= top;
		if(_DEGUGG) cout<<e.motion.x<<","<<e.motion.y<<"\n";
		if(state){
			int control = max(leftBindedControl,max(rightBindedControl,middleBindedControl));
			if(control==-1){
				this->callEvent(&e,e.type);
			}else{
				if(within(children[control]->left,children[control]->top,
					children[control]->width,children[control]->height,
					e.motion.x,e.motion.y)){
						children[control]->mouseEnter(&e);
						children[control]->handleEvent(e);
						hoveredControl = control;
				}else{
					children[control]->mouseLeave(&e);
					children[control]->handleEvent(e);
					hoveredControl = -2;
				}
			}
				
		}else{
			//find which control this belongs to.
			int control = -2;
			if(within(left,top,width,height,e.motion.x,e.motion.y)) control = -1;
			for(int i = 0; i < children.size(); i++){
				if(within(children[i]->left,children[i]->top,children[i]->width,
					children[i]->height,e.motion.x,e.motion.y)){
					control = i;
				}
			}
			if(control!=hoveredControl){
				if(hoveredControl==-1) mouseLeave(&e);
				if(hoveredControl>=0) children[hoveredControl] -> mouseLeave(&e);	
			}
			if(control==-1){
				mouseEnter(&e);
				callEvent(&e,e.type);
			}
			if(control>=0){
				children[control]->mouseEnter(&e);
				children[control]->handleEvent(e);
			}
			hoveredControl = control;
		}

}
void form::handleEvent(SDL_Event &e){
	if(e.type == event_mousemotion){
		if(!renderStatred) handleMouseMove(e);
	}else if(e.type == event_frameTick){
		//pass the motion!
		SDL_Event me;
		int mx, my;
		int state = SDL_GetMouseState(&mx,&my);
		me.type = event_mousemotion; me.motion.x = mx; me.motion.y = my;
		me.motion.xrel = mx-prevMouseX; me.motion.yrel = my - prevMouseY;
		if(mx!=prevMouseX||my!=prevMouseY){
			//only handle mouse moves if the mouse had been moved.
			handleMouseMove(me);
			prevMouseX = mx; prevMouseY = my;
		}
		//render.
		SDL_FillRect((SDL_Surface*)e.user.data1,NULL,_BACKCOLOUR);
		render((SDL_Surface*)e.user.data1);

		//tick
		frameTick(&e);
			
	}else if(e.type == SDL_MOUSEBUTTONDOWN){
		if(_DEGUGG) cout<<"Mousedown\n";
		//if(!within(left,top,width,height,e.button.x,e.button.y))return;
		//check if anything is already clicked and hence bound.
		int bound = max(leftBindedControl,max(rightBindedControl,middleBindedControl));
		int control = -1;
			
		//determine which control this message is sent to.
		e.button.x-=left; e.button.y-=top;
		if(bound!=-2){
			//yep.
			control = bound;
		}else{
			//find which control this is landing on.
			for(int i = 0; i < children.size(); i++){
				if(within(children[i]->left,children[i]->top,children[i]->width,children[i]->height,e.button.x,e.button.y)) control = i;
			}
		}
		//send mousedown to the control.
		if(_DEGUGG) cout<<"Control: "<<control<<"\n";
		if(control==-1){
			this->mouseDown(&e);
			this->callEvent(&e,e.type);
		}else if(control>=0){
			children[control]->mouseDown(&e);
			children[control]->handleEvent(e);
		}
		//manage focus
		if(control!=focusedControl){
			if(focusedControl>=0) children[focusedControl]->loseFocus();
			if(focusedControl==-1) this->loseFocus();
			focusedControl = control;
			if(control>=0) children[control]->getFocus();
			if(focusedControl==-1) this->getFocus();
		}

		if(e.button.button == SDL_BUTTON_LEFT) leftBindedControl = control;
		if(e.button.button == SDL_BUTTON_RIGHT) rightBindedControl = control;
		if(e.button.button == SDL_BUTTON_MIDDLE) middleBindedControl = control;
			
	}else if(e.type == SDL_MOUSEBUTTONUP){
		//if(!within(left,top,width,height,e.button.x,e.button.y))return;
		//find which control this event belongs to.
		int control, type;
		if(e.button.button == SDL_BUTTON_LEFT){
			control = leftBindedControl; type = event_Lclick;
			leftBindedControl = -2;
		}
		if(e.button.button == SDL_BUTTON_RIGHT){
			control = rightBindedControl; type = event_Rclick;
			rightBindedControl = -2;
		}
		if(e.button.button == SDL_BUTTON_MIDDLE){
			control = middleBindedControl; type = event_Mclick;
			middleBindedControl = -2;
		}
		//handle click
		if(_DEGUGG) cout<<control<<"\n";
		e.button.x-=left; e.button.y-=top;
		if(control >= 0){
			if(within(children[control]->left,children[control]->top,
				children[control]->width,children[control]->height,e.button.x,e.button.y)){
				SDL_Event te;
				te.type = type;
				children[control]->handleEvent(te);
			}
		}else{
			SDL_Event te;
			te.type = type;
			this->callEvent(&te,te.type);
		}
		//send mouseup message
		if(control>=0)children[control]->mouseUp(&e);
		else this->mouseUp(&e);
		if(control>=0)children[control]->handleEvent(e);
		else this->callEvent(&e,e.type);
			
	}else if(e.type == event_keydown||e.type==event_keyup){
		//keys only send to focus.
		if(focusedControl>0) children[focusedControl]->handleEvent(e);
		if(focusedControl==-1) this->callEvent(&e,e.type);
	}
}
void form::mouseLeave(SDL_Event *e){
	if(hovered){
		hovered = false;
		callEvent(e,event_leave);
	}
	if(hoveredControl>=0){
		children[hoveredControl]->mouseLeave(e);
	}
	hoveredControl = -2;
}
void form::wait(){
	SDL_Event e;
	SDL_WaitEvent(&e);
	handleEvent(e);
}	//the jist of event driven! Is responsible for event behavior of all controls, and especially the click event.
void form::loseFocus(){
	focused = false;
	if(focusedControl>=0) children[focusedControl]->loseFocus();
	focusedControl = -2;
}

//override the render.
void form::render(SDL_Surface *s){
	SDL_Surface *ts = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height, 32,255<<16,255<<8,255,0);

	SDL_Event e;
	e.type = event_render;
	e.user.data1 = ts;
		
	SDL_Rect dRect = {left,top,width,height};
	SDL_BlitSurface(s,&dRect,ts,NULL);

	if(handler[event_render]!=NULL)callEvent(&e,event_render);
	else draw(ts);

	for(int i = 0; i < children.size(); i++){
		children[i]->render(ts);
	}

	SDL_BlitSurface(ts,NULL,s,&dRect);
	SDL_FreeSurface(ts);
}

void form::startFramerate(float fps, SDL_Surface *s){
	//begins a timer that pushes an event_frameTick event every frame with a pointer to the surface to be rendered on.
	frametimer = SDL_AddTimer(1000.0/fps,timerTick, s);
	renderStatred = true;
	framerate = fps;
}

void form::stopFramerate(){
	if(!renderStatred) return;
	SDL_RemoveTimer(frametimer);
	renderStatred = false;
}
	
