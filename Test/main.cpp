/*
main
MAde following tutorials by Luka Horvat
Also take a look at lazyfoo if you want further help
*/

//Specific headers
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>

//vertical collision detection function
bool checkVerticalCollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh){
    if (Ay+Ah < By) return false;//A above B
    else if (Ay > By+Bh) return false;//A below B
    else if (Ax+Aw < Bx) return false;//A left of B
    else if (Ax > Bx+Bw) return false;//A right of B
    return true;//otherwise, collision occured
}

//horizontal collision detection function
int checkHorizontalCollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh, int Avel){
    //if A is above or below B, do nothing
    if ((Ay+Ah < By) || (Ay > By+Bh)) return Avel;
    else{
        //A left of B's center AND A is on the B AND going right
        if (((Ax+Aw < Bx+.5*Bw) && (Ax+Aw > Bx) && (Avel>0)) ||//change to left
        //A right of B's center AND A is on the B AND going left
            ((Ax > Bx+.5*Bw) && (Ax < Bx+Bw) && (Avel<0))) return -Avel;//change to right
        //A hit to side it's going toward
        else return Avel;//so it keeps going that way
    }
}

struct Brick{
    float x;
    float y;
    float width;
    float height;
    bool alive;
};


//Start of program
int main(int argc, char* args[]){
    //Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    //Set OpenGL memory usage
    //Good default settings
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    //Caption of the window
    SDL_WM_SetCaption( "Our first game", NULL);

    //Size of window
    SDL_SetVideoMode(600, 400, 32, SDL_OPENGL); //Could add '| FULLSCREEN'

    //Specify the clear color
    glClearColor(1,1,1,1); //RED,GREEN,BLUE,ALPHA (alpha should be one)

    //What portion of the screen to display
    glViewport(0,0,600,400);

    //Shader model - Use this to p<uses-permission android:name="android.permission.INTERNET"></uses-permission>revent ugly
    glShadeModel(GL_SMOOTH);

    //2D rendering
    glMatrixMode(GL_PROJECTION);

    //"Save" it
    glLoadIdentity();

    //Disable depth checking (cuz 2D)
    glDisable(GL_DEPTH_TEST);

    std::cout << "OpenGL is running\n";
    std::cout << "Main loop has started\n";

    //Handles the main loop
    bool isRunning = true;

    //For handling with events
    SDL_Event event;

    //The Player elements
    float myX = 300;
    float myY = 370;
    float width = 80;
    float height = 20;

    bool left = false, right = false;//left and right keys do not start pressed
    bool up = false, down = false;

    //The Ball elements
    float ballX = 50;
    float ballY = 300;
    float ballWH = 30;

    float vellX = 5.0;
    float vellY = -5.0;

    //Brick elements
    const static int BRICKS = 45;

    Brick bricks[BRICKS];

    for(int n=0, x=4, y=10;n<BRICKS;n++,x+=66){
        if(x > 560){
            x = 4;
            y += 25;
        }
        bricks[n].x = x;
        bricks[n].y = y;
        bricks[n].width = 60;
        bricks[n].height = 20;
        bricks[n].alive = true;
    }

    //Main game loop
    while(isRunning){
        //EVENTS
        while(SDL_PollEvent(&event)){
            //Uf window was closed
            if(event.type == SDL_QUIT){
                isRunning = false;
            }
            //KEYUP is for release, KEYDOWN is for press
            //If a button was released && if the button was escape
            if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE){
                isRunning = false;
            }

            //press r to change clear color to red (simple test)
            if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_r){
                glClearColor(1,0,0,1);
            }

            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_LEFT){
                    left = true;
                }
                if (event.key.keysym.sym == SDLK_RIGHT){
                    right = true;
                }
                if (event.key.keysym.sym == SDLK_UP){
                    up = true;
                }
                if (event.key.keysym.sym == SDLK_DOWN){
                    down = true;
                }
            } else if (event.type == SDL_KEYUP){
                if (event.key.keysym.sym == SDLK_LEFT){
                    left = false;
                }
                if (event.key.keysym.sym == SDLK_RIGHT){
                    right = false;
                }
                if (event.key.keysym.sym == SDLK_UP){
                    up = false;
                }
                if (event.key.keysym.sym == SDLK_DOWN){
                    down = false;
                }
            }



            //logic that should happen for a certain event

        }

        //LOGIC
            //Player
            if (left == true){
                myX -= 4.5;
            }
            if (right == true){
                myX += 4.5;
            }
            if (up == true){
                myY -= 4.5;
            }
            if (down == true){
                myY += 4.5;
            }
            if (myX < 0){
                myX = 0;
            }
            if (myX+width > 600){
                myX = 600 - width;
            }
            if (myY < 0){
                myY = 0;
            }
            if (myY+height > 400){
                myY = 400 - height;
            }

            //Ball
            ballX += vellX;


            //Brick collision detecting
            for(int n=0; n<BRICKS; n++){
                if (bricks[n].alive){
                    if(checkVerticalCollision(ballX,ballY,ballWH,ballWH,bricks[n].x,bricks[n].y,bricks[n].width,bricks[n].height)){
                        vellX = -vellX;
                        bricks[n].alive = false;
                        break;
                    }
                }
            }
            ballY += vellY;

            for(int n=0; n<BRICKS; n++){
                if (bricks[n].alive){
                    if(checkVerticalCollision(ballX,ballY,ballWH,ballWH,bricks[n].x,bricks[n].y,bricks[n].width,bricks[n].height)){
                        vellY = -vellY;
                        bricks[n].alive = false;
                        break;
                    }
                }
            }

            //X leaving screen conditions
            if (ballX < 0){
                vellX = -vellX;
            }else if (ballX+ballWH > 600){
                vellX = -vellX;
            }
            //Y leaving screen conditions
            if (ballY < 0){
                vellY = -vellY;
            }else if (ballY+ballWH > 400){
                //The Player elements
                myX = 300;
                myY = 370;
                width = 80;
                height = 20;

                left = false, right = false;//left and right keys do not start pressed
                up = false, down = false;

                //The Ball elements
                ballX = 50;
                ballY = 300;
                ballWH = 30;

                vellX = 5.0;
                vellY = -5.0;

                for(int n=0, x=4, y=10;n<BRICKS;n++,x+=66){
                    bricks[n].alive = true;
                }

            }

            if(checkVerticalCollision(ballX,ballY,ballWH,ballWH,myX,myY,width,height)){
                vellY = -vellY;
            }

            vellX = checkHorizontalCollision(ballX,ballY,ballWH,ballWH,myX,myY,width,height,vellX);



        //RENDERING to the screen
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix(); //Start phase

            glOrtho(0,600,400,0,-1,1); //Set the matrix

            glColor4ub(0,0,0,255); //Player color (black)

            //Player

            glBegin(GL_QUADS);

            glVertex2f(myX,myY);
            glVertex2f(myX+width,myY);
            glVertex2f(myX+width,myY+height);
            glVertex2f(myX,myY+height);

            glEnd();

            //Ball
            glColor4ub(255,0,0,255); //Ball Color (red)
            glBegin(GL_QUADS);

            glVertex2f(ballX,ballY);
            glVertex2f(ballX+ballWH,ballY);
            glVertex2f(ballX+ballWH,ballY+ballWH);
            glVertex2f(ballX,ballY+ballWH);

            glEnd();

            //Bricks
            glColor4ub(0,0,255,255);//Brick color (blue)
            glBegin(GL_QUADS);

            for(int n=0;n<BRICKS;n++){
                if(bricks[n].alive){
                    if(n%2 == 0) glColor4ub(122,0,255,255);//Brick color 2 (purple)
                    else glColor4ub(0,0,255,255);//Original Brick color (blue)
                    glVertex2f(bricks[n].x,bricks[n].y);
                    glVertex2f(bricks[n].x+bricks[n].width,bricks[n].y);
                    glVertex2f(bricks[n].x+bricks[n].width,bricks[n].y+bricks[n].height);
                    glVertex2f(bricks[n].x,bricks[n].y+bricks[n].height);
                }
            }

            glEnd();

            /*
            glColor4ub(255,0,0,255); //Red color

            //First Drawing, a rectangle
            glBegin(GL_QUADS); //GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLYGON

            //TWO dimensions, FLOAT
            glVertex2f(5,5); //top left corner
            glVertex2f(595, 5);

            glColor4ub(0, 255, 0, 255); //green
            glVertex2f(595, 395);
            glVertex2f(5,395); //bottom right corner


            glEnd(); // end drawing

            //Second drawing
            //a line
            glBegin(GL_LINES);

            glColor4ub(0,0,0,255); // black
            glVertex2f(0,0);
            glVertex2f(600, 400);

            glEnd();

            //Third Drawing
            //another line
            glBegin(GL_LINES);

            glVertex2f(600, 0);
            glVertex2f(0,400);

            glEnd();
            */

            glPopMatrix(); //End

            SDL_GL_SwapBuffers();

            SDL_Delay(1); //Delay / pause, so program doesnt go as fast as possible
    }

    std::cin.get();

    SDL_Quit();

    return 0;
}
