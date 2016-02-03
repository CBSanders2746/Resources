// Example program:
// Using SDL2 to create an application window
//Note to self: Convert the 2Player mode to true multiplayer over the break as a test-thingy.
//-Create a class/method to handle the creations of the images
#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_Image/SDL_image.h"
#endif
//Current Video: Interactions-7
#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

#if defined(__linux__)
#include <unistd.h>
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include <direct.h>
#define getcwd _getcwd
#endif

#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

//code for framerate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

//create the SDL_Rectangle for the texture's position and size -x,y,w,h
SDL_Rect bk1Pos;
SDL_Rect bk2Pos;
//bg movement speed
int backSpeed = 100;

//set up temp vars to hold movement
float back1PosX = 0, back1PosY = 0;
float back2PosX = 0, back2PosY = -768;
//Backgrounds END

void UpdateBackground()
{
	//Update background 1
	back1PosY+=(backSpeed*1)*deltaTime;
	back2PosY+=(backSpeed*1)*deltaTime;

	//set the new bg1 pos
	bk1Pos.y = (int)(back1PosY + 0.5f);
	bk2Pos.y = (int)(back2PosY + 0.5f);

	//reset when off the bottom of the screen
	if(bk1Pos.y>=768)
	{
		bk1Pos.y=-768;
		back1PosY = bk1Pos.y;
	}
	if(bk2Pos.y>=768)
	{
		bk2Pos.y=-768;
		back2PosY = bk2Pos.y;
	}
}

const int JOYSTICK_DEAD_ZONE = 8000;
float xDir, yDir;//0 / 1
float posX, posY;
SDL_Rect curPos, actPos;
int cSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event)
{
	//js 0 = 1st player
	if(event.which == 0)
	{
		//check x
		if(event.axis == 0)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}
			else
			{
				xDir = 0.0f;
			}
		}
		//check y
		if(event.axis == 1)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f;
			}
			else
			{
				yDir = 0.0f;
			}
		}
	}
}

void updateCursor(float deltaTime)
{
	//update values
	posX.x += (cSpeed * xDir)*deltaTime;
	posY.y += (cSpeed * yDir)*deltaTime;

	//assign to rects
	curPos.x = (int)(posX+0.5f);
	curPos.y = (int)(posY+0.5f);

	//update active position of cursor - collision box
	actPos.x = curPos.x;
	actPos.y = curPos.y;

	//off the screen x-axis
	if(curPos.x<0)
	{
		curPos.x = 0;
		posX = curPos.x;
	}
	if(curPos.x>1024 - curPos.w)
	{
		curPos.x = 1024 - curPos.w;
		posX = curPos.x;
	}

	//off the screen y-axis
	if(curPos.y<0)
	{
		curPos.y = 0;
		posY = curPos.y;
	}
	if(curPos.y>768 - curPos.h)
	{
		curPos.y = 768 - curPos.h;
		posY = curPos.y;
	}
}

//variables determining overstates for buttons
bool player1Over = false, player2Over = false, instructionsOver = false,
		quitOver = false, menuOver = false, playOver = false;

#include "player.h"

int main(int argc, char* argv[]) {

#if defined(__APPLE__)
	cout << "Running on Apple." << endl;
	//get current working directory
	string s_cwd(getcwd(NULL, 0));
	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";
	//test
	//cout<<s_cwd_images<<endl;
#endif

#if defined(__linux__)
	cout << "Running on Linux." << endl;
	string s_cwd(getcwd(NULL, 0));
	string s_cwd_images = s_cwd + "/Resources/Images/";
	//This is the commit at the beginning of v10; Adding Graphics to the Menu
#endif

#if defined(_WIN32_) || (_WIN64_)
	cout << "Running on Windows." << endl;
	string s_cwd(getcwd(NULL, 0));
	string s_cwd_images = s_cwd + "\\Resources\\Images\\";
#endif

	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2 //VIDEO

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
			"An SDL2 window",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			1024,                               // width, in pixels
			768,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	//accelerated openGL renderer declared
	SDL_Renderer *renderer = NULL;

	//create the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Create Players
	Player PlayerOne = Player(renderer, 0, s_cwd_images.c_str(), 250.0, 500.0);
	Player PlayerTwo = Player(renderer, 1, s_cwd_images.c_str(), 750.0, 500.0);


	//*********Create Background Image**************
	string BGpath = s_cwd_images + "/1024x768.png";//Done
	string IMGcur = s_cwd_images + "/DotCursor.png";//Done
	string Tpath = s_cwd_images + "/Title.png";//Done
	string P1path = s_cwd_images + "/P1.png";//Done
	string P2path = s_cwd_images + "/P2.png";//Done
	string INSpath = s_cwd_images + "/Instructions.png";//Done
	string QUITpath = s_cwd_images + "/Quit.png";//Done
	string OverP1path = s_cwd_images + "/P1Over.png";//Done
	string OverP2path = s_cwd_images + "/P2Over.png";//Done
	string OverINSpath = s_cwd_images + "/InstructionsOver.png";//Done
	string OverQUITpath = s_cwd_images + "/QuitOver.png";//Done
	string ITpath = s_cwd_images + "/InstructionsTitle.png";//Done
	string MENUpath = s_cwd_images + "/Menu.png";//Done
	string OverMENUpath = s_cwd_images + "/MenuOver.png";//Done
	string LOSEpath = s_cwd_images + "/LoseTitle.png";//Done
	string WINpath = s_cwd_images + "/WinTitle.png";//Done
	string PLAYpath = s_cwd_images + "/Play.png";//Done
	string OverPLAYpath = s_cwd_images + "/PlayOver.png";//Done


	//Create the Surface
	SDL_Surface *surface;
	//Player1 and Player2's path is declared in player.cpp, win, lose


	//******************************** Create Win Title Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(WINpath.c_str());
	SDL_Texture *Win;//create
	//place surface info into the texture
	Win = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free
	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect wPos;
	//set the x, y, w, and h for the rectangle
	wPos.x = 240;
	wPos.y = 288;
	wPos.w = 596;//width of image
	wPos.h = 114;//height of image
	//Main Menu END

	//******************************** Create Lose Title Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(LOSEpath.c_str());
	SDL_Texture *Lose;//create
	//place surface info into the texture
	Lose = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free
	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect lPos;
	//set the x, y, w, and h for the rectangle
	lPos.x = 240;
	lPos.y = 288;
	lPos.w = 596;//width of image
	lPos.h = 114;//height of image
	//Main Menu END

	//******************************** Create Play Normal/Over Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(PLAYpath.c_str());
	SDL_Texture *Play;//create
	//place surface info into the texture
	Play = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	surface = IMG_Load(OverPLAYpath.c_str());
	SDL_Texture *PlayOver;
	PlayOver = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect pPos;
	//set the x, y, w, and h for the rectangle
	pPos.x = 875;
	pPos.y = 702;
	pPos.w = 125;//width of image
	pPos.h = 40;//height of image
	//Main Menu END

	//******************************** Create Instructions Title Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(ITpath.c_str());
	SDL_Texture *Ititle;//create
	//place surface info into the texture
	Ititle = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free
	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect itPos;
	//set the x, y, w, and h for the rectangle
	itPos.x = 120;
	itPos.y = 271;
	itPos.w = 781;//width of image
	itPos.h = 258;//height of image
	//Main Menu END

	//******************************** Create Menu Normal/Over Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(MENUpath.c_str());
	SDL_Texture *Menu;//create
	//place surface info into the texture
	Menu = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	surface = IMG_Load(OverMENUpath.c_str());
	SDL_Texture *MenuOver;
	MenuOver = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect mPos;
	//set the x, y, w, and h for the rectangle
	mPos.x = 22;
	mPos.y = 702;
	mPos.w = 145;//width of image
	mPos.h = 41;//height of image
	//Main Menu END

	//******************************** Create Backgrounds ********************************
	//create SDL surface to hold the background image
	surface = IMG_Load(BGpath.c_str());

	//create a SDL texture
	SDL_Texture *background1;
	SDL_Texture *background2;

	//place surface info into the texture
	background1 = SDL_CreateTextureFromSurface(renderer, surface);
	background2 = SDL_CreateTextureFromSurface(renderer, surface);

	//free the SDL surfaces
	SDL_FreeSurface(surface);

	//set the x, y, w, and h for the rectangle
	bk1Pos.x = 0;
	bk1Pos.y = 0;
	bk1Pos.w = 1024;
	bk1Pos.h = 768;

	bk2Pos.x = 0;
	bk2Pos.y = -768;
	bk2Pos.w = 1024;
	bk2Pos.h = 768;




	//******************************** Create Cursor ********************************
	//create SDL surface to hold the background image
	surface = IMG_Load(IMGcur.c_str());
	SDL_Texture *cursor;//create
	//place surface info into the texture
	cursor = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free
	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	//SDL_Rect curPos;
	//SDL_Rect actPos;
	//set the x, y, w, and h for the rectangle
	curPos.x = 0;
	curPos.y = 0;
	curPos.w = 16;
	curPos.h = 16;

	actPos.x = 0;//10
	actPos.y = 0;//10
	actPos.w = 16;//10
	actPos.h = 16;//10

	//variable for cursor speed
	//int cSpeed = 400;
	//Cursor END

	//******************************** Create Menu Title Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(Tpath.c_str());
	SDL_Texture *title;//create
	//place surface info into the texture
	title = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free
	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect tPos;
	//set the x, y, w, and h for the rectangle
	tPos.x = 264;
	tPos.y = 73;
	tPos.w = 532;//width of image
	tPos.h = 71;//height of image
	//Main Menu END

	//******************************** Create 1P Normal/Over Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(P1path.c_str());
	SDL_Texture *Player1;//create
	//place surface info into the texture
	Player1 = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	surface = IMG_Load(OverP1path.c_str());
	SDL_Texture *Player1Over;
	Player1Over = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect p1Pos;
	//set the x, y, w, and h for the rectangle
	p1Pos.x = 380;
	p1Pos.y = 261;
	p1Pos.w = 311;//width of image
	p1Pos.h = 42;//height of image
	//Main Menu END

	//******************************** Create 2P Normal Normal/Over Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(P2path.c_str());
	SDL_Texture *Player2;//create
	//place surface info into the texture
	Player2 = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	surface = IMG_Load(OverP2path.c_str());
	SDL_Texture *Player2Over;
	Player2Over = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect p2Pos;
	//set the x, y, w, and h for the rectangle
	p2Pos.x = 360;
	p2Pos.y = 343;
	p2Pos.w = 352;//width of image
	p2Pos.h = 42;//height of image
	//Main Menu END

	//****************************** Create Instructions Normal/Over Text/Image ******************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(INSpath.c_str());
	SDL_Texture *Instructions;//create
	//place surface info into the texture
	Instructions = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	surface = IMG_Load(OverINSpath.c_str());
	SDL_Texture *InstructionsOver;//create
	InstructionsOver = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect iPos;
	//set the x, y, w, and h for the rectangle
	iPos.x = 351;
	iPos.y = 424;
	iPos.w = 374;//width of image
	iPos.h = 42;//height of image
	//Main Menu END

	//******************************** Create Quit Normal/Over Text/Image ********************************
	//create SDL surface to hold the texture file
	surface = IMG_Load(QUITpath.c_str());
	SDL_Texture *Quit;//create
	//place surface info into the texture
	Quit = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	surface = IMG_Load(OverQUITpath.c_str());
	SDL_Texture *QuitOver;//create
	QuitOver = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);//free

	//create the SDL_Rectangle for the texture's position and size -x,y,w,h
	SDL_Rect qPos;
	//set the x, y, w, and h for the rectangle
	qPos.x = 391;
	qPos.y = 503;
	qPos.w = 288;//width of image
	qPos.h = 42;//height of image
	//Main Menu END

	//********** Set up a Game Controller variable **********
	SDL_GameController* gController = NULL;
	// Open gController
	gController = SDL_GameControllerOpen(0);
	/// turn on gController Events
	SDL_GameControllerEventState(SDL_ENABLE);
	//sdl event polling keyboard, etc... INPUT
	SDL_Event event;
	//SET UP VARS for the gamestates
	enum GState{MENU,INSTRUCTIONS,PLAYER1,PLAYER2,WIN,LOSE};
	//set up the init state
	GState gS = MENU;
	//bool values to control movement through states
	bool menu, instructions, player1, player2, win, lose, quit;


	// The window is open: could enter program loop here (see SDL_PollEvent())
	while(!quit)//uninitialized bools default to false
	{
		switch(gS)
		{
		case MENU:
			menu = true;
			cout<<"The Gamestate is Menu."<<endl;
			cout<<"Hover over a selection and Press A to select.."<<endl;
			while(menu)
			{
				//setup framerate for this section or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime-lastTime)/1000;
				lastTime = thisTime;

				//check input
				if(SDL_PollEvent(&event))
				{
					//check if sdlwindow is closed
					if(event.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}
					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)//0=p1, 1=p2
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(player1Over)
								{
									menu = false;
									gS = PLAYER1;
									player1Over = false;
								}
								if(player2Over)
								{
									menu = false;
									gS = PLAYER2;
									player2Over = false;
								}
								if(instructionsOver)
								{
									menu = false;
									gS = INSTRUCTIONS;
									instructionsOver = false;
								}
								if(quitOver)
								{
									menu = false;
									quit = true;
									quitOver = false;
								}
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}

				}
				UpdateBackground();
				updateCursor(deltaTime);

				player1Over = SDL_HasIntersection(&actPos, &p1Pos);
				player2Over = SDL_HasIntersection(&actPos, &p2Pos);
				instructionsOver = SDL_HasIntersection(&actPos, &iPos);
				quitOver = SDL_HasIntersection(&actPos, &qPos);

				//************************************start Drawing
				//clear sdl renderer
				SDL_RenderClear(renderer);

				//Draw here
				SDL_RenderCopy(renderer, background1, NULL/*cropping*/, &bk1Pos);
				SDL_RenderCopy(renderer, background2, NULL, &bk2Pos);
				SDL_RenderCopy(renderer, title, NULL, &tPos);
				//P1 Button
				if(player1Over)
					SDL_RenderCopy(renderer, Player1Over, NULL, &p1Pos);
				else
					SDL_RenderCopy(renderer, Player1, NULL, &p1Pos);
				//P2 Button
				if(player2Over)
					SDL_RenderCopy(renderer, Player2Over, NULL, &p1Pos);
				else
					SDL_RenderCopy(renderer, Player2, NULL, &p2Pos);
				//Instructions Button
				if(instructionsOver)
					SDL_RenderCopy(renderer, InstructionsOver, NULL, &iPos);
				else
					SDL_RenderCopy(renderer, Instructions, NULL, &iPos);
				//Quit Button
				if(quitOver)
					SDL_RenderCopy(renderer, QuitOver, NULL, &qPos);
				else
					SDL_RenderCopy(renderer, Quit, NULL, &qPos);


				SDL_RenderCopy(renderer, cursor, NULL, &curPos);//last

				//sdl render present
				SDL_RenderPresent(renderer);
			}
			break;
		case INSTRUCTIONS://~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Fix Menu Options
			instructions = true;
			//clear
			cout<<"The Gamestate is Instructions."<<endl;
			cout<<"Hover over a selection and Press A to select.."<<endl;
			while(instructions)
			{
				//setup framerate for this section or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime-lastTime)/1000;
				lastTime = thisTime;
				//check input
				if(SDL_PollEvent(&event))
				{
					//check if sdlwindow is closed
					if(event.type == SDL_QUIT)
					{
						quit = true;
						instructions = false;
						break;
					}
					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)//0=p1, 1=p2
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver)
								{
									instructions = false;
									gS = MENU;
									menuOver = false;
								}
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
					{
						moveCursor(event.caxis);
					}break;
					}
				}
				UpdateBackground();
				updateCursor(deltaTime);
				//check for cursor intersection with menu button
				menuOver = SDL_HasIntersection(&actPos, &mPos);

				//************************************start Drawing
				//clear sdl renderer
				SDL_RenderClear(renderer);

				//Draw here
				SDL_RenderCopy(renderer, background1, NULL/*cropping*/, &bk1Pos);
				SDL_RenderCopy(renderer, background2, NULL, &bk2Pos);
				SDL_RenderCopy(renderer, title, NULL, &tPos);

				//Menu Button
				if(menuOver)
					SDL_RenderCopy(renderer, MenuOver, NULL, &mPos);
				else
					SDL_RenderCopy(renderer, Menu, NULL, &mPos);

				SDL_RenderCopy(renderer, Ititle, NULL, &itPos);

				SDL_RenderCopy(renderer, cursor, NULL, &curPos);//last

				//sdl render present
				SDL_RenderPresent(renderer);
			}
			break;
		case PLAYER1:
			player1 = true;
			//clear
			cout<<"The Gamestate is 1-Player."<<endl;
			cout<<"Press X for Win."<<endl;
			cout<<"Press Y for Lose."<<endl;
			while(player1)
			{
				//setup framerate for this section or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime-lastTime)/1000;
				lastTime = thisTime;
				//check input
				if(SDL_PollEvent(&event))
				{
					//check if sdlwindow is closed
					if(event.type == SDL_QUIT)
					{
						quit = true;
						player1 = false;
						break;
					}
					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)//0=p1, 1=p2
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								player1 = false;
								gS = WIN;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								player1 = false;
								gS = LOSE;
								//break;
							}
							//send button press info to p1
							PlayerOne.OnControllerButton(event.cbutton);
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
					{
						PlayerOne.OnControllerAxis(event.caxis);
					}break;
					}
				}
				UpdateBackground();
				PlayerOne.Update(deltaTime);
				//************************************start Drawing
				//clear sdl renderer
				SDL_RenderClear(renderer);

				//Draw here
				SDL_RenderCopy(renderer, background1, NULL/*cropping*/, &bk1Pos);
				SDL_RenderCopy(renderer, background2, NULL, &bk2Pos);
				SDL_RenderCopy(renderer, Player1, NULL, &p1Pos);

				PlayerOne.Draw(renderer);
				//sdl render present
				SDL_RenderPresent(renderer);
			}
			break;
		case PLAYER2:
			player2 = true;
			//clear
			cout<<"The Gamestate is 2-Player."<<endl;
			cout<<"Press A for Win."<<endl;
			cout<<"Press B for Lose."<<endl;
			while(player2)
			{
				//setup framerate for this section or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime-lastTime)/1000;
				lastTime = thisTime;
				//check input
				if(SDL_PollEvent(&event))
				{
					//check if sdlwindow is closed
					if(event.type == SDL_QUIT)
					{
						quit = true;
						player2 = false;
						break;
					}
					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)//0=p1, 1=p2
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								player2 = false;
								gS = WIN;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								player2 = false;
								gS = LOSE;
								//break;
							}
							//send button press info to p1
							PlayerOne.OnControllerButton(event.cbutton);
							//send button press info to p2
							PlayerTwo.OnControllerButton(event.cbutton);
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
					{
						PlayerOne.OnControllerAxis(event.caxis);
						PlayerTwo.OnControllerAxis(event.caxis);
					}break;
					}
				}
				UpdateBackground();
				PlayerOne.Update(deltaTime);
				PlayerTwo.Update(deltaTime);
				//************************************start Drawing
				//clear sdl renderer
				SDL_RenderClear(renderer);

				//Draw here
				SDL_RenderCopy(renderer, background1, NULL/*cropping*/, &bk1Pos);
				SDL_RenderCopy(renderer, background2, NULL, &bk2Pos);
				SDL_RenderCopy(renderer, Player2, NULL, &p2Pos);

				PlayerOne.Draw(renderer);
				PlayerTwo.Draw(renderer);
				//sdl render present
				SDL_RenderPresent(renderer);
			}
			break;//p2 end
		case WIN:
			win = true;
			//clear
			cout<<"The Gamestate is Win."<<endl;
			cout<<"Hover over a selection and Press A to select.."<<endl;
			while(win)
			{
				//setup framerate for this section or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime-lastTime)/1000;
				lastTime = thisTime;
				//check input
				if(SDL_PollEvent(&event))
				{
					//check if sdlwindow is closed
					if(event.type == SDL_QUIT)
					{
						quit = true;
						win = false;
						break;
					}
					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)//0=p1, 1=p2
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver)
								{
									win = false;
									gS = MENU;
									menuOver = false;
								}
								if(playOver)
								{
									win = false;
									gS = PLAYER1;
									playOver = false;
								}
							}
						}break;
					case SDL_CONTROLLERAXISMOTION:
					{
						moveCursor(event.caxis);
					}break;
					}
				}
				UpdateBackground();
				updateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&actPos, &mPos);
				playOver = SDL_HasIntersection(&actPos, &pPos);

				//************************************start Drawing
				//clear sdl renderer
				SDL_RenderClear(renderer);

				//Draw here
				SDL_RenderCopy(renderer, background1, NULL/*cropping*/, &bk1Pos);
				SDL_RenderCopy(renderer, background2, NULL, &bk2Pos);
				SDL_RenderCopy(renderer, title, NULL, &tPos);
				SDL_RenderCopy(renderer, Win, NULL, &wPos);

				if(menuOver)
					SDL_RenderCopy(renderer, MenuOver, NULL, &mPos);
				else
					SDL_RenderCopy(renderer, Menu, NULL, &mPos);
				//Play Again
				if(playOver)
					SDL_RenderCopy(renderer, PlayOver, NULL, &pPos);
				else
					SDL_RenderCopy(renderer, Play, NULL, &pPos);

				SDL_RenderCopy(renderer, cursor, NULL, &curPos);
				//sdl render present
				SDL_RenderPresent(renderer);
			}
			break;
		case LOSE:
			lose = true;
			//clear
			cout<<"The Gamestate is Lose."<<endl;
			cout<<"Hover over a selection and Press A to select.."<<endl;
			while(lose)
			{
				//setup framerate for this section or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime-lastTime)/1000;
				lastTime = thisTime;
				//check input
				if(SDL_PollEvent(&event))
				{
					//check if sdlwindow is closed
					if(event.type == SDL_QUIT)
					{
						quit = true;
						lose = false;
						break;
					}
					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)//0=p1, 1=p2
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver)
								{
									lose = false;
									gS = MENU;
									menuOver = false;
								}
								if(playOver)
								{
									lose = false;
									gS = PLAYER1;
									playOver = false;
								}
							}
						}break;
					case SDL_CONTROLLERAXISMOTION:
					{
						moveCursor(event.caxis);
					}break;
					}
				}
				UpdateBackground();
				updateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&actPos, &mPos);
				playOver = SDL_HasIntersection(&actPos, &pPos);

				//************************************start Drawing
				//clear sdl renderer
				SDL_RenderClear(renderer);

				//Draw here
				SDL_RenderCopy(renderer, background1, NULL/*cropping*/, &bk1Pos);
				SDL_RenderCopy(renderer, background2, NULL, &bk2Pos);
				SDL_RenderCopy(renderer, title, NULL, &tPos);
				SDL_RenderCopy(renderer, Lose, NULL, &lPos);
				if(menuOver)
					SDL_RenderCopy(renderer, MenuOver, NULL, &mPos);
				else
					SDL_RenderCopy(renderer, Menu, NULL, &mPos);
				//Play Again
				if(playOver)
					SDL_RenderCopy(renderer, PlayOver, NULL, &pPos);
				else
					SDL_RenderCopy(renderer, Play, NULL, &pPos);
				SDL_RenderCopy(renderer, cursor, NULL, &curPos);
				//sdl render present
				SDL_RenderPresent(renderer);
			}
			break;
		default: break;
		}
	}


	/*
	 	 	 	//P1 Button
				if(player1Over)
					SDL_RenderCopy(renderer, Player1Over, NULL, &p1Pos);
				else
					SDL_RenderCopy(renderer, Player1, NULL, &p1Pos);
				//P2 Button
				if(player2Over)
					SDL_RenderCopy(renderer, Player2Over, NULL, &p1Pos);
				else
					SDL_RenderCopy(renderer, Player2, NULL, &p2Pos);
				//Menu Button
				if(menuOver)//only need this?
					SDL_RenderCopy(renderer, MenuOver, NULL, &mPos);
				else
					SDL_RenderCopy(renderer, Menu, NULL, &mPos);
				//Instructions Button
				if(instructionsOver)
					SDL_RenderCopy(renderer, InstructionsOver, NULL, &iPos);
				else
					SDL_RenderCopy(renderer, Instructions, NULL, &iPos);
				//Quit Button
				if(quitOver)
					SDL_RenderCopy(renderer, QuitOver, NULL, &qPos);
				else
					SDL_RenderCopy(renderer, Quit, NULL, &qPos);
	 */



	//SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
