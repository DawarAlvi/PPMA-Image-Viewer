#include <SDL2/SDL.h>
#include <fstream>
#include <string>

void RenderImage(SDL_Renderer* renderer, char* filename) {
	std::fstream file(filename, std::fstream::in);

	if (file.is_open()) {
		char ch = 0;
		std::string token = "";

		int imageWidth = 0;
		int imageHeight = 0;
		int maxValue = 0;

		bool commentActive = false;
		size_t tokenCount = 0;
		size_t valueCount = 0;

		int i = 0, j = 0;
		int r = 0, g = 0, b = 0;

		while (file >> std::noskipws >> ch) {
			if (commentActive) {
				if (ch == '\n') {
					commentActive = false;
					token = "";
				}
				else
				{
					token += ch;
				}
			}
			else {
				if (ch == '#') {
					token += ch;
					commentActive = true;
				}
				else if (ch == '\n' || ch == ' ') {
					if (token != "") {
						++tokenCount;
						switch (tokenCount) {
						case 1:
							break;
						case 2:
							imageWidth = stoi(token);
							break;
						case 3:
							imageHeight = stoi(token);
							break;
						case 4:
							maxValue = stoi(token);
							break;
						default:
						{
							++valueCount;

							if (valueCount % 3 == 0) {
								b = (stoi(token)) / (float)(maxValue) * 255;

								SDL_SetRenderDrawColor(renderer, r, g, b, 255);
								SDL_RenderDrawPoint(renderer, i, j);

								i = ((i + 1) % imageWidth);
								if (i == 0) j++;
								//reset rgb
							}
							else if (valueCount % 3 == 1) {
								r = (stoi(token)) / (float)(maxValue) * 255;
							}
							else {
								g = (stoi(token)) / (float)(maxValue) * 255;
							}
						}
						}
						token = "";
					}
				}
				else
				{
					token += ch;
				}
			}
		}
		file.close();
	}
}
void RenderClear(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}
int main(int argc, char* argv[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	char* imagePath = "";

	if (SDL_Init(SDL_INIT_VIDEO) < 0) { SDL_Log("SDL could not be initialized!"); }
	else {
		window = SDL_CreateWindow("PPMA Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
		if (!window) { SDL_Log("SDL window could not be created!"); }
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			SDL_Event e;
			bool quit = false;

			RenderClear(renderer);
			if (argc > 1) {
				imagePath = argv[1];
				RenderImage(renderer, imagePath);
			}

			while (quit == false) {
				while (SDL_PollEvent(&e)) {
					if (e.type == SDL_QUIT) quit = true;
					if (e.type == SDL_DROPFILE) {
						RenderClear(renderer);
						imagePath = e.drop.file;
						RenderImage(renderer, imagePath);
					}
					if (e.type == SDL_WINDOWEVENT)
					{
						if (e.window.event == SDL_WINDOWEVENT_RESIZED && imagePath != "") {
							RenderImage(renderer, imagePath);
						}
					}
				}
				SDL_RenderPresent(renderer);
			}
			SDL_DestroyWindow(window);
		}
	}
	SDL_Quit();
	return 0;
}