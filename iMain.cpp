# include "iGraphics.h"
# include <windows.h>
# include <mmsystem.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# pragma comment(lib, "Winmm.lib")

#define SCREEN_WIDTH 630
#define SCREEN_HEIGHT 800
#define START_LINE_HEIGHT 20
#define PLAYER_INIT_POS_X 485
#define OPP_INIT_POS_X 113

int GameMode = 0;

/* 
	GameMode 0: Main Menu
	GameMode 1: Level 01
	GameMode 2: Level 02
	GameMode 3: Level 03
	GameMode 4: Take Name
	GameMode 5: Result
	GameMode 6: Credits
	GameMode 7: Instructions
	GameMode 8: Score Option
	GameMode 9: Scores
*/

bool PlayMode = false;
int Timer1, Timer2, Timer3, Timer4, Timer5, Timer6;

float Player_X  = PLAYER_INIT_POS_X, Player_Y = START_LINE_HEIGHT + 5;
float Opp_X = OPP_INIT_POS_X, Opp_Y = START_LINE_HEIGHT + 5;

bool PlayerMotionUp = false;

float Init_Vel = 3; 
float Acc = 0;

bool Thrust = false;

int RoadSide_Y[20] = {0};
int Road_Y[25] = {0};  
float RoadSpeed = 5;

bool LevelWin = false;

int PlayerCarLen = 86, OppCarLen = 81;

char ShowHighScoresText[30] = "", ShowLevelText[30] = "";
char ShowHighScoresValue[30] = "", ShowLevelValue[30] = "";

bool MusicOn = true;

char Musics[15][30] = {"MenuMusic.wav", "RaceMusic.wav", "RaceMusic.wav", "RaceMusic.wav", "MenuMusic.wav", "LevelResult.wav", "MenuMusic.wav", "MenuMusic.wav", "MenuMusic.wav", "MenuMusic.wav", "MenuMusic.wav"};

char BarrierCars[4][30] = {"police01_resized.bmp", "blue_resized.bmp", "green_resized.bmp", "white_resized.bmp"};
float BarrCarX[4] = {200, 300, 400, 500};
float BarrCarY[4] = {1000, 1500, 2000, 1800};

float GameScore = 0;

int LevelsPlayed = 0;

bool NameGiven = false, TextBoxActive = false;

char TempUserInfo[35] = "";
char UserNames[10][35] = {}, UserScores[10][15] = {};
int UserIndex = 0;

struct NameAndScore{
	char Name[35];
	float Score;
};

struct NameAndScore HighScores[11];
struct NameAndScore LatestScores[11];

bool NewScoreTaken = false;
int LateOrHighScore = 0;

bool Crash = false, CrashMusic = false;
int CrashRender = 0;

float CollisionPosX, CollisionPosY;

char ExplosionPics[10][20] = {"frame01.bmp", "frame02.bmp","frame03.bmp", "frame04.bmp", "frame05.bmp", "frame06.bmp"};

float mod(float n){
	if(n>0) return n;
	else return -1*n;
}
 
void DrawMenu();
void MovePlayerUp();
void SetRoads();
void DrawTrack();
void DrawCars();
void ChangeRoads();
void MoveOppUp();
void PlayMusic();
void DrawBarrierCars();
void MoveBarrierCars();
void CollisionCheck();
void ResetRace();
void ShowHighScores();
void SortScores();
void FixLatestScores();
void ShowLatestScores();
void PlayCrashMusic();
void ChangeToResult();

void iDraw() {
	
	iClear();
	
	if(GameMode == 0){
		DrawMenu();
	}

	else if(GameMode >= 1 && GameMode <= 3){
		
		DrawTrack();	

		iShowBMP(75, 0, "startline.bmp");
		iShowBMP(75, 780, "startline.bmp");

		DrawCars();
		DrawBarrierCars();

		if(PlayMode == false && Crash == false){
			iShowBMP2(0, 725, "back_button.bmp", 0);
			iShowBMP2(190, 645, "playpause.bmp", 0);
		}

		if(Crash){
			iShowBMP2(CollisionPosX-50, CollisionPosY-50, ExplosionPics[CrashRender/6], 0);
			CrashRender++;
			if(CrashRender >= 85){
				CrashRender = 0;
				ChangeToResult();
			}
		}
	}

	else if(GameMode == 4){

		iShowBMP2(0, 0, "bg05.bmp", 0);
		iShowBMP2(0, 725, "back_button.bmp", 0);
		
		if(NameGiven){
			iShowBMP2(555, 0, "next_button.bmp", 0);
		}

		iSetColor(255, 255, 255);

		iText(175, 575, "Name must not contain any spaces", GLUT_BITMAP_HELVETICA_18);
		iText(225, 610, "Enter Your Name", GLUT_BITMAP_TIMES_ROMAN_24);

		if(TextBoxActive || NameGiven){
			iSetColor(250, 250, 250);	
		}
		else{
			iSetColor(200, 200, 200);
		}

		iFilledRectangle(150, 510, 330, 50);

		if(TextBoxActive || NameGiven){
			iSetColor(0, 0, 0);
			iText(165, 526, TempUserInfo, GLUT_BITMAP_HELVETICA_18);
		}

		iSetColor(255, 255, 255);
		iText(195, 480, "Click on the box to activate it", GLUT_BITMAP_HELVETICA_18);
		iText(225, 445, "Press enter to finish", GLUT_BITMAP_HELVETICA_18);
	}

	else if(GameMode == 5){

		iShowBMP(0, 0, "bg05.bmp");
		iSetColor(255, 255, 255);

		sprintf(ShowHighScoresValue, "Score: %.0f", GameScore);
		iText(255, 470, ShowHighScoresValue, GLUT_BITMAP_TIMES_ROMAN_24);

		if(LevelWin){
			
			if(LevelsPlayed == 0 || LevelsPlayed == 1){
				iShowBMP2(555, 0, "next_button.bmp", 0);
				iText(230, 520, "Level Completed!", GLUT_BITMAP_TIMES_ROMAN_24);
			}
			else{
				iText(230, 520, "Game Completed!", GLUT_BITMAP_TIMES_ROMAN_24);
			}
		}
		
		else{
			iText(255, 520, "Game Over", GLUT_BITMAP_TIMES_ROMAN_24);
		}

		iShowBMP2(0, 725, "back_button.bmp", 0);

	}

	else if(GameMode == 6){
		iShowBMP2(0, 0, "bg07.bmp", 0);
		iShowBMP(100, 120, "creditss.bmp");
		iShowBMP2(0, 725, "back_button.bmp", 0);
	}

	else if(GameMode == 7){

		iShowBMP2(0, 0, "bg07.bmp", 0);
		iShowBMP2(0, 725, "back_button.bmp", 0);
		iSetColor(255, 0, 0);
		iShowBMP2(100, 120, "textbg.bmp", 0);

		iSetColor(255, 255, 255);

		iText(220, 620, "INSTRUCTIONS", GLUT_BITMAP_TIMES_ROMAN_24);
		iLine(220, 615, 397, 615);
		iLine(220, 610, 397, 610);

		iShowBMP(100, 120, "instructionss.bmp");

		/* iText(135, 570, "Press F1 to pause or resume music", GLUT_BITMAP_HELVETICA_18);
		iText(135, 540, "Use arrow keys to control car movement", GLUT_BITMAP_HELVETICA_18);
		iText(135, 510, "Press 'R' to start or resume the race", GLUT_BITMAP_HELVETICA_18);
		iText(135, 480, "Press 'P' to pause the race", GLUT_BITMAP_HELVETICA_18);
		iText(135, 450, "Collision with other cars terminates the race", GLUT_BITMAP_HELVETICA_18);
		iText(135, 420, "You can't enter the opponent's lane", GLUT_BITMAP_HELVETICA_18);
		iText(135, 390, "Reach the finish line before the opponent", GLUT_BITMAP_HELVETICA_18); */
	}

	else if(GameMode == 8){

		iShowBMP(0, 0, "bg00.bmp");
		iShowBMP2(0, 725, "back_button.bmp", 0);

		iShowBMP2(223, 440, "Scoreboard.bmp", 0);
		iShowBMP2(223, 365, "Leaderboard.bmp", 0);

	}

	else if(GameMode == 9){
		
		iShowBMP2(0, 0, "bg08.bmp", 0);
		iShowBMP2(0, 725, "back_button.bmp", 0);
		iShowBMP2(100, 120, "textbg.bmp", 0);

		iSetColor(255, 255, 255);
	
		iText(140, 575, "Name", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(390, 575, "Score", GLUT_BITMAP_TIMES_ROMAN_24);
		iLine(140, 570, 200, 570);
		iLine(390, 570, 445, 570); 

		if(LateOrHighScore == 1){
			ShowHighScores();
		}

		else if(LateOrHighScore == 2){
			ShowLatestScores();
		}
	}
}

void iMouseMove(int mx, int my){
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		if(GameMode == 8){
			if(mx > 224 && mx < 405 && my > 366 && my < 424){
				GameMode = 9;
				LateOrHighScore = 1;
			}
			if(mx > 224 && mx < 405 && my > 441 && my < 499){
				GameMode = 9;
				LateOrHighScore = 2;
			}
		}	 

		if(GameMode >= 1 && GameMode <= 3 && PlayMode == false){
			if(mx > 0 && mx < 75 && my > 725 && my < 800){
				
				GameMode = 0;
				GameScore = 0;
				LevelsPlayed = 0;
				Init_Vel = 3;
				PlayerMotionUp = false;
				ResetRace();

				TempUserInfo[0] = 0;
				TextBoxActive = false;
				NameGiven = false;
				
				MusicOn = true;
				PlayMusic();
			}
		}

		if(GameMode == 4){
			if(mx >= 150 && mx <= (150+330) && my >= 510 && my <= (510+50)){
				TextBoxActive = true;
			}
		}
		
		if(GameMode == 0){
			
			if(mx >= 226 && mx <= (220+183) && my >= 553 && my <= (547+60)){
				GameMode = 4;
			}

			if(mx >= 226 && mx <= (220+183) && my >= 327 && my <= (323+60)){
				GameMode = 6;
			}

			if(mx >= 226 && mx <= (220+183) && my >= 477 && my <= (473+60)){
				GameMode = 7;
			}

			if(mx >= 226 && mx <= (220+183) && my >= 402 && my <= (398+60)){
				GameMode = 8;
			}

			if(mx >= 226 && mx <= (220+183) && my >= 253 && my <= (247+60)){
				exit(0);
			}

		}

		if(GameMode == 5){
			if(mx > 0 && mx < 75 && my > 725 && my < 800){

				strcpy(HighScores[10].Name, TempUserInfo);
				HighScores[10].Score = GameScore;

				strcpy(LatestScores[10].Name, TempUserInfo);
				LatestScores[10].Score = GameScore;

				NewScoreTaken = true;

				SortScores();
				FixLatestScores();

				TempUserInfo[0] = 0;

				GameMode = 0;
				GameScore = 0;
				LevelsPlayed = 0;
				Init_Vel = 3;
				NameGiven = false;
				PlayerMotionUp = false;
		
				ResetRace();
				PlayMusic();
			}

			if(LevelWin == true && LevelsPlayed < 2){
				if(mx > 555 && mx < 630 && my > 0 && my < 75){

					if(LevelsPlayed == 0){
						GameMode = 2;	
					}
					else if(LevelsPlayed == 1){
						GameMode = 3;
					}

					LevelsPlayed++;

					PlayerMotionUp = false;
					Init_Vel += 1;

					ResetRace();
					PlayMode = false;
					LevelWin = false;
					MusicOn = false;
					PlayMusic();
				}
			}
		}

		if(GameMode == 4){
			if(mx > 0 && mx < 75 && my > 725 && my < 800){
				GameMode = 0;
				TempUserInfo[0] = 0;
				NameGiven = false;
				TextBoxActive = false;
			}
			if(mx > 555 && mx < 630 && my > 0 && my < 75 && NameGiven){
				
				GameMode = 1;
				MusicOn = false;
				PlayMusic();
			}
		}

		if(GameMode >= 6 && GameMode <= 8){
			if(mx > 0 && mx < 75 && my > 725 && my < 800){
				GameMode = 0;
			}
		}

		if(GameMode == 9){
			if(mx > 0 && mx < 75 && my > 725 && my < 800){
				GameMode = 8;
			}
		}  
		
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		
	}
}

void iKeyboard(unsigned char key) {

	if(key == 'p' || key == 'P'){
		if(GameMode >= 1 && GameMode <= 3 && PlayMode){
			PlayMode = false;
			MusicOn = false;
			PlayMusic();
			iPauseTimer(Timer1);
			iPauseTimer(Timer2);
			iPauseTimer(Timer3);
			iPauseTimer(Timer4);
		}
	}

	if(key == 'r' || key == 'R'){
		if(GameMode >= 1 && GameMode <= 3 && PlayMode == false){
			PlayMode = true;
			MusicOn = true;
			PlayMusic();
			iResumeTimer(Timer1);
			iResumeTimer(Timer2);
			iResumeTimer(Timer3);
			iResumeTimer(Timer4);
		}
	}

	if(GameMode == 4 && TextBoxActive){
		 
		if(key == '\r'){
			TextBoxActive = false;
			NameGiven = true;
		}

		else if(key == '\b' && NameGiven == false){
			TempUserInfo[strlen(TempUserInfo)-1] = 0;
		} 

		else if(strlen(TempUserInfo) < 30 && NameGiven == false && key != ' '){
			sprintf(TempUserInfo, "%s%c", TempUserInfo, key);
		}
	}
}


void iSpecialKeyboard(unsigned char key) {

	/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/

	if (key == GLUT_KEY_END) {
		exit(0);
	}
	
	if(key == GLUT_KEY_UP){
		if(GameMode >= 1 && GameMode <= 3 && PlayMode){
			PlayerMotionUp = true;
			Thrust = true;
			GameScore += 20;
			iResumeTimer(Timer1);
		}
	}
	
	if(key == GLUT_KEY_DOWN && PlayMode){
		if(GameMode >= 1 && GameMode <= 3){
			if(Player_Y > 30){
				Player_Y = Player_Y - 3.5 - 0.5*GameMode;
;
				if(GameScore > 25){
					GameScore -= 25;
				}
			}
		}
	}

	if(key == GLUT_KEY_LEFT && PlayMode){
		if(GameMode >= 1 && GameMode <= 3 && Player_X >= 190){
			Player_X = Player_X - 4.5 - 0.5*GameMode;
		}
	}

	if(key == GLUT_KEY_RIGHT && PlayMode){
		if(GameMode >= 1 && GameMode <= 3 && Player_X <= 510){
			Player_X = Player_X + 4.5 + 0.5*GameMode;
		}
	}

	if(key == GLUT_KEY_F1){

		if(MusicOn){
			MusicOn = false;
		}
		else{
			MusicOn = true;
		}

		PlayMusic();
	}
}


int main() {

	SortScores();
	FixLatestScores();

	SetRoads();

	Timer1 = iSetTimer(35, MovePlayerUp);
	Timer2 = iSetTimer(25, ChangeRoads);
	Timer3 = iSetTimer(35, MoveOppUp);
	Timer4 = iSetTimer(35, MoveBarrierCars);

	PlayMusic();

	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Speed Blaze: Infinite Rush");

	return 0;
}

// Functions

void MovePlayerUp(){

	if(GameMode >= 1 && GameMode <= 3){

		if(Init_Vel <= Acc || Player_Y+Init_Vel > (800-PlayerCarLen)){
			PlayerMotionUp = false;
			Acc = 0;
			iPauseTimer(Timer1);
		}
		
		if(PlayerMotionUp){
			
			Player_Y = Player_Y + Init_Vel;

			if(!Thrust){
				Player_Y -= Acc;
				Acc += 0.35;
			}
			
			Thrust = false;
		}

	}
}
 
void SetRoads(){
	int i;
	for(i=0; i<25; i++)
		Road_Y[i] += 40*i;

	for(i=0; i<20; i++){
		RoadSide_Y[i] += 50*i;
	} 
}

void ChangeRoads(){
	
	if(GameMode >= 1 && GameMode <= 3 && PlayMode){
		
		int i;

		for(i=0; i<25; i++){
			Road_Y[i] -= RoadSpeed;
		}

		for(i=0; i<25; i++){
			if(Road_Y[i] < 0){
				Road_Y[i] = SCREEN_HEIGHT;
			}
		}
		
		for(i=0; i<20; i++){
			RoadSide_Y[i] -= RoadSpeed;
		}

		for(i=0; i<20; i++){
			if(RoadSide_Y[i] < 0){
				RoadSide_Y[i] = SCREEN_HEIGHT;
			}
		}
 
	}

	else{
		iPauseTimer(Timer2);
	}

}  

void MoveOppUp(){

	if(Opp_Y + 0.5 > (800-OppCarLen) || Player_Y+Init_Vel > (800-PlayerCarLen)){
		
		MusicOn = false;
		PlayMusic();

		PlayMode = false;

		iPauseTimer(Timer1);
		iPauseTimer(Timer2);
		iPauseTimer(Timer3);
		iPauseTimer(Timer4);

		GameMode = 5;

		MusicOn = true;
		PlayMusic();

		if(Opp_Y + OppCarLen > Player_Y + PlayerCarLen){
			LevelWin = false;
		}
		else{
			LevelWin = true;
			GameScore = GameScore + 5000 + 20*(Player_Y-Opp_Y);
		}
	}

	if(GameMode >= 1 && GameMode <= 3 && PlayMode && Opp_Y + 0.5 <= (800-OppCarLen)){
		Opp_Y = Opp_Y + 0.5 + (GameMode-1)*0.5; 
	}

	if(PlayMode){
		GameScore += 0.2;
	}

}

void DrawTrack(){

	int i;

	iShowBMP2(75, 0, "road.bmp", 0); // Size of image is 480x70

	for(i=0; i<25; i++)
		iShowBMP2(75, Road_Y[i], "road.bmp", 0);

	iShowBMP2(0, 0, "roadsideleft.bmp", 0); // Size of image is 75x50
	
	for(i=0; i<20; i++)
		iShowBMP2(0, RoadSide_Y[i], "roadsideleft.bmp", 0);     

	iShowBMP2(555, 0, "roadsideright.bmp", 0); // Size of image is 75x50

	for(i=0; i<20; i++)
		iShowBMP2(555, RoadSide_Y[i], "roadsideright.bmp", 0);

	sprintf(ShowHighScoresText, "Score");
	sprintf(ShowHighScoresValue, "%.0f", GameScore);

	iShowBMP2(555, 569, "ScoreBox.bmp", 0); // 75x84
	iSetColor(240, 191, 96);

	iText(566, 620, ShowHighScoresText, GLUT_BITMAP_HELVETICA_18);
	iText(566, 590, ShowHighScoresValue, GLUT_BITMAP_HELVETICA_18);

	sprintf(ShowLevelText, "Level");
	sprintf(ShowLevelValue, "%02d", GameMode);
	
	iShowBMP2(0, 569, "ScoreBox.bmp", 0); // 75x84
	iSetColor(240, 191, 96);
	iText(13, 620, ShowLevelText, GLUT_BITMAP_HELVETICA_18); 
	iText(25, 590, ShowLevelValue, GLUT_BITMAP_HELVETICA_18); 

}

void DrawCars(){

	// Player Car
	iShowBMP2(Player_X, Player_Y, "yellow_resized.bmp", 0); // Size of image is 40x86 
	// Opponent Car
	iShowBMP2(Opp_X, Opp_Y, "red_resized.bmp", 0); // Size of image is 40x81

}

void DrawMenu(){

	// Background		
	iShowBMP(0, 0, "bg00.bmp");  // Size of image is 1200x800

	// Buttons
	// SS from ppt(80%). Cropped to make dim 283x93. Resized to 183x60

	iShowBMP2(223, 550, "NewGame.bmp", 0); // 183x60
	iShowBMP2(223, 475, "Instructions.bmp", 0); // 183x60
	iShowBMP2(223, 400, "Scores.bmp", 0); // 183x60
	iShowBMP2(223, 325, "Credits.bmp", 0); // 183x60
	iShowBMP2(223, 250, "Quit.bmp", 0); // 183x60

}

void PlayMusic(){

	if(MusicOn){
		PlaySoundA(Musics[GameMode], NULL, SND_LOOP | SND_ASYNC);
	}
	else{
		PlaySoundA(0, 0, 0);
	} 
}

void PlayCrashMusic(){

	if(CrashMusic){
		PlaySoundA("crashsound.wav", NULL, SND_FILENAME | SND_ASYNC);
	}
	else{
		PlaySoundA(0, 0, 0);
	} 
}

void DrawBarrierCars(){

	int i;

	for(i=0; i<4; i++){
		iShowBMP2(BarrCarX[i], BarrCarY[i], BarrierCars[i], 0); 
	}
}

void MoveBarrierCars(){

	int i, j;
	float temp1, temp2;

	if(GameMode >= 1 && GameMode <= 3 && PlayMode == true){

		for(i=0; i<4; i++){
			BarrCarY[i] = BarrCarY[i] - RoadSpeed - 3 - 2*GameMode;
		}
		
		for(i=0; i<4; i++){

			bool flag = true;

			if(BarrCarY[i] < 0 && Player_Y < 600){

				temp1 = 190 + rand() % 330;
				temp2 = 800 + rand() % (100*GameMode);
				
				for(j=0; j<4; j++){
					if(j==i) continue;
					if(temp1 >= (BarrCarX[j]-60) && temp1 <= (BarrCarX[j]+60)){
						if(temp2 >= (BarrCarY[j]-100) && temp2 <= (BarrCarY[j]+100)){
							flag = false;
							break;
						}
					}
				}

				if(flag){
					BarrCarX[i] = temp1;
					BarrCarY[i] = temp2;
				}
			}
		}

		CollisionCheck();
	} 

}

void CollisionCheck(){

	int i;

	for(i=0; i<4; i++){
		if(mod(Player_X-BarrCarX[i]) <= 38 && mod(Player_Y-BarrCarY[i]) <= 78){
			
			LevelWin = false;
			PlayMode = false;
			PlayerMotionUp = false;

			CollisionPosX = (Player_X + BarrCarX[i] + 40)/2;
			CollisionPosY = (Player_Y + BarrCarY[i] + 80)/2;

			MusicOn = false;
			PlayMusic();
	
			Crash = true;
			CrashMusic = true;
			PlayCrashMusic();
		}
	}

}

void ResetRace(){
	
	PlayMode = 0;
	Player_X  = PLAYER_INIT_POS_X; Player_Y = START_LINE_HEIGHT + 5;
	Opp_X = OPP_INIT_POS_X, Opp_Y = START_LINE_HEIGHT + 5;

	BarrCarX[0] = 200; 
	BarrCarX[1] = 300; 
	BarrCarX[2] = 400; 
	BarrCarX[3] = 500; 

	BarrCarY[0] = 1000;
	BarrCarY[1] = 1500;
	BarrCarY[2] = 2000;
	BarrCarY[3] = 1800;
}

void ShowHighScores(){

	iText(220, 625, "LEADERBOARD", GLUT_BITMAP_TIMES_ROMAN_24);
	iLine(220, 618, 398, 618);
	iLine(220, 613, 398, 613);

	int i;

	for(i=0; i<10; i++){

		if(HighScores[i].Name[0] == 0) break;

		sprintf(UserNames[i], "%02d) %s", i+1, HighScores[i].Name);
		sprintf(UserScores[i], "%.0f", HighScores[i].Score);
	} 

	for(i=0; i<10; i++){

		if(strcmp(UserScores[i], "0") == 0) break;

		iText(140, 530-40*i, UserNames[i], GLUT_BITMAP_HELVETICA_18);
		iText(390, 530-40*i, UserScores[i], GLUT_BITMAP_HELVETICA_18);
	} 
}

void SortScores(){

	int i, j;
	float temp_score;
	char temp_name[35] = "";

	FILE *fptr = fopen("leaderboard.txt", "r");

	for(i=0; i<10; i++){
		fscanf(fptr, "%s %f\n", &HighScores[i].Name, &HighScores[i].Score);
	}

	for(i=0; i<10; i++){
			for(j=i+1; j<11; j++){

				if(HighScores[i].Score < HighScores[j].Score){

					temp_score = HighScores[i].Score;
					HighScores[i].Score = HighScores[j].Score;
					HighScores[j].Score = temp_score;
				
					strcpy(temp_name, HighScores[i].Name);
					strcpy(HighScores[i].Name, HighScores[j].Name);
					strcpy(HighScores[j].Name, temp_name);
				}
			}
		}

	fclose(fptr);

	fptr = fopen("leaderboard.txt", "w");

	for(i=0; i<10; i++){
		fprintf(fptr, "%s %.0f\n", HighScores[i].Name, HighScores[i].Score);
	}

	fclose(fptr);
}

void FixLatestScores(){

	int i, j;
	float temp_score;
	char temp_name[35] = "";

	FILE *fptr = fopen("latestscores.txt", "r");

	for(i=0; i<10; i++){
		fscanf(fptr, "%s %f\n", &LatestScores[i].Name, &LatestScores[i].Score);
	}

	if(NewScoreTaken){

		strcpy(temp_name, LatestScores[10].Name);
		temp_score = LatestScores[10].Score;

		for(i=10; i>0; i--){
			strcpy(LatestScores[i].Name, LatestScores[i-1].Name);
			LatestScores[i].Score = LatestScores[i-1].Score;
		}

		strcpy(LatestScores[0].Name, temp_name);
		LatestScores[0].Score = temp_score; 

		NewScoreTaken = false;
	}


	fclose(fptr);

	fptr = fopen("latestscores.txt", "w");

	for(i=0; i<10; i++){
		fprintf(fptr, "%s %.0f\n", LatestScores[i].Name, LatestScores[i].Score);
	}

	fclose(fptr);
}

void ShowLatestScores(){
	
	iText(220, 625, "SCOREBOARD", GLUT_BITMAP_TIMES_ROMAN_24);
	iLine(220, 618, 383, 618);
	iLine(220, 613, 383, 613);

	int i;

	for(i=0; i<10; i++){
		if(LatestScores[i].Name[0] != 0){
			sprintf(UserNames[i], "%s", LatestScores[i].Name);
			sprintf(UserScores[i], "%.0f", LatestScores[i].Score);
		}
	} 

	for(i=0; i<10; i++){
		if(strcmp(UserScores[i], "0") != 0){
			iText(140, 530-40*i, UserNames[i], GLUT_BITMAP_HELVETICA_18);
			iText(390, 530-40*i, UserScores[i], GLUT_BITMAP_HELVETICA_18);
		}
	} 
}

void ChangeToResult(){
	
	GameMode = 5;
	CrashMusic = false;
	Crash = false;
	MusicOn = true;
	PlayMusic();
}