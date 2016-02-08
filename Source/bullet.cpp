#include "bullet.h"

//Bullet::

Bullet::Bullet(SDL_Renderer *renderer, string filePath, float x, float y)
{
	active = false;

	speed = 800.0f;

	SDL_Surface *surface = IMG_Load(filePath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	posRect.x - x - (posRect.w / 2);
	posRect.y - y;

	posX = x;
	posY = y;

	xDir = 0;
	yDir = -1;
}

void Bullet::Update(float deltaTime)
{
	if (active)
	{
		posY += (speed*yDir)*deltaTime;
		posRect.y = (int)(posY + 0.5f);
		if (posRect.y < (0 - posRect.h))
		{
			posRect.x = -1000;
			posX = posRect.x;
			active = false;
		}
	}
}

void Bullet::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

Bullet::~Bullet()
{
	SDL_DestroyTexture(texture);
}