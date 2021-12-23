/***********************************
/ hecho con raylib 4.0 - raylib.com 
/ por Armando Orozco		  
/ compilacion del 22 / 11 / 2021 
/*********************************
/ made with raylib 4.0 - raylib.com 
/ by Armando Orozco		  
/ compilation date 22 / 11 / 2021 
***********************************/

#include "raylib.h"

//core
const int screenwidth = 1280;
const int screenheight = 720;
int frameCounter=0;
bool pause = true;
float speedMultli = 1;

//rectangle
const int rectangleHeight = screenheight / 10;
const int rectangleWidth = screenwidth / 80;
const int rectangleX = 25;
const int rectangleXder = screenwidth - rectangleX * 2;

int rectangleYizq = (screenheight / 2) - rectangleHeight/2;
int rectangleYder = (screenheight / 2) - rectangleHeight/2;


float posicion_1 = rectangleYder;
float velocidad_1 = 0.f;
float posicion_2 = rectangleYizq;
float velocidad_2 = 0.f;

//ball
Vector2 center = { screenwidth / 2, screenheight / 2 };
Vector2 BallSpeed = { 4.0f, 5.0f };
float radio = 5.5;

//puntos
int P1Points = 0;
int P2Points = 0;
bool puntosX = false;
bool puntosY = false;

//mando si toco la pared o es la R de restar(1, si toca la R, 0 si toca la pared)
void res(bool res){

	//speedMultli = 1;
	center = { screenwidth / 2, screenheight / 2 };
	pause = true;
	if (res) {
		BallSpeed = { 5.0f, 5.0f };
		P1Points = 0;
		P2Points = 0;
	}
	
}

void drawBorder() {
	//rectangle
	Rectangle rectangleline;
	rectangleline.x = 0;
	rectangleline.y = 0;
	rectangleline.width = screenwidth;
	rectangleline.height = screenheight;

	//lines
	Vector2 startPos;
	startPos.x = screenwidth / 2;
	startPos.y = 0;

	Vector2 endPos;
	endPos.x = screenwidth / 2;
	endPos.y = screenheight;

	float thick = 5;
	Color c = WHITE;

	Vector2 texPos = { screenwidth / 4, screenheight / 50 };
	
	//draw
	
	

	DrawRectangleLinesEx(rectangleline, 2 , WHITE);
	DrawLineEx(startPos, endPos, thick, c);

	DrawText(TextFormat("%d", P1Points), screenwidth / 4, screenheight / 50, 50, RAYWHITE);
	DrawText(TextFormat("%d", P2Points), screenwidth - (screenwidth / 4), screenheight / 50, 50, RAYWHITE);
	
}


void update(bool pause, Sound punto, Sound colision) {
	//restart
	if (IsKeyPressed(KEY_R)) {
		res(1);
	}
	float dt = GetFrameTime();
	float aceleracion_1 = .0f;
	float aceleracion_2 = .0f;
	float constante = 6500.f;
	//izquierda
	{
		if (IsKeyDown(KEY_W) && posicion_1 > 0) aceleracion_1 -= constante;
		if (IsKeyDown(KEY_S) && (posicion_1 + rectangleHeight) < screenheight) aceleracion_1 += constante;


		aceleracion_1 -= velocidad_1 * 10.f;

		posicion_1 = posicion_1 + velocidad_1 * dt + aceleracion_1 * dt * dt * .5f;
		velocidad_1 = velocidad_1 + aceleracion_1 * dt;

		if (posicion_1 > 0 && (posicion_1 + rectangleHeight) < screenheight) { rectangleYizq = posicion_1; }
		else {
			if (posicion_1 < 0) {
				posicion_1 = 0;
				velocidad_1 *= -0.001f;
			}
			if ((posicion_1 + rectangleHeight) > screenheight) {
				posicion_1 = screenheight - rectangleHeight;
				velocidad_1 *= -0.001f;
			}
		}
	}

	//derecho
	{
		if (IsKeyDown(KEY_UP) && posicion_2 > 0) aceleracion_2 -= constante;
		if (IsKeyDown(KEY_DOWN) && (posicion_2 + rectangleHeight) < screenheight) aceleracion_2 += constante;

		aceleracion_2 -= velocidad_2 * 10.f;

		posicion_2 = posicion_2 + velocidad_2 * dt + aceleracion_2 * dt * dt * .5f;
		velocidad_2 = velocidad_2 + aceleracion_2 * dt;

		if (posicion_2 > 0 && (posicion_2 + rectangleHeight) < screenheight) { rectangleYder = posicion_2; }
		else {

			if (posicion_2 < 0) {
				posicion_2 = 0;
				velocidad_2 *= -0.001f;
			}
			if ((posicion_2 + rectangleHeight) > screenheight) {
				posicion_2 = screenheight - rectangleHeight;
				velocidad_2 *= -0.001f;
			}

		}

	}

	if (!pause) {
		//ball
		center.x += BallSpeed.x;
		center.y += BallSpeed.y;

		//colission
		{
			if (center.x <= rectangleX + rectangleWidth && center.y <= rectangleYizq + rectangleHeight && center.y >= rectangleYizq && center.x >= rectangleX) {
				BallSpeed.x *= -1.0f;
				BallSpeed.y = (center.y - (rectangleYizq + (rectangleHeight / 2))) / 3 + velocidad_1 * 0.005f;
				//speedMultli += 0.2f;
				PlaySound(colision);
			}

			if (center.x >= rectangleXder && center.y <= rectangleYder + rectangleHeight && center.y >= rectangleYder && center.x <= rectangleXder + rectangleWidth) {
				BallSpeed.x *= -1.0f;
				BallSpeed.y = (center.y - (rectangleYder + (rectangleHeight / 2))) / 3 + velocidad_2 * 0.005f;
				//speedMultli += 0.2f;
				PlaySound(colision);
			}
		}
		//paredes
		{
			if (center.y < 0 || center.y > GetScreenHeight()) BallSpeed.y *= -1.0f;

			if (center.x < 0 || center.x > GetScreenWidth()) {
				if (center.x <= 0) {
					P2Points++;
					puntosY = true;

				}

				if (center.x >= GetScreenWidth()) {
					P1Points++;
					puntosX = true;
				}

				PlaySound(punto);
				BallSpeed.x *= -1.0f;
				BallSpeed.y = -1.0f;
				res(0);
			}

		}
	}

}

void draw() {
	BeginDrawing();

	ClearBackground(BLACK);

	drawBorder();

	DrawRectangle(rectangleXder, rectangleYder, rectangleWidth, rectangleHeight, RAYWHITE);
	DrawRectangle(rectangleX, rectangleYizq, rectangleWidth, rectangleHeight, RAYWHITE);

	DrawCircleV(center, radio, RAYWHITE);
	

	EndDrawing();
}

void menu() {

	int frameCounter =0 ;
	while (!IsKeyPressed(KEY_SPACE))
	{
		BeginDrawing();
		ClearBackground(BLACK);


		DrawText("PONG", GetScreenWidth() / 4 + GetScreenWidth()/7, GetScreenHeight()/4 - 02, 100, RAYWHITE);
		
		if ((frameCounter/30)%2)
		{
			DrawText("PRESS SPACE TO PLAY", GetScreenWidth() / 4, GetScreenHeight() / 2, 50, RAYWHITE);
		}
		frameCounter++;

		DrawText("PRESS ESC TO QUIT GAME", GetScreenWidth() / 3 + 50, GetScreenHeight() - GetScreenHeight() / 4, 25, RAYWHITE);

		EndDrawing();

		if (IsKeyReleased(KEY_ESCAPE)) CloseWindow();
	}

}

int main(void) {
	//inizialitation
		//window
	InitWindow(screenwidth,screenheight, "PONG");


		//audio
	InitAudioDevice();
	Sound colision = LoadSound("resources/audio/colision.wav");
	Sound punto = LoadSound("resources/audio/punto.ogg");
	SetTargetFPS(60);
	
	menu();


	while (!WindowShouldClose())
	{
		//DrawFPS(0,0);
		if (IsKeyPressed(KEY_SPACE)) pause = false;

			if (puntosX) {
				frameCounter++;
				if (frameCounter < 120) { DrawText("Point for player 1 \npress SPACE to serve", screenheight - (screenwidth / 6), screenheight / 60, 25, LIGHTGRAY); }
				else { puntosX = false; frameCounter = 0; }
			}

			if (puntosY) {
				frameCounter++;
				if (frameCounter < 120) { DrawText("Point for player 2 \npress SPACE to serve", screenheight - (screenwidth / 6), screenheight / 60, 25, LIGHTGRAY); }
				else { puntosY = false; frameCounter = 0; }
			}

			
			update(pause,punto,colision);

			
			
			draw();
	}

	UnloadSound(colision);
	UnloadSound(punto);

	CloseAudioDevice();
	CloseWindow();
	return(0);
}