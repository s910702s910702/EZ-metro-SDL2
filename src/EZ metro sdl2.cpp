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


// 0 for nothing , 1 for up is conect, 2 for down is connected, 4 for left is connected, 8 for right is connected
int rconnect(int x) {
	if(x == 0) return 1 | 4;
	if(x == 1) return 1 | 8;
	if(x == 2) return 2 | 8;
	if(x == 3) return 2 | 4;
	if(x == 4) return 4 | 8;
	if(x == 5) return 1 | 2;
}

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
						if(x >= rect[i].x && x <= rect[i].x + rect[i].w && y >= rect[i].y && y <= rect[i].y + rect[i].h) type = i - 1;
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

		// read object image

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

		// mapt is for checking the rail is connected or not
		// 0 for nothing. 1 for up. 2 for down. 4 for left. 8 for right d

		vector<SDL_Rect> orect;
		objects maps[5][5];
		int book[5][5];
		int mapt[5][5];

		for(int i = 0; i < 5; i++)
			for(int j = 0; j < 5; j++) {
				mapt[i][j] = 0;
				book[i][j] = 0;
			}

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


			// garbage can
			SDL_Rect paste = object[7].rect;

			paste.x = 990;
			paste.y = 30;

			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[7].texture, NULL, &paste);
			//SDL_RenderPresent(renderer);


			paste.x = 990;
			paste.y = 720 - object[7].rect.h - 25;

			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[7].texture, NULL, &paste);
			//SDL_RenderPresent(renderer);


			// bomb
			paste = object[6].rect;
			paste.x = 200;
			paste.y = 30;

			orect.push_back(paste);
			SDL_RenderCopy(renderer, object[6].texture, NULL, &paste);
			//SDL_RenderPresent(renderer);

			paste.x = 200;
			paste.y = 720 - object[6].rect.h - 25;

			cout << orect.size() << endl;
			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[6].texture, NULL, &paste);
			SDL_RenderPresent(renderer);

			objects rails[2][5];

			// random rail
			// state is for checking the rail is connected or not
			// 0 for nothing , 1 for up is conect, 2 for down is connected, 4 for left is connected, 8 for right is connected
			int railt[2][5];

			srand(NULL);
			for(int i = 0; i < 5; i++) {
				int o = rand() % 6;

				paste = object[o].rect;
				paste.x = i * 100 + 370;
				paste.y = 10;

				railt[0][i] = rconnect(o);
				rails[0][i] = object[o];
				rails[0][i].rect = paste;

				SDL_RenderCopy(renderer, rails[0][i].texture, NULL, &paste);
				SDL_RenderPresent(renderer);

				o = rand() % 6;

				paste = object[o].rect;
				paste.x = i * 100 + 400;
				paste.y = 720 - object[o].rect.h - 10;

				railt[1][i] = rconnect(o);
				rails[1][i] = object[o];
				rails[1][i].rect = paste;

				SDL_RenderCopy(renderer, rails[1][i].texture, NULL, &rails[1][i].rect);
				SDL_RenderPresent(renderer);

			}

			// start and end

			paste = object[9].rect;

			// s1
			int s1x = rand() % 2 * 4, s1y = rand() % 2 * 4;
			book[s1x][s1y] = -1;
			mapt[s1x][s1y] = 1 | 2 | 4 | 8;

			paste.x = inix + s1x * paste.h;
			paste.y = iniy + s1y * paste.w;

			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[9].texture, NULL, &paste);

			// s2
			int s2x = rand() % 2 * 4, s2y = rand() % 2 * 4;

			while(s1x == s2x && s1y == s2y) {
				s2x = rand() % 2 * 4;
				s2y = rand() % 2 * 4;
			}

			book[s2x][s2y] = -1;
			mapt[s2x][s2y] = 1 | 2 | 4 | 8;

			paste = object[10].rect;
			paste.x = inix + s2x * paste.h;
			paste.y = iniy + s2y * paste.w;

			orect.push_back(paste);

			SDL_RenderCopy(renderer, object[10].texture, NULL, &paste);

			// e
			paste = object[11].rect;

			paste.x = inix + 2 * paste.h;
			paste.y = iniy + 2 * paste.w;

			book[2][2] = -2;
			mapt[2][2] = 1 & 2 & 4 & 8;

			orect.push_back(paste);
			SDL_RenderCopy(renderer, object[11].texture, NULL, &paste);
			SDL_RenderPresent(renderer);



		//orect: 0 ~ 24 map, 25 26 bargage can, 27 28 bomb, 29 start1, 30 start2, 31 end;
		//rails: 0 ~ 4 up, 5 ~ 8 down;

		int select = 0;

		// gamet: 0 for normal, 1 for bomb, 2 for garbage can
		int gamet = 0;

		while(event.type != SDL_QUIT) {
			if(SDL_PollEvent(&event)) {
				if(event.type == SDL_QUIT) break;
				else if(event.type == SDL_MOUSEBUTTONDOWN) {
					if(event.button.button == SDL_BUTTON_LEFT) {
						int x = event.button.x;
						int y = event.button.y;

						for(int i = 0; i < 5; i++) {
							cout <<	railt[0][i] << " ";
						}
						cout << endl;

						for(int i = 0; i < 5; i++) {
							cout << railt[1][i] << " ";
						}
						cout << endl;

						// click bomb
						for(int i = 27; i <= 28; i++) {
							if(x >= orect[i].x && x <= orect[i].x + orect[i].w && y >= orect[i].y && y <= orect[i].y + orect[i].h) {
								cout << "Got bomb" << endl;
								gamet = 1;
							}
						}


						// click garbate can
						for(int i = 25; i <= 26; i++) {
							if(x >= orect[i].x && x <= orect[i].x + orect[i].w && y >= orect[i].y && y <= orect[i].y + orect[i].h) {
								cout << "Got garbage can" << endl;
								//gamet = 2;

								for(int j = 1; j < 5; j++) {
									SDL_SetRenderDrawColor(renderer, 250, 250, 220, 210);
									railt[select][j - 1] = railt[select][j];
									rails[select][j - 1].texture = rails[select][j].texture;
									SDL_RenderFillRect(renderer, &rails[select][j - 1].rect);
									SDL_RenderCopy(renderer, rails[select][j - 1].texture, NULL, &rails[select][j - 1].rect);
								}
								int o = rand() % 6;

								rails[select][4].texture = object[o].texture;
								railt[select][4] = rconnect(o);
								SDL_RenderFillRect(renderer, &rails[select][4].rect);
								SDL_RenderCopy(renderer, rails[select][4].texture, NULL, &rails[select][4].rect);
								SDL_RenderPresent(renderer);

								break;
							}
						}


						// click the map
						for(int i = 0; i < 25; i++) {
							if((x >= orect[i].x && x <= orect[i].x + orect[i].w && y >= orect[i].y && y <= orect[i].y + orect[i].h)) {
								cout << "Gamet: " << gamet << " Got " << i / 5 << " " << i % 5 << endl;

								if(gamet == 0 && book[i / 5][i % 5] == 0) {
									cout << "Put" << endl;
									book[i / 5][i % 5] = 1;
									maps[i / 5][i % 5].texture = rails[select][0].texture;
									mapt[i / 5][i % 5] = railt[select][0];
									SDL_RenderCopy(renderer, maps[i / 5][i % 5].texture, NULL, &maps[i / 5][i % 5].rect);

									for(int j = 1; j < 5; j++) {
										SDL_SetRenderDrawColor(renderer, 250, 250, 220, 210);
										railt[select][j - 1] = railt[select][j];
										rails[select][j - 1].texture = rails[select][j].texture;
										SDL_RenderFillRect(renderer, &rails[select][j - 1].rect);
										SDL_RenderCopy(renderer, rails[select][j - 1].texture, NULL, &rails[select][j - 1].rect);
									}
									int o = rand() % 6;

									rails[select][4].texture = object[o].texture;
									railt[select][4] = rconnect(o);
									SDL_RenderFillRect(renderer, &rails[select][4].rect);
									SDL_RenderCopy(renderer, rails[select][4].texture, NULL, &rails[select][4].rect);
								}
								else if(gamet == 1 && book[i / 5][i % 5] == 1) {
									cout << "Crashed" << endl;
									book[i / 5][i % 5] = 0;
									maps[i / 5][i % 5].texture = object[8].texture;
									mapt[i / 5][i % 5] = 0;
									gamet = 0;

									select ^= 1;
									SDL_RenderCopy(renderer, maps[i / 5][i % 5].texture, NULL, &maps[i / 5][i % 5].rect);
								}

								SDL_RenderPresent(renderer);

								for(int i = 0; i < 5; i++) {
									for(int j = 0; j < 5; j++) {
										cout << mapt[j][i] << " ";
									}
									cout << endl;
								}


								int walked[5][5];
								for(int i = 0; i < 5; i++)
									for(int j = 0; j < 5; j++)
										walked[i][j] = 0;
								walked[s1x][s1y] = 1;
								walked[s2x][s2y] = 1;

								int tracex = s1x - 1, tracey = s1y;
								cout << s1x << " " << s1y << endl;

								// check finished ?
								while(tracex != 2 && tracey != 2){

									for(int i = 0; i < 5; i++) {
										for(int j = 0; j < 5; j++){
											cout << walked[j][i] << " ";
										}
										cout << endl;

									}


									cout << "At the :" << tracex << " " << tracey << endl;
									cout << "The type is :" << mapt[tracex][tracey] << endl;
									cout << "The book is :" << walked[tracex][tracey] << endl;

									bool go = false;
									walked[tracex][tracey] = 1;

									int which = mapt[tracex][tracey];
									if(which == 0) break;
									switch(which) {
									case 3:
										if(tracey + 1 < 5 && walked[tracex][tracey + 1] == 0) {
											walked[tracex][tracey + 1] = 8;
											go = true;
											tracex++;
										}
										else if(tracey - 1 >= 0 && walked[tracex][tracey - 1] == 0) {
											go = true;
											tracex--;
										}
										break;
									case 5:
										if(tracey - 1 >= 0 && walked[tracex][tracey - 1] == 0) {
											go = true;
											tracey--;
										}
										else if(tracex - 1 >= 0 && walked[tracex - 1][tracey] == 0) {
											go = true;
											tracex--;
										}
									case 6:
										if(tracey - 1 >= 0 && walked[tracex][tracey - 1] == 0) {
											go = true;
											tracey--;
										}
										else if(tracex + 1 < 5 && walked[tracex + 1][tracey] == 0) {
											go = true;
											tracex++;
										}
										break;
									case 9:
										if(tracey - 1 >= 0 && walked[tracex][tracey - 1] == 0) {
											go = true;
											tracey--;
										}
										else if(tracex + 1 < 5 && walked[tracex + 1][tracey] == 0) {
											go = true;
											tracex++;
										}
										break;
									case 10:
										if(tracex + 1 < 5 && walked[tracex + 1][tracey] == 0) {
											go = true;
											tracex++;
										}
										else if(tracey + 1 < 5 && walked[tracex][tracey + 1] == 0) {
											go = true;
											tracex++;
										}
										break;

									}

									if(tracex == 2 && tracey == 2) {
										SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing file", "Game over.\n Good Game.", NULL);
									}


									if(!go) break;
								}
								///////////////////
							}
						}

						select ^= 1;
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

