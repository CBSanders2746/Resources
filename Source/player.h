/* Notes:
 *
 *
 *
 * */

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_Image/SDL_image.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

#include <string.h>
#include <iostream>
using namespace std;

class Player
{
public:
	//hold the path to the player's texture
	string playerPath;
	//the player's number
	int playerNum;
	//load the png in
	SDL_Surface *surface;
	//holds the image
	SDL_Texture *texture;
	//XYWH
	SDL_Rect posRect;
	//joystick axis (-1, 0, 1)
	float xDir;
	float yDir;
	//player speed1
	float speed;
	//player's position to avoid precision loss
	float posX, posY;

	//Player's creation method using passed vals
	//starting pos x/y
	Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y);
	//update player using deltaTime
	void Update(float deltaTime);
	//draw player
	void Draw(SDL_Renderer *renderer);
	//allows the player to move via the joystick axis
	void OnControllerAxis (const SDL_ControllerAxisEvent event);
	//
	void OnControllerButton(const SDL_ControllerButtonEvent event);
	~Player();
};



