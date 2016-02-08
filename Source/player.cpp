#include "player.h"

//
const int JOYSTICK_DEAD_ZONE = 8000;//customizable for own controller

//
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y)
{
	playerNum = pNum;

	speed = 500.0f;

	if(playerNum == 0)
		playerPath = filePath+"Player1.png";
	else
		playerPath = filePath+"Player2.png";

	surface = IMG_Load(playerPath.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	posRect.x = x;
	posRect.y = y;
	//Use SDL_Query to get the W and H of the player's House.
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	posX = x;
	posY = y;

	xDir = 0;
	yDir = 0;

	string bulletPath;
	if (playerNum == 0)
		bulletPath = filePath + "Bullet1.png";
	else
		bulletPath = filePath + "Bullet2.png";
	for (int i = 0; i < 10; i++)
	{
		Bullet temp(renderer, bulletPath, -1000, -1000);
		bulletList.push_back(temp);
	}
}

void Player::Update(float deltaTime)
{
	//update values
	posX += (speed * xDir)*deltaTime;
	posY += (speed * yDir)*deltaTime;

	//assign to rects
	posRect.x = (int)(posX+0.5f);
	posRect.y = (int)(posY+0.5f);

	//off the screen x-axis
	if(posRect.x<0)
	{
		posRect.x = 0;
		posX = posRect.x;
	}
	if(posRect.x>1024 - posRect.w)
	{
		posRect.x = 1024 - posRect.w;
		posX = posRect.x;
	}

	//off the screen y-axis
	if(posRect.y<0)
	{
		posRect.y = 0;
		posY = posRect.y;
	}
	if(posRect.y>768 - posRect.h)
	{
		posRect.y = 768 - posRect.h;
		posY = posRect.y;
	}

	for (int i = 0; i < bulletList.size(); i++)
		if (bulletList[i].active)
			bulletList[i].Update(deltaTime);
}

void Player::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
	for (int i = 0; i < bulletList.size(); i++)
		if (bulletList[i].active)
			bulletList[i].Draw(renderer);
}

void Player::CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			bulletList[i].active = true;
			bulletList[i].posRect.x = (posX + (posRect.w / 2));
			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w / 2));
			bulletList[i].posRect.y = posRect.y;
			bulletList[i].posX = posX;
			bulletList[i].posY = posY;
			break;
		}
	}
}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if (event.which == 0 && playerNum == 0)
		if (event.button == 0)
			

	if (event.which == 1 && playerNum == 1)
		if(event.button == 0)
			CreateBullet();
}

void Player::OnControllerAxis (const SDL_ControllerAxisEvent event)
{
	//Axis movements and buton presses both sent here as SDL_ControllerAxisEvent structures
	if(event.which == 0 && playerNum == 0)
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
	}//Note to self: switch the elseif with a regular if if there are problems
	else if(event.which == 1 && playerNum == 1)
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
Player::~Player()
{
	SDL_DestroyTexture(texture);
}





