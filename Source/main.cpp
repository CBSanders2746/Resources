// Example program:
// Using SDL2 to create an application window

#if defined(__APPLE__)
	#include "SDL2/SDL.h"
	#include "SDL2_Image/SDL_image.h"
#endif

#if defined(__linux__)
	#include "SDL2/SDL.h"
#endif

#if defined(_WIN32) || (_WIN64)
	#include "SDL.h"
#endif

#include <stdio.h>
#include <iostream>
using namespace std;

//code for framerate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;


int main(int argc, char* argv[]) {

	#if defined(__APPLE__)
		cout << "Running on Apple." << endl;
		//get current working directory
		string s_cwd(getcwd(NULL, 0));
		//create a string linking to the mac's images folder
		string s_cwd_images = s_cwd + "/Resources/Images";
		//test
		//cout<<s_cwd_images<<endl;
	#endif

	#if defined(__linux__)
		cout << "Running on Linux." << endl;
	#endif

	#if defined(_WIN32_) || (_WIN64_)
		cout << "Running on Windows." << endl;
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

    //*********Create Background Image**************
    string BGpath = s_cwd_images + "/1024x768.png";
    string IMGcur = s_cwd_images + "/DotCursor.png";

    //cout<<BGpath<<endl;

    //create SDL surface to hold the background image
    SDL_Surface *surface = IMG_Load(BGpath.c_str());
    SDL_Surface *CURsurface = IMG_Load(IMGcur.c_str());

    //create a SDL texture
    SDL_Texture *background1;
    SDL_Texture *background2;
    SDL_Texture *cursor;

    //place surface info into the texture
    background1 = SDL_CreateTextureFromSurface(renderer, surface);
    background2 = SDL_CreateTextureFromSurface(renderer, surface);
    cursor = SDL_CreateTextureFromSurface(renderer, CURsurface);

    //free the SDL surfaces
    SDL_FreeSurface(surface);
    SDL_FreeSurface(CURsurface);

    //create the SDL_Rectangle for the texture's position and size -x,y,w,h
    SDL_Rect bk1Pos;
    SDL_Rect bk2Pos;
    SDL_Rect curPos;

    //set the x, y, w, and h for the rectangle
    bk1Pos.x = 0;
    bk1Pos.y = 0;
    bk1Pos.w = 1024;
    bk1Pos.h = 768;

    bk2Pos.x = 0;
    bk2Pos.y = -768;
    bk2Pos.w = 1024;
    bk2Pos.h = 768;

    curPos.x = 100;
    curPos.y = 100;
    curPos.w = 16;
    curPos.h = 16;

    //bg movement speed
    int backSpeed = 100;

    //set up temp vars to hold movement
    float back1PosX = 0, back1PosY = 0;
    float back2PosX = 0, back2PosY = -768;


	/*//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Get window surface
	screenSurface = SDL_GetWindowSurface( window );

	//Fill the surface white
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0xFF ) );

	//Update the surface
	SDL_UpdateWindowSurface( window );*/

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
			cout<<"Press A for Instructions."<<endl;
			cout<<"Press B for 1-Player."<<endl;
			cout<<"Press X for 2-Player."<<endl;
			cout<<"Press Y to Quit.."<<endl;
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
								menu = false;
								gS = INSTRUCTIONS;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								menu = false;
								gS = PLAYER1;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								menu = false;
								gS = PLAYER2;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								menu = false;
								quit = true;
								//break;
							}
						}
						break;
					}

				}
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

				//start Drawing
				//clear sdl renderer
				SDL_RenderClear(renderer);

				//Draw here
				SDL_RenderCopy(renderer, background1, NULL/*cropping*/, &bk1Pos);
				SDL_RenderCopy(renderer, background2, NULL/*cropping*/, &bk2Pos);
				SDL_RenderCopy(renderer, cursor, NULL/*cropping*/, &curPos);

				//sdl render present
				SDL_RenderPresent(renderer);
			}
			break;
		case INSTRUCTIONS:
			instructions = true;
			//clear
			cout<<"The Gamestate is Instructions."<<endl;
			cout<<"Press START for Menu."<<endl;
			cout<<"Press B for 1-Player."<<endl;
			cout<<"Press X for 2-Player."<<endl;
			cout<<"Press Y to Quit.."<<endl;
			while(instructions)
			{
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
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
							{
								instructions = false;
								gS = MENU;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								instructions = false;
								gS = PLAYER1;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								instructions = false;
								gS = PLAYER2;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								instructions = false;
								quit = true;
								//break;
							}
						}
						break;
					}
				}
			}
			break;
		case PLAYER1:
			player1 = true;
			//clear
			cout<<"The Gamestate is 1-Player."<<endl;
			cout<<"Press A for Win."<<endl;
			cout<<"Press B for Lose."<<endl;
			while(player1)
			{
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
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								player1 = false;
								gS = WIN;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								player1 = false;
								gS = LOSE;
								//break;
							}
						}
						break;
					}
				}
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
						}
						break;
					}
				}
			}
			break;//p2 end
		case WIN:
			win = true;
			//clear
			cout<<"The Gamestate is Win."<<endl;
			cout<<"Press A for Menu."<<endl;
			cout<<"Press B to Quit.."<<endl;
			while(win)
			{
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
								win = false;
								gS = MENU;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								win = false;
								quit = true;
								//break;
							}
						}
						break;
						}
					}
				}
				break;
		case LOSE:
			lose = true;
			//clear
			cout<<"The Gamestate is Lose."<<endl;
			cout<<"Press A for Menu."<<endl;
			cout<<"Press B to Quit.."<<endl;
			while(lose)
			{
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
								lose = false;
								gS = MENU;
								//break;
							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								lose = false;
								quit = true;
								//break;
							}
						}
						break;
						}
					}
				}
				break;
		default: break;
		}
	}






    //SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
