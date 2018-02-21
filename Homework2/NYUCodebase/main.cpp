
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Thing.h"
#include <cmath>
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
Thing* paddle1, *paddle2, *ball, *rightWon, *leftWon;
float lastFrameTicks;
const Uint8 *keys = SDL_GetKeyboardState(NULL);

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	if (!surface){ return -1; }
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

void PreGameLoop(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	lastFrameTicks = 0.0f;

	GLuint paddle = LoadTexture("paddle.png");
	GLuint ballPic = LoadTexture("ball.png");
	GLuint leftWins = LoadTexture("left.png");
	GLuint rightWins = LoadTexture("right.png");

	ball = new Thing(ballPic, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f);
	paddle1 = new Thing(paddle,1.2f,0.0f,90.0f,0.1f,0.4f);
	paddle2 = new Thing(paddle, -1.2f, 0.0f, 90.0f, 0.1f, 0.4f);
	rightWon = new Thing(rightWins, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	leftWon = new Thing(leftWins, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	ball->speed = 1.0f;
	ball->direction_y = 0.75f;
	ball->direction_x = 0.75f;

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	glViewport(0, 0, 800, 600);

}

bool notDone(){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
		else if (event.type == SDL_MOUSEMOTION) {
			paddle1->y = (((float)(600 - event.motion.y) / 600.0f) * 2.0f) - 1.0f; //the new y position from mouse
		}

	}
	return true;
}

void updateFrame(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	ball->rotation += elapsed * 90;
	
	ball->y += ball->speed*elapsed*ball->direction_y;
	ball->x += ball->speed*elapsed*ball->direction_x;

	//paddle2->y = ball->y; // ideal machine player

	if (keys[SDL_SCANCODE_UP]) {
		paddle1->y += 1.0f*elapsed;
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		paddle1->y -= 1.0f*elapsed;
	}

	if (keys[SDL_SCANCODE_W]) {
		paddle2->y += 1.0f*elapsed;
	}
	else if (keys[SDL_SCANCODE_S]) {
		paddle2->y -= 1.0f*elapsed;
	}

	if (paddle1->y > 0.8f){ paddle1->y = 0.8f; }
	else if (paddle1->y < -0.8f){ paddle1->y = -0.8f; }
	if (paddle2->y > 0.8f){ paddle2->y = 0.8f; }
	else if (paddle2->y < -0.8f){ paddle2->y = -0.8f; }
	
	if (ball->y>0.95f&&ball->direction_y > 0.0f){ ball->direction_y = -ball->direction_y; }
	else if (ball->y<-0.95f&&ball->direction_y < 0.0f){ ball->direction_y = -ball->direction_y; }

	if (ball->x<-1.1f&&ball->x>-1.3&&ball->y>paddle2->y - 0.25f&&ball->y < paddle2->y + 0.25f){ ball->direction_x = 1.0; }
	else if (ball->x>1.1f&&ball->x<1.3&&ball->y>paddle1->y - 0.25f&&ball->y < paddle1->y + 0.25f){ ball->direction_x = -1.0; }
}

void reDraw(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	paddle1->Draw();
	paddle2->Draw();
	if (ball->x < -1.4){ rightWon->Draw(); } //left player lost
	else if (ball->x > 1.4){ leftWon->Draw(); } //right player lost
	else ball->Draw();
	SDL_GL_SwapWindow(displayWindow);
}


int main(int argc, char *argv[])
{
	PreGameLoop();
	while (notDone()) {
		updateFrame();
		reDraw();
	}
	
	return 0;
}
