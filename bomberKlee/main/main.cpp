#pragma comment(lib, "SDL2_mixer.lib")
#include "SDL.h"
#include "stdlib.h"
#include <cstdio>
#include <locale>
#include <iostream>
#include <cmath>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SDL_mixer.h"
#include "main.h"
#include <fstream>

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define COUNT_WALL 10
#define COUNT_BONUS 6
#define COUNT_ENEMY 2 

SDL_Rect wall[COUNT_WALL] = {
	0,0,50,SCREEN_HEIGHT,
	0,0,SCREEN_WIDTH,50,
	SCREEN_WIDTH - 50,0,50,SCREEN_HEIGHT,
	0,550,SCREEN_WIDTH,50,
	200,270,SCREEN_WIDTH / 3,50,
	485,400,SCREEN_WIDTH / 3,50,
	150,SCREEN_HEIGHT - 100,50,50,
	150,100,50,50,
	150,50,50,50,
	550,150,SCREEN_WIDTH / 4,50
};

SDL_Rect cat = { 51,51,70,75 };
SDL_Rect enemy[COUNT_ENEMY] = { 600,100,40,70 };

Mix_Chunk* Sound = NULL;
Mix_Music* fon = NULL;

struct Records 
{
	char name[50];
	int rec = 0;
};

SDL_Texture* get_text_texture(SDL_Renderer*& renderer, char* text, TTF_Font* font) 
{
	SDL_Surface* textSurface = NULL;
	SDL_Color fore_color = { 255,255,255 };
	SDL_Color back_color = { 100,0,0,100 };
	textSurface = TTF_RenderText_Shaded(font, text, fore_color, back_color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	return texture;

}


void draw_text(SDL_Renderer*& renderer, SDL_Texture* texture)
{
	int y = 0, w = 300, h = 100, x = SCREEN_WIDTH / 3;
	SDL_Rect rect[3] = { x,120,w,h, x,250,w,h, x,380,w,h };
	for(int i = 0; i < 3;i++)
		SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
}

void DrawMap(SDL_Renderer*& renderer,SDL_Rect wall, SDL_Texture* image) 
{
	SDL_RenderCopy(renderer, image, nullptr, &wall);
	SDL_RenderPresent(renderer);
}

void loadmusic(int snd)
{
	if (snd == 1) {
		fon = Mix_LoadMUS("mazLove.mp3");
		Mix_PlayMusic(fon, -1);
	}
	else {
		fon = Mix_LoadMUS("sleep.mp3");
		Mix_PlayMusic(fon, -1);
	}

}

void sound(int gg)
{
	if (gg == 1) {
		Sound = Mix_LoadWAV("bUmBuM.mp3");
		Mix_VolumeChunk(Sound, 128);
		Mix_PlayChannel(-1, Sound, 0);
	}
	else if (gg ==2)
	{
		Sound = Mix_LoadWAV("win.mp3");
		Mix_VolumeChunk(Sound, 128);
		Mix_PlayChannel(-1, Sound, 0);
	}
	else
	{
		Sound = Mix_LoadWAV("dead.mp3");
		Mix_VolumeChunk(Sound, 128);
		Mix_PlayChannel(-1, Sound, 0);
	}
}


int CheckWl(SDL_Rect r1, SDL_Rect cat) {
	
	if (cat.x > r1.x + r1.w || cat.x + cat.w < r1.x || cat.y > r1.y + r1.h || cat.y + cat.h < r1.y) return 0;
	
	return 1;
}

void player(int& x1, int& y1, bool& quit, SDL_Renderer* renderer, SDL_Event event)
{
	int dmove = 10;

	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_UP))
	{
		y1 -= dmove;
	}
	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_DOWN))
	{
		y1 += dmove;
	}
	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LEFT))
	{
		x1 -= dmove;
	}
	if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_RIGHT))
	{
		x1 += dmove;
	}
}


void win(SDL_Renderer* renderer)
{
	int gg = 2;
	sound(gg);
	SDL_Rect win = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Texture* image5 = IMG_LoadTexture(renderer, "win.jpg");
	SDL_RenderCopy(renderer, image5, nullptr, &win);
	SDL_RenderPresent(renderer);
	SDL_Delay(4000);
}

void not_win(SDL_Renderer* renderer) {
	int gg = 3;
	sound(gg);
	SDL_Rect notWin = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Texture* image5 = IMG_LoadTexture(renderer, "dead.jpg");
	SDL_RenderCopy(renderer, image5, nullptr, &notWin);
	SDL_RenderPresent(renderer);
	SDL_Delay(4000);
}

void SaveRecords(Records* record,int reccnt)
{
	ofstream fic("out.txt");

	for (int i = 0; i < reccnt; i++) {
		fic << record[i].name << " " << record[i].rec << endl;
	}
	fic.close();
}


void level(SDL_Renderer*& renderer, int i) {

	TTF_Init();
	TTF_Font* my_font = TTF_OpenFont("Text.ttf", 100);
	SDL_Texture* textTexture;

	char text[] = "nemu";
	struct Records record[10];

	SDL_Event event;
	bool quit = false;

	int life = 2, life1 = 1, cnt = 0, gg, level=i;
	int arr[3] = {}, rec=0;
	
	textTexture = get_text_texture(renderer, text, my_font);
	if (level == 2) enemy[1] = { 500,500,40,70 };
	
	
	SDL_Texture* image = IMG_LoadTexture(renderer, "wall3.bmp");
	int x = 51, y = 51;
	
	SDL_Texture* image2 = IMG_LoadTexture(renderer, "player.png");
	

	SDL_Rect bonus[COUNT_BONUS];
	SDL_Texture* image3 = IMG_LoadTexture(renderer, "bomb.png");
	SDL_Texture* image4 = IMG_LoadTexture(renderer, "enemy.png");

	int wB = 50;
	int wH = 50;
	int k = 0;

	for (int i = 0; i < COUNT_BONUS; i++) {
		bonus[i] = { rand() % 500 + 50, rand() % 500 + 50, wB, wH };
	}
	
	int ch_enemy = 0;
	

	while (life != 0)
	{

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = true;
			int dmove1 = 2;
			
			if (level == 3) { dmove1 = 4; enemy[1] = {}; }
			int x1 = 0, y1 = 0;

			double dir_x = cat.x - enemy[0].x;
			double dir_y = cat.y - enemy[0].y;

			double dir_x1 = cat.x - enemy[1].x;
			double dir_y1 = cat.y - enemy[1].y;

			double hyp = sqrt(dir_x * dir_x + dir_y * dir_y);
			double hyp1 = sqrt(dir_x1 * dir_x1 + dir_y1 * dir_y1);

			dir_x /= hyp; dir_y /= hyp;
			dir_x1 /= hyp1; dir_y1 /= hyp1;

			player(x1, y1, quit, renderer, event);

			cat.x += x1;
			cat.y += y1;

			enemy[0].x += dir_x * dmove1; enemy[0].y += dir_y * dmove1;
			enemy[1].x += dir_x1 * dmove1; enemy[1].y += dir_y1 * dmove1;
			bool t = false;
			for (int i = 0; i < COUNT_WALL; i++) {
				if (CheckWl(wall[i], cat)) {
					t = true;
					break;
				}
			}

			if (t) {
				cat.x -= x1;
				cat.y -= y1;
			}

			int k1;
			for (int i = 0; i < COUNT_BONUS; i++) {
				if (CheckWl(bonus[i], cat) && bonus[i].w != 0 && (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_q)) {
					k1 = rand() % 20 + 10;
					k += k1;
					bonus[i].w = 0;
					gg = 1;
					sound(gg);
					cout << i + 1 << " bonus: " << k1 << endl;

					if (bonus[0].w == 0 && bonus[1].w == 0 && bonus[2].w == 0 && bonus[3].w == 0 && bonus[4].w == 0 && bonus[5].w == 0) { cnt = 6; win(renderer); life = 0; };
					break;
				}
			}


			bool t2 = false;
			for (int i = 0; i < COUNT_ENEMY; i++)
				if (CheckWl(enemy[i], cat))
				{
					t2 = true;
					life--;
					if (life == 0)life1 = 0;
					break;
				}

			if (t2) {
				cat.x -= x1;
				cat.y -= y1;
			}
			
			SDL_DestroyTexture(textTexture);
			textTexture = get_text_texture(renderer, text, my_font);
		}


		// Игровая логика 

		SDL_SetRenderDrawColor(renderer, 254, 115, 136, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, image2, nullptr, &cat);
		for (int i = 0; i < COUNT_WALL; i++) {
			SDL_RenderCopy(renderer, image, nullptr, &wall[i]);
		}

		for (int i = 0; i < COUNT_BONUS; i++) {
			SDL_RenderCopy(renderer, image3, nullptr, &bonus[i]);
		}

		for (int i = 0; i < COUNT_ENEMY; i++) {

			SDL_RenderCopy(renderer, image4, nullptr, &enemy[i]);
		}

		// Отрисовка сцены 
		//draw_text(renderer, textTexture);
		SDL_RenderPresent(renderer);
	}


	TTF_CloseFont(my_font);
	TTF_Quit();
	cat.x = 51;
	cat.y = 51;
	if (life1 == 0)
	{
		gg = 3;
		not_win(renderer);
		cout << "you dead" << endl << "your score: " << k << endl;
		rec = k;
		
	}

	if (cnt == 6)
		cout << "your score: " << k << endl;
	
	int reccnt=0;
	/*if (level == 3) {
		if(reccnt <10)reccnt++;
		cout << "name: ";
		cin >> record[reccnt].name;
		record[reccnt].rec = k;
		SaveRecords(record,reccnt);
	}*/
}


void Menu(SDL_Renderer* renderer)
{
	Mix_Init(0);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	int y=0, w=300,h=100,x=SCREEN_WIDTH/3;
	SDL_Rect button[4]={ x,70,w,h, x,200,w,h, x,330,w,h, x,460,w,h };
	SDL_Rect rules = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
	int x_r=SCREEN_WIDTH/3, y_r=0;
	SDL_Texture* butt_img1 = IMG_LoadTexture(renderer, "button_start.png");
	SDL_Texture* butt_img2_1 = IMG_LoadTexture(renderer, "button_v_on.png");
	SDL_Texture* butt_img2_2 = IMG_LoadTexture(renderer, "button_v_off.png");
	SDL_Texture* butt_img3_1 = IMG_LoadTexture(renderer, "button_rules.png");
	SDL_Texture* butt_img3_2 = IMG_LoadTexture(renderer, "rules.jpg");
	SDL_Texture* butt_img4 = IMG_LoadTexture(renderer, "button_exit.png");
	
	int snd = -1, rule = -1;
	
	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&event)) {
			
			SDL_Rect cursor = { event.button.x, event.button.y,10,10 };

			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			{
				for (int i = 0; i < 4; i++)
				{
					if (CheckWl(button[i], cursor))
					{
						if (i == 0)
						{
							for (int i = 1; i < 4; i++)
								level(renderer, i);

						}
							

						if (i == 1)
						{
							snd *= -1;
							//cout << snd;
							loadmusic(snd);
						}

						if (i == 3)
						{
							rule *= -1;
						}
							
						if (i == 2)
							quit = true;
					}
				}
			}
			
		}
		/* Игровая логика */
		SDL_SetRenderDrawColor(renderer, 204, 102, 255, 0);
		SDL_RenderClear(renderer);
		/* Отрисовка сцены */
		
		SDL_RenderCopy(renderer, butt_img1, nullptr, &button[0]);
		if (snd == 1)
			SDL_RenderCopy(renderer, butt_img2_1, nullptr, &button[1]);
		else
			SDL_RenderCopy(renderer, butt_img2_2, nullptr, &button[1]);

		SDL_RenderCopy(renderer, butt_img4, nullptr, &button[2]);
		if (rule == 1)
		{
			SDL_RenderCopy(renderer, butt_img3_2, nullptr, &rules);
			SDL_RenderCopy(renderer, butt_img3_1, nullptr, &button[3]);
		}

		SDL_RenderCopy(renderer, butt_img3_1, nullptr, &button[3]);

		
		//draw_text(renderer, textTexture);
		SDL_RenderPresent(renderer);
	}
	//
	
}


int main(int argc, char** argv)
{
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(u8"bomberKlee",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Surface* surface = NULL;

	Menu(renderer);

	Mix_CloseAudio();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 1;
}
