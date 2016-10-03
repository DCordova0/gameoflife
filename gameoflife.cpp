#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 680;

void init();
void loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface *gLifeCell = NULL, *gDeadCell = NULL;
SDL_Surface *gLifeCell3 = NULL, *gDeadCell3 = NULL;
SDL_Surface *gTitle = NULL, *gPauseTitle;

void init() 
{
	bool succes = true;
	if (SDL_Init( SDL_INIT_VIDEO ) < 0){
		cout << "SDL could not initialize! SDL_Error: " ;
		cout << SDL_GetError() << endl;
		succes = false;
	}
	else{
		gWindow = SDL_CreateWindow( "Game Of Life", 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, 
			SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			cout << "Window could not be created! SDL_Error: "; 
			cout << SDL_GetError() << endl;
			succes = false;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}
	if (succes == false) {
		cout << "Failed to initialize!\n";
		exit(EXIT_FAILURE);
	}
}

void loadMedia()
{
	bool succes = true;

	gLifeCell = SDL_LoadBMP ("Images/livecell2.bmp");
	gDeadCell = SDL_LoadBMP ("Images/deadcell2.bmp");
	gLifeCell3 = SDL_LoadBMP ("Images/livecell3.bmp");
	gDeadCell3 = SDL_LoadBMP ("Images/deadcell3.bmp");
	gTitle = SDL_LoadBMP ("Images/title.bmp");
	gPauseTitle = SDL_LoadBMP ("Images/titlepaused.bmp");

	if ( gLifeCell == NULL ) {
		cout << "Unable to load image Images/livecell2.bmp! SDL_Error: "; 
		cout << SDL_GetError() << endl;
		succes = false;
	}
	if ( gDeadCell == NULL ) {
		cout << "Unable to load image Images/deadcell2.bmp! SDL_Error: ";
		cout << SDL_GetError() << endl;	
		succes = false;
	}
	if ( gLifeCell3 == NULL ) {
		cout << "Unable to load image Images/livecell3.bmp! SDL_Error: "; 
		cout << SDL_GetError() << endl;
		succes = false;
	}
	if ( gDeadCell3 == NULL ) {
		cout << "Unable to load image Images/deadcell3.bmp! SDL_Error: ";
		cout << SDL_GetError() << endl;	
		succes = false;
	}
	if ( gTitle == NULL ) {
		cout << "Unable to load image Images/title.bmp! SDL_Error: ";
		cout << SDL_GetError() << endl;	
		succes = false;
	}
	if ( gPauseTitle == NULL ) {
		cout << "Unable to load image Images/titlepaused.bmp! SDL_Error: ";
		cout << SDL_GetError() << endl;	
		succes = false;
	}
	if (succes == false) {
		cout << "Failed to load media!\n";
		exit(EXIT_FAILURE);
	}
}

void close()
{
	SDL_FreeSurface( gLifeCell );
	gLifeCell = NULL;
	SDL_FreeSurface( gDeadCell );
	gDeadCell = NULL;
	SDL_FreeSurface( gLifeCell3 );
	gLifeCell3 = NULL;
	SDL_FreeSurface( gDeadCell3 );
	gDeadCell3 = NULL;

	SDL_FreeSurface( gTitle );
	gTitle = NULL;
	SDL_FreeSurface( gPauseTitle );
	gPauseTitle = NULL;
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	SDL_Quit();
}

int main( int argc, char* argv[] )
{
	ifstream inFile;
	string line;
	vector<vector<bool> > matrix1;
	vector<vector<bool> > matrix2;
	int mwidth = 120;
	int mhight = 68;
	int mx,my,option; //Matrix x and 


	matrix1 = vector<vector<bool> >(mwidth,vector<bool>(mhight,false));
	matrix2 = vector<vector<bool> >(mwidth,vector<bool>(mhight,false));
	cout << "Welcome!!" << endl << endl;
	cout << "Maps: " << endl;
	cout << "1) Empty" << endl;
	cout << "2) Travellers" << endl;
	cout << "3) Love" << endl;
	cout << "4) Map1" << endl << endl;
	cout << "Choose an option: ";
	cin >> option;

	switch (option) {
	case 1:
		line = "Maps/empty.txt";
		break;
	case 2:
		line = "Maps/travellers.txt";
		break;
	case 3:
		line = "Maps/love.txt";
		break;
	case 4:
		line = "Maps/map1.txt";
		break;
	}


	inFile.open(line.c_str());
	if (inFile.fail()) {
		cout << "Error opening file\n";
		exit(EXIT_FAILURE);
	}
	int j = 0;
	while (!inFile.eof()) {
		inFile >> line;
		for (int i=0;i<mwidth;i++) {
			if (line[i] == 'X') {
				matrix1[i][j] = true;
				matrix2[i][j] = true;
			} else {
				matrix1[i][j] = false;
				matrix2[i][j] = false;
			}
		} 
		j++;
	}

	init();
	loadMedia();


	//Bliting Title
	SDL_Rect destination;

	SDL_Event evnt;
	bool quit = false;
	bool pause = true;
	bool pollo = false;
	bool ephilepsya = false;

	while ( !quit ){ 
		//MAIN LOOP

		destination.x =0;
		destination.y =0;
		destination.w =1200;
		destination.h =60;
		if (pause) 
			SDL_BlitSurface( gPauseTitle, NULL, gScreenSurface, &destination);
		else
			SDL_BlitSurface( gTitle, NULL, gScreenSurface, &destination);

		while ( SDL_PollEvent(&evnt) != 0 ){
			switch (evnt.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (evnt.motion.x >= 0 && evnt.motion.x <SCREEN_WIDTH
					&& evnt.motion.y >= 60 && evnt.motion.y <SCREEN_HEIGHT)
					mx = evnt.motion.x/10;
					my = (evnt.motion.y-60)/10;
					matrix2[mx][my] = !matrix2[mx][my]; // Change status
				break;
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym){
				case SDLK_p:
					pause = !pause;
					break;
				case SDLK_e:
					ephilepsya = !ephilepsya;
					break;
				}
				break;
			}
		}

		//Bliting cells to Screen
		destination.w =10;
		destination.h =10;
		for (int i=0;i<mwidth;i++) {
			for (int j=0;j<mhight;j++) {
				matrix1[i][j] = matrix2[i][j];
				destination.x = i*10;
				destination.y = j*10+60;
				if (matrix1[i][j]) {
					if (ephilepsya) {
						if (pollo && ephilepsya) {
							SDL_BlitSurface( gLifeCell, NULL, 
								gScreenSurface, &destination);
						} else {
							SDL_BlitSurface( gLifeCell3, NULL, 
								gScreenSurface, &destination);
						}
					} else {
						SDL_BlitSurface( gLifeCell, NULL, 
								gScreenSurface, &destination);
					}

				} else {
					SDL_BlitSurface( gDeadCell, NULL, 
						gScreenSurface, &destination);
				}
			}
		}
		SDL_UpdateWindowSurface( gWindow );
		
		if (!pause) {
		//Calculating the status of the cells
			for (int i=0;i<mwidth;i++) {
				for (int j=0;j<mhight;j++) {
					int cellscont = 0;

					//Count adyacent live cells
					for (int k=i-1;k<=i+1;k++) {
						for (int l=j-1;l<=j+1;l++){
							if ((i!=k || j!=l) && k>=0 && k<mwidth && l>=0 
								&& l<mhight) {
								if (matrix1[k][l])
									cellscont++;
							}
						}
					}
					if (matrix1[i][j]) {
						if (cellscont < 2)
							matrix2[i][j] = false;
						else if (cellscont > 3)
							matrix2[i][j] = false;
					} else {
						if (cellscont == 3)
							matrix2[i][j] = true;
					}
				}
			}
			pollo = !pollo;
		}
		
		for (int i=0; i<50000000;i++)
			i;
	} //END MAIN LOOP
	close();
	return 0;
}