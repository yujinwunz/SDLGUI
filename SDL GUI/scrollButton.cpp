#include "scrollButton.h"
scrollButton::scrollButton(form* parent, int x, int y, int w, int h):Button(parent,x,y,w,h){

}
void scrollButton::mouseLeave(SDL_Event* e){
	if(hovered&&!leftDown){
		hovered = false;
		callEvent(e,event_leave);
	}
}