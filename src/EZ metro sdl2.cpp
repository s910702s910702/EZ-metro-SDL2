#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define SCREEN_FPS  60
#define MAP_SIZE 5

SDL_Window * window = NULL;
SDL_Surface * screen = NULL;
SDL_Surface * image = NULL;
SDL_Renderer * renderer = NULL;
SDL_Texture * texture = NULL;
SDL_Event event;

typedef struct {
	SDL_Rect rect;
	SDL_Surface * image;
	SDL_Texture * texture;
} objects;

bool init() {
	if(SDL_Init(SDL_INIT_VIDEO) == -1) return false;
	int IMG_Init(IMG_INIT_PNG);
	window = SDL_CreateWindow("Wahaha~EZ metro", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(window = NULL) return false;
	else screen = SDL_GetWindowSurface(window);

	return true;
}

int play_menu() {
	int type = 0;

	freopen("menu.in", "r", stdin);

	int menu_case = 0;

	scanf("%d", &menu_case);

	SDL_Rect rect[menu_case];

	for(int i = 0; i < menu_case; i++) {
		char file[100];
		int w, h, x, y;
		scanf("%s%d%d%d%d", &file, &w, &h, &x, &y);

		rect[i] = {x, y, w, h};


		//SDL_Rect rect = {x, y, w, h};
		image = IMG_Load(file);
		texture = SDL_CreateTextureFromSurface(renderer, image);

		SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
		SDL_RenderPresent(renderer);

	}

	while(event.type != SDL_QUIT && type ==  0) {
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) break;
			else if(event.type == SDL_MOUSEBUTTONDOWN) {
				if(event.button.button == SDL_BUTTON_LEFT) {
					int x = event.button.x;
					int y = event.button.y;

					for(int i = 2; i < 5; i++)
						if(x >= rect[i].x && x <= rect[i].x + rect[i].w && y >= rect[i].y && y <= rect[i].y + rect[i].h) {

							type = i - 1;
						}

				}
			}
		}
		SDL_Delay(1000 / SCREEN_FPS + 10);
	}

	return type;
}

int play_game() {
	int type = 0;

		freopen("play.in", "r", stdin);

		int play_case = 0;

		scanf("%d", &play_case);

		SDL_Rect rect[play_case];

		for(int i = 0; i < play_case; i++) {
			char file[100];
			int w, h, x, y;
			scanf("%s%d%d%d%d", &file, &w, &h, &x, &y);

			rect[i] = {x, y, w, h};


			//SDL_Rect rect = {x, y, w, h};
			image = IMG_Load(file);
			texture = SDL_CreateTextureFromSurface(renderer, image);

			SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
			SDL_RenderPresent(renderer);

		}

		// read 8 image

		int object_number;
		scanf("%d", &object_number);


		objects object[object_number];

		for(int i = 0; i < object_number; i++) {
			object[i].image = NULL;
			object[i].texture = NULL;
		}


		for(int i = 0; i < object_number; i++) {
			char file[100];
			int w, h, x, y;
			scanf("%s%d%d%d%d", &file, &w, &h, &x, &y);

			object[i].rect = {x, y, w, h};

			object[i].image = IMG_Load(file);
			object[i].texture = SDL_CreateTextureFromSurface(renderer, object[i].image);
		}

		//rect[i] = {x, y, w, h};
		vector<SDL_Rect> orect;
		objects maps[5][5];
		bool book[5][5];
		for(int i = 0; i < 5; i++)
			for(int j = 0; j < 5; j++)
				book[i][j] = false;
		// 5 * 5 , map
		int inix = 380, iniy = 110;
		for(int i = 0; i < MAP_SIZE; i++) {
			for(int j = 0; j < MAP_SIZE; j++) {
				SDL_Rect paste = object[8].rect;

				maps[i][j] = object[8];


				paste.x = inix + i * paste.h;
				paste.y = iniy + j * paste.w;

				maps[i][j].rect = paste;

				orect.push_back(paste);

				SDL_RenderCopy(renderer, maps[i][j].texture, NULL, &paste);
				SDL_RenderPresent(renderer);
			}
		}

		//{
			// garbage can
			SDL_Rect paste = object[7].rect;

			SDL_Rect map[5][5];

			paste.x = 1050;
			paste.y = 10;
			SDL_RenderCopy(renderer, object[7].texture, NULL, &paste);
			SDL_RenderPresent(renderer);

			paste.x = 1050;
			paste.y = 720 - object[7].rect.h - 10;

			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[7].texture, NULL, &paste);
			SDL_RenderPresent(renderer);


			// bomb
			paste = object[6].rect;
			paste.x = 250;
			paste.y = 10;
			SDL_RenderCopy(renderer, object[6].texture, NULL, &paste);
			SDL_RenderPresent(renderer);

			paste.x = 250;
			paste.y = 720 - object[6].rect.h - 10;

			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[6].texture, NULL, &paste);
			SDL_RenderPresent(renderer);

			objects rails[2][5];

			// random rail

			srand(NULL);
			for(int i = 0; i < 5; i++) {
				int o = rand() % 6;

				paste = object[o].rect;
				paste.x = i * 100 + 450;
				paste.y = 10;

				rails[0][i] = object[o];
				rails[0][i].rect = paste;

				SDL_RenderCopy(renderer, rails[0][i].texture, NULL, &paste);
				SDL_RenderPresent(renderer);

				o = rand() % 6;

				paste = object[o].rect;
				paste.x = i * 100 + 450;
				paste.y = 720 - object[o].rect.h - 10;

				rails[1][i] = object[o];
				rails[1][i].rect = paste;

				SDL_RenderCopy(renderer, rails[1][i].texture, NULL, &rails[1][i].rect);
				SDL_RenderPresent(renderer);

			}

			// start and end
			paste = object[9].rect;
			int s1x = rand() % 2 * 4, s1y = rand() % 2 * 4;
			int s2x = rand() % 2 * 4, s2y = rand() % 2 * 4;
			book[s1x][s1y] = true;
			book[s2x][s2y] = true;
			while(s1x == s2x && s1y == s2y) {
				s2x = rand() % 2 * 4;
				s2y = rand() % 2 * 4;
			}

			paste.x = inix + s1x * paste.h;
			paste.y = iniy + s1y * paste.w;

			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[9].texture, NULL, &paste);
			SDL_RenderPresent(renderer);
			paste.x = inix + s2x * paste.h;
			paste.y = iniy + s2y * paste.w;

			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[9].texture, NULL, &paste);
			SDL_RenderPresent(renderer);

			paste = object[10].rect;

			paste.x = inix + 2 * paste.h;
			paste.y = iniy + 2 * paste.w;

			orect.push_back(paste);
			book[2][2] = true;
			SDL_RenderCopy(renderer, object[10].texture, NULL, &paste);
			SDL_RenderPresent(renderer);
		//}


		//orect: 0 ~ 24 map, 25 26 bargage can, 27 start1, 28 start2, 29 end;
		//rails: 0 ~ 4 up, 5 ~ 8 down;


		int select = 0;

		while(event.type != SDL_QUIT && type ==  0) {
			if(SDL_PollEvent(&event)) {
				if(event.type == SDL_QUIT) break;
				else if(event.type == SDL_MOUSEBUTTONDOWN) {
					if(event.button.button == SDL_BUTTON_LEFT) {
						int x = event.button.x;
						int y = event.button.y;


						for(int i = 0; i < 25; i++) {
							if((x >= orect[i].x && x <= orect[i].x + orect[i].w && y >= orect[i].y && y <= orect[i].y + orect[i].h) && book[i / 5][i % 5] == false) {
								cout << "Got " << i << endl;
								book[i / 5][i % 5] = true;
								maps[i / 5][i % 5].texture = rails[select][0].texture;

								for(int j = 1; j < 5; j++) {
									SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

									rails[select][j - 1].texture = rails[select][j].texture;
									SDL_RenderFillRect(renderer, &rails[select][j - 1].rect);
									SDL_RenderCopy(renderer, rails[select][j - 1].texture, NULL, &rails[select][j - 1].rect);
									SDL_RenderPresent(renderer);
								}
							 	int o = rand() % 6;

								rails[select][4].texture = object[o].texture;

								SDL_RenderFillRect(renderer, &rails[select][4].rect);
								SDL_RenderCopy(renderer, rails[select][4].texture, NULL, &rails[select][4].rect);
								SDL_RenderPresent(renderer);

								SDL_RenderCopy(renderer, maps[i / 5][i % 5].texture, NULL, &maps[i / 5][i % 5].rect);
								SDL_RenderPresent(renderer);

								select ^= 1;

							}
						}
					}
				}

			}
			SDL_Delay(1000 / SCREEN_FPS + 10);
		}

		return type;
}

int play_option() {
	int type = 0;

	return type;
}

int play_staff() {
	int type = 0;

	return type;
}

void clean_up() {
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char * args[]) {
	if(!init()) return 0;

	int type = 0; // 0 for menu, 1 for game play, 2 for option, 3 for staff
	bool quit = false;




	while(!quit) {
		SDL_PollEvent(&event);

		if(event.type == SDL_QUIT) {
			break;
		}

		switch(type) {

		case 0:
			type = play_menu();
			break;
		case 1:
			//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing file", "Got play.", NULL);
			type = play_game();
			break;
		case 2:
			type = play_option();
			break;
		case 3:
			type = play_staff();
			break;
		}

		SDL_Delay(1000 / SCREEN_FPS);
	}
	clean_up();
	return 0;
}

