#include <cstdio>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define SCREEN_WIDTH  1000
#define SCREEN_HEIGHT 720
#define SCREEN_FPS  32


SDL_Window * window = NULL;
SDL_Surface * screen = NULL;
SDL_Surface * image = NULL;
SDL_Renderer * renderer = NULL;
SDL_Texture * texture = NULL;
SDL_Event event;


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

int play_option() {
	int type = 0;

	return type;
}

int play_staff() {
	int type = 0;
	// someone write here
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

