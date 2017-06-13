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
Mix_Chunk *op = NULL;
Mix_Chunk *button = NULL;
Mix_Chunk *end = NULL;
int type = 0; // 0 for menu, 1 for game play, 2 for option, 3 for staff
bool played = false;

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
	if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) return false;
	window = SDL_CreateWindow("Wahaha~ EZ metro~", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(window = NULL) return false;
	else screen = SDL_GetWindowSurface(window);

	return true;
}

int play_menu() {
	if(played == false) {
		op = Mix_LoadWAV("A_hisa - BloominLights (Callionet - Cherry snow).wav");
		button = Mix_LoadWAV("Button.wav");
		end = Mix_LoadWAV("end_sp.wav");
		Mix_PlayChannel(-1, op, 0);
		Mix_Volume(0, MIX_MAX_VOLUME/2);
		played = true;
	}

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

		image = IMG_Load(file);
		texture = SDL_CreateTextureFromSurface(renderer, image);
		for(int l = 0; l < 256; l += 10) {
			SDL_SetTextureAlphaMod(texture, l);
			SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
			SDL_RenderPresent(renderer);

			SDL_Delay(1000 / SCREEN_FPS);
		}
		//SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
		//SDL_RenderPresent(renderer);

	}

	// make the into play rect smaller
	rect[0] = { 480, 450, 360,  40};

	while(event.type != SDL_QUIT && type ==  0) {
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) break;
			else if(event.type == SDL_MOUSEBUTTONDOWN) {
				if(event.button.button == SDL_BUTTON_LEFT) {
					Mix_PlayChannel(-1, button, 0);

					int x = event.button.x;
					int y = event.button.y;

					// 0 for play 1 for volumn 2 for staff
					for(int i = 0; i < menu_case; i++)
						if(x >= rect[i].x && x <= rect[i].x + rect[i].w && y >= rect[i].y && y <= rect[i].y + rect[i].h) {
							type = i + 1;
							cout << "Menu got " << i << endl;
						}
				}
			}
		}
		SDL_Delay(1000 / SCREEN_FPS);
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
			for(int l = 0; l < 256; l += 10) {
				SDL_SetTextureAlphaMod(texture, l);
				SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
				SDL_RenderPresent(renderer);

				SDL_Delay(1000 / SCREEN_FPS);
			}
			//SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
			//SDL_RenderPresent(renderer);

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
		int inix = 390, iniy = 110;

		for(int i = 0; i < MAP_SIZE; i++) {
			for(int j = 0; j < MAP_SIZE; j++) {
				SDL_Rect paste = object[8].rect;

				maps[i][j] = object[8];


				paste.x = inix + i * paste.h;
				paste.y = iniy + j * paste.w;

				maps[i][j].rect = paste;

				orect.push_back(paste);

				//SDL_RenderCopy(renderer, maps[i][j].texture, NULL, &paste);
				//SDL_RenderPresent(renderer);
			}
		}

			// garbage can
			SDL_Rect paste = object[7].rect;

			paste.x = 11;
			paste.y = 384;

			orect.push_back(paste);

			for(int l = 0; l < 256; l += 60) {
				SDL_SetTextureAlphaMod(object[7].texture, l);
				SDL_RenderCopy(renderer, object[7].texture, NULL, &paste);
				SDL_RenderPresent(renderer);

				SDL_Delay(1000 / SCREEN_FPS);
			}
			//SDL_RenderCopy(renderer, object[7].texture, NULL, &paste);
			//SDL_RenderPresent(renderer);


			paste.x = 1168;
			paste.y = 720 - object[7].rect.h - 236;

			orect.push_back(paste);
			for(int l = 0; l < 256; l += 30) {
				SDL_SetTextureAlphaMod(object[7].texture, l);
				SDL_RenderCopy(renderer, object[7].texture, NULL, &paste);
				SDL_RenderPresent(renderer);

				SDL_Delay(1000 / SCREEN_FPS);
			}

			//SDL_RenderCopy(renderer, object[7].texture, NULL, &paste);
			//SDL_RenderPresent(renderer);


			// bomb
			paste = object[6].rect;
			paste.x = 11;
			paste.y = 239;

			orect.push_back(paste);
			SDL_RenderCopy(renderer, object[6].texture, NULL, &paste);
			//SDL_RenderPresent(renderer);

			paste.x = 1167;
			paste.y = 720 - object[6].rect.h - 381;

			cout << orect.size() << endl;
			orect.push_back(paste);
			for(int l = 0; l < 256; l += 60) {
				SDL_SetTextureAlphaMod(object[6].texture, l);
				SDL_RenderCopy(renderer, object[6].texture, NULL, &paste);
				SDL_RenderPresent(renderer);

				SDL_Delay(1000 / SCREEN_FPS);
			}

			//SDL_RenderCopy(renderer, object[6].texture, NULL, &paste);
			//SDL_RenderPresent(renderer);

			objects rails[2][5];

			// random rail
			// state is for checking the rail is connected or not
			// 0 for nothing , 1 for up is conect, 2 for down is connected, 4 for left is connected, 8 for right is connected
			int railt[2][5];

			int place1x[5] = {65, 140, 162, 140, 65};
			int place2x[5] = {1114, 1043, 1018, 1043, 1119};
			int placey[5] = {42, 176, 310, 444, 578};

			srand(NULL);
			for(int i = 0; i < 5; i++) {
				int o = rand() % 6;

				paste = object[o].rect;
				paste.x = place1x[i];
				paste.y = placey[i];

				railt[0][i] = rconnect(o);
				rails[0][i] = object[o];
				rails[0][i].rect = paste;

				for(int l = 0; l < 256; l += 60) {
					SDL_SetTextureAlphaMod(rails[0][i].texture, l);
					SDL_RenderCopy(renderer, rails[0][i].texture, NULL, &paste);
					SDL_RenderPresent(renderer);

					SDL_Delay(1000 / SCREEN_FPS);
				}
				//SDL_RenderCopy(renderer, rails[0][i].texture, NULL, &paste);
				//SDL_RenderPresent(renderer);

				o = rand() % 6;

				paste = object[o].rect;
				paste.x = place2x[i];
				paste.y = placey[i];

				railt[1][i] = rconnect(o);
				rails[1][i] = object[o];
				rails[1][i].rect = paste;

				for(int l = 0; l < 256; l += 60) {
					SDL_SetTextureAlphaMod(rails[1][i].texture, l);
					SDL_RenderCopy(renderer, rails[1][i].texture, NULL, &paste);
					SDL_RenderPresent(renderer);

					SDL_Delay(1000 / SCREEN_FPS);
				}
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

			for(int l = 0; l < 256; l += 60) {
				SDL_SetTextureAlphaMod(object[10].texture, l);
				SDL_RenderCopy(renderer, object[10].texture, NULL, &paste);
				SDL_RenderPresent(renderer);

				SDL_Delay(1000 / SCREEN_FPS);
			}
			//SDL_RenderCopy(renderer, object[10].texture, NULL, &paste);

			// e
			paste = object[11].rect;

			paste.x = inix + 2 * paste.h;
			paste.y = iniy + 2 * paste.w;

			book[2][2] = -2;
			mapt[2][2] = 1 & 2 & 4 & 8;

			orect.push_back(paste);
			for(int l = 0; l < 256; l += 60) {
				SDL_SetTextureAlphaMod(object[11].texture, l);
				SDL_RenderCopy(renderer, object[11].texture, NULL, &paste);
				SDL_RenderPresent(renderer);

				SDL_Delay(1000 / SCREEN_FPS);
			}
			//SDL_RenderCopy(renderer, object[11].texture, NULL, &paste);
			//SDL_RenderPresent(renderer);



		//orect: 0 ~ 24 map, 25 26 garbage can, 27 28 bomb, 29 start1, 30 start2, 31 end;
		//rails: 0 ~ 4 up, 5 ~ 8 down;

		int select = 0;

		// gamet: 0 for normal, 1 for bomb, 2 for garbage can
		int gamet = 0;

		while(event.type != SDL_QUIT) {
			if(SDL_PollEvent(&event)) {
				if(event.type == SDL_QUIT) break;
				else if(event.type == SDL_MOUSEBUTTONDOWN) {
					if(event.button.button == SDL_BUTTON_LEFT) {
						Mix_PlayChannel(-1, button, 0);

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
									if(select == 0 && j == 1) SDL_SetRenderDrawColor(renderer, 228, 192, 199, 0);
									else if(select == 0 && j != 1) SDL_SetRenderDrawColor(renderer, 221, 176, 185, 0);
									else if(select == 1 && j == 1) SDL_SetRenderDrawColor(renderer, 0xD3, 0xCC, 0xE5, 0);
									else if(select == 1 && j != 1) SDL_SetRenderDrawColor(renderer, 199, 190, 221, 0);
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

										if(select == 0 && j == 1) SDL_SetRenderDrawColor(renderer, 228, 192, 199, 0);
										else if(select == 0 && j != 1) SDL_SetRenderDrawColor(renderer, 221, 176, 185, 0);
										else if(select == 1 && j == 1) SDL_SetRenderDrawColor(renderer, 0xD3, 0xCC, 0xE5, 0);
										else if(select == 1 && j != 1) SDL_SetRenderDrawColor(renderer, 199, 190, 221, 0);

										railt[select][j - 1] = railt[select][j];
										rails[select][j - 1].texture = rails[select][j].texture;

										 SDL_RenderFillRect(renderer, &rails[select][j - 1].rect);
										//else
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
									SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
									SDL_RenderFillRect(renderer, &maps[i / 5][i % 5].rect);

									if(i / 5 >= 1 && i / 5 <= 3 && i % 5 >= 1 && i % 5 <= 3) SDL_SetRenderDrawColor(renderer, 0xD6, 0xEB, 0xD7, 0);
									else  SDL_SetRenderDrawColor(renderer, 0xEB, 0xF5, 0xEB, 0);

									SDL_Rect temp_rect = {maps[i / 5][i % 5].rect.x + 2, maps[i / 5][i % 5].rect.y + 2, maps[i / 5][i % 5].rect.w - 4, maps[i / 5][i % 5].rect.h - 4 };

									SDL_RenderFillRect(renderer, &temp_rect);
									//SDL_RenderCopy(renderer, maps[i / 5][i % 5].texture, NULL, &maps[i / 5][i % 5].rect);
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
										break;
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
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing file", "I like to PEROPERO.\n Wahahahahaha ~", NULL);
	return type;
}

int play_staff() {
	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "The grant staff", "Thank you everybody.", NULL);

	freopen("staff.in", "r", stdin);

	int staff_case = 0;

	scanf("%d", &staff_case);

	SDL_Rect rect[staff_case];
	SDL_Texture *staff;
	for(int i = 0; i < staff_case; i++) {
		char file[100];
		int w, h, x, y;
		scanf("%s%d%d%d%d", &file, &w, &h, &x, &y);

		rect[i] = {x, y, w, h};

		image = IMG_Load(file);
		staff = SDL_CreateTextureFromSurface(renderer, image);

		//SDL_RenderCopy(renderer, staff, NULL, &rect[i]);
		SDL_RenderPresent(renderer);

	}

	for(int i = 0; i < 256; i += 5) {
		SDL_SetTextureAlphaMod(staff, i);
		SDL_RenderCopy(renderer, staff, NULL, &rect[0]);
		SDL_RenderPresent(renderer);

		SDL_Delay(1000 / SCREEN_FPS);
	}

	while(event.type != SDL_QUIT) {
			if(SDL_PollEvent(&event)) {
				if(event.type == SDL_QUIT) break;
				else if(event.type == SDL_MOUSEBUTTONDOWN) {
					if(event.button.button == SDL_BUTTON_LEFT) {
						Mix_PlayChannel(-1, button, 0);

						break;
					}
				}
			}
			SDL_Delay(1000 / SCREEN_FPS);
		}

	cout << "Type is 0" << endl;

	return 0;
}

void clean_up() {
	Mix_FreeChunk(button);
	Mix_FreeChunk(op);
	Mix_FreeChunk(end);


	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char * args[]) {
	if(!init()) return 0;


	bool quit = false;


	while(!quit) {
		SDL_PollEvent(&event);

		if(event.type == SDL_QUIT) break;

		switch(type) {
		// 0 for play 1 for volumn 2 for staff
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

	Mix_PlayChannel(-1, end, 0);
	SDL_Delay(3437);

	clean_up();
	return 0;
}
