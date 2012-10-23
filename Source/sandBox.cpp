#include "sandbox.h"
#include <sstream>


struct teamRadioStruct{
	int team;
	XAutomataEngine *game;
	teamRadioStruct(int team = 0, XAutomataEngine *game = NULL){
		this->team = team, this->game = game;
	}
};

static void changeTeam(SDL_Event *e, void* p){
	radioButton* s = (radioButton*)p;
	if(!s->checked) return;
	teamRadioStruct *team = (teamRadioStruct*)s->tag;
	team->game->team = team->team;
}

static void changeRestrict(SDL_Event *e, void* p){
	checkBox *s = (checkBox*)p;
	XAutomataEngine* game =  (XAutomataEngine*)s->tag;
	game->ruleRestricted = s->checked;
}

static void sandUpHandler(SDL_Event *e, void* p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->panUp();
}
static void sandDownHandler(SDL_Event *e, void* p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->panDown();
}
static void sandLeftHandler(SDL_Event *e, void* p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->panLeft();
}
static void sandRightHandler(SDL_Event *e, void* p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->panRight();
}

static void sandHomeHandler(SDL_Event *e, void* p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->fit();
}

static void sandZoomInHandler(SDL_Event *e, void*p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->zoomIn();
}
static void sandZoomOutHandler(SDL_Event *e, void*p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->zoomOut();
}

static void sandScrollHandler(SDL_Event *e, void *p){
	
	HScroll *s = (HScroll*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->ticksPerFrame = (float)pow(3.0,(s->value-5));
}

static void sandPause(SDL_Event *e, void *p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->paused = !game->paused;
}

static void sandStep(SDL_Event *e, void *p){
	
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	game->paused = true;
	game->simulate(1);
}

static void sandLabelText(SDL_Event *e, void *p){
	
	label *s = (label*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	if(game->paused||game->ticksPerFrame<=0.00001) s->text = "Speed: paused";
	else{
		stringstream ss;
		ss<<game->ticksPerFrame;
		ss>>s->text;
		s->text = "Speed: "+s->text;
	}
}

static void sandPauseButtonText(SDL_Event *e, void *p){
	Button *s = (Button*)p;
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	if(game->paused) s->text = "Play";
	else s->text = "Pause";
}

static bool running;

static void sandBack(SDL_Event *e, void* p){
	running = false;
}

static void sandKey(SDL_Event *e, void *p){
	form* s = (form*)p;
	//pass to the game.
	XAutomataEngine *game = (XAutomataEngine*)s->tag;
	if(s->findChild(game)==s->focusedControl) return;
	game->callEvent(e,e->type);
}


void sandbox(SDL_Surface *s, int width, int height){
	form sand(0,0,1000,600);
	cout<<sand.children.size()<<"\n";
	XAutomataEngine game(&sand,200,100,750,400,width,height);
	//configure key events to pass preview to sand.
	sand.tag = &game;
	sand.addHandler(event_keydown,sandKey);

	//pan controls
	Button panUp(&sand,70,190,45,45); panUp.text = "Up";
	Button panDown(&sand,70,290,45,45); panDown.text = "Down";
	Button panLeft(&sand,20,240,45,45); panLeft.text = "Left";
	Button panRight(&sand,120,240,45,45); panRight.text = "Right";
	Button home(&sand,70,240,45,45); home.text = (width==-1&&height==-1?"Home":"Fit");

	panUp.tag = panDown.tag = panLeft.tag = panRight.tag = home.tag = (void*)(&game);
	panUp.addHandler(event_Lclick,sandUpHandler);
	panDown.addHandler(event_Lclick,sandDownHandler);
	panLeft.addHandler(event_Lclick,sandLeftHandler);
	panRight.addHandler(event_Lclick,sandRightHandler);
	home.addHandler(event_Lclick,sandHomeHandler);

	//zoom controls
	Button zoomIn(&sand,55,375,75,45); zoomIn.text = "Zoom in";
	Button zoomOut(&sand,55,445,75,45); zoomOut.text = "Zoom out";
	zoomIn.tag = zoomOut.tag =  (void*)(&game);
	zoomIn.addHandler(event_Lclick,sandZoomInHandler);
	zoomOut.addHandler(event_Lclick,sandZoomOutHandler);

	//speed controls
	label speedLabel(&sand,200,530,130,30); speedLabel.text = "Speed: Paused";
	HScroll speedScroll(&sand,350,530,200,30,1,10,5);
	Button pause(&sand,600,530,70,30); pause.text = "Pause";
	Button step(&sand,700,530,70,30); step.text = "Step";
	speedLabel.tag = speedScroll.tag = pause.tag = step.tag = (void*)(&game);
	speedScroll.addHandler(event_scroll,sandScrollHandler);
	pause.addHandler(event_Lclick,sandPause);
	step.addHandler(event_Lclick,sandStep);
	
	//colour controls
	radioButton white(&sand,300,35,80,20,NULL);	white.setTextColour(255,255,255);	white.setText("Master");
	radioButton red(&sand,400,35,80,20,&white);	red.setTextColour(255,100,100);		red.setText("Red");
	radioButton green(&sand,500,35,80,20,&red);	green.setTextColour(0,255,100);	green.setText("Green");
	radioButton blue(&sand,600,35,80,20,&green);	blue.setTextColour(100,100,255);	blue.setText("Blue");
	radioButton yellow(&sand,700,35,80,20,&green);	yellow.setTextColour(255,255,0);	yellow.setText("Yellow");

	checkBox bind(&sand,300,70,150,30); bind.setText("Enforce rules");
	bind.tag = (void*)&game;
	bind.addHandler(event_checkchanged,changeRestrict);

	teamRadioStruct params[5];
	radioButton *colours[5] = {&white,&red,&green,&blue,&yellow};
	for(int i = 0; i < 5; i++){
		params[i] = teamRadioStruct(i,&game);
		colours[i]->tag = &params[i];
		colours[i]->addHandler(event_checkchanged,changeTeam);
	}
	
	white.select();
	game.fit();

	//adding a save feature. No open yet...
	Button save(&sand,900,10,80,40); save.text = "Save";

	//exit controls
	Button back(&sand,50,50,100,50); back.text = "Back";
	back.addHandler(event_Lclick,sandBack);

	sand.addHandler(event_quit,sandBack);

	//keep that label and pause button updated
	pause.addHandler(event_frameTick,sandPauseButtonText);
	speedLabel.addHandler(event_frameTick,sandLabelText);

	sand.startFramerate(20,s);
	
	running = true;
	while(running){
		sand.wait();
		SDL_Flip(s);
	}
	cout<<"Ended";
}
