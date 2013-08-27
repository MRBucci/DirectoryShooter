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
bool checkHorizontalCollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh, float Avel){
    //if A is above or below B, do nothing
    if ((Ay+Ah < By) || (Ay > By+Bh)) return false;
    else{
        //A left of B's center AND A is on the B AND going right
        if (((Ax+Aw < Bx+.5*Bw) && (Ax+Aw > Bx) && (Avel>0)) ||//change to left
        //A right of B's center AND A is on the B AND going left
            ((Ax > Bx+.5*Bw) && (Ax < Bx+Bw) && (Avel<0))) return true;//change to right
        //A hit to side it's going toward
        else return false;//so it keeps going that way
    }
}

//Add durability to blocks


/*
//gives fresh start to player when game ends
public void startPlayer(Player p){
    p.pX = 300;
    p.pY = 370;
    p.left = false
    p.right = false
    p.up = false
    p.down = false;
    p.gotColorChange = false;
    p.gotGun = false;
}

//put ball back at beginning when game ends
public void startBall(Ball b){
    b.pX = 50;
    b.pY = 300;
    b.vX = 4.0;
    b.vY = -4.0;
}*/


struct Player{
    float pX;//position in X
    float pY;//position in Y
    float width;//width
    float height;//height

    //keep track of player movement
    //based on arrow keys
    bool left,right,up,down;
    //You don't start with powerups
    bool gotColorChange;
    bool gotGun;
    Player(){
        pX = 300;
        pY = 370;
        width = 80;
        height = 20;
        //keep track of player movement
        //based on arrow keys
        left = false,right = false,up = false,down = false;
        //You don't start with powerups
        gotColorChange = false;
        gotGun = false;


    }
};

struct Ball{
    float pX;//position in X
    float pY;//position in Y
    float vX;//velocity in X
    float vY;//velocity in Y
    float wh;//width/height of ball
    Ball(){
        pX = 50;
        pY = 300;
        vX = 4.0;//velocity in X
        vY = -4.0;//velocity in Y
        wh = 30;//width/height of ball
    }
};

struct PowerUp{
    float pX;//position in X
    float pY;//position in Y
    float vX;//velocity in X
    float vY;//velocity in Y
    float wh;//width/height of powerup
    bool alive;//state(alive/dead)
    int type;//type of powerup (1 for color change, 2 for gun)
    PowerUp(){
        pX = 30;//position in X
        pY = 30;//position in Y
        vX = 0;//velocity in X
        vY = 0;//velocity in Y
        wh = 30;//width/height of powerup
        alive = false;//state(alive/dead)
    }
};

struct Brick{
    float pX;//position in X
    float pY;//position in Y
    float width;//width
    float height;//height
    bool alive;//state(alive/dead)
    int type;//type of powerup (1 for doc, 2 for pic, 3 for music, 4 for special)
    Brick(){
        alive = true;
        width = 60;
        height = 20;
    }
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

    //The Player start position
    Player pOne;

    //The Ball start position
    Ball bOne;

    //Color Change PowerUp
    PowerUp colorChange;
    colorChange.type = 1;

    //Gun PowerUp
    PowerUp gun;
    gun.type = 2;

    //Brick elements
    static int BRICKS = 0;

    //test set of files by type
    int files[2] = {2, 2};

    //use files to determine how many bricks will be made
    for(int i=0;i<sizeof(files);i++){
        if(files[i] == 1){
            BRICKS += 5;
        }else if(files[i] == 2){
            BRICKS += 3;
        }else if(files[i] == 3){
            BRICKS += 2;
        }else{
            BRICKS += 1;
        }
    }

    //make group of bricks of determined size
    Brick bricks[BRICKS];

    //make 5 bricks for doc, 3 for pic, 2 for music, 1 for other
    int x = 4;
    int y = 10;
    int b = 0;
    for(int i=0;i<sizeof(files);i++){
        if(files[i] == 1){
                for(int n=b;n<b+5;b++,x+=66){
                    if(x > 560){
                        x = 4;
                        y += 25;
                    }
                    bricks[n].pX = x;
                    bricks[n].pY = y;
                    bricks[n].type = 1;
                }
                b+=5;
        }else if(files[i] == 2){
                for(int n=b;n<b+3;b++,x+=66){
                    if(x > 560){
                        x = 4;
                        y += 25;
                    }
                    bricks[n].pX = x;
                    bricks[n].pY = y;
                    bricks[n].type = 2;
                }
                b+=3;
        }else if(files[i] == 3){
                for(int n=b;n<b+2;b++,x+=66){
                    if(x > 560){
                        x = 4;
                        y += 25;
                    }
                    bricks[n].pX = x;
                    bricks[n].pY = y;
                    bricks[n].type = 3;
                }
                b+=2;
        }else{
                for(int n=b;n<b+1;b++,x+=66){
                    if(x > 560){
                        x = 4;
                        y += 25;
                    }
                    bricks[n].pX = x;
                    bricks[n].pY = y;
                    bricks[n].type = 4;
                }
                b+=1;
        }
        x+=66;
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

            //Player movement mapped to arrow keys
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_LEFT){
                    pOne.left = true;
                }
                if (event.key.keysym.sym == SDLK_RIGHT){
                    pOne.right = true;
                }
                if (event.key.keysym.sym == SDLK_UP){
                    pOne.up = true;
                }
                if (event.key.keysym.sym == SDLK_DOWN){
                    pOne.down = true;
                }
            } else if (event.type == SDL_KEYUP){
                if (event.key.keysym.sym == SDLK_LEFT){
                    pOne.left = false;
                }
                if (event.key.keysym.sym == SDLK_RIGHT){
                    pOne.right = false;
                }
                if (event.key.keysym.sym == SDLK_UP){
                    pOne.up = false;
                }
                if (event.key.keysym.sym == SDLK_DOWN){
                    pOne.down = false;
                }
            }
            //logic that should happen for a certain event
        }

        //LOGIC
            //Player
            //Speed
            if (pOne.left == true){
                pOne.pX -= 4.5;
            }
            if (pOne.right == true){
                pOne.pX += 4.5;
            }
            if (pOne.up == true){
                pOne.pY -= 4.5;
            }
            if (pOne.down == true){
                pOne.pY += 4.5;
            }
            //Doesn't let player leave screen
            if (pOne.pX < 0){
                pOne.pX = 0;
            }
            if (pOne.pX+pOne.width > 600){
                pOne.pX = 600 - pOne.width;
            }
            if (pOne.pY < 0){
                pOne.pY = 0;
            }
            if (pOne.pY+pOne.height > 400){
                pOne.pY = 400 - pOne.height;
            }

            //Ball
            bOne.pX += bOne.vX;


            //Brick collision detecting
            for(int n=0; n<BRICKS; n++){
                if (bricks[n].alive){
                    if(checkVerticalCollision(bOne.pX,bOne.pY,bOne.wh,bOne.wh,bricks[n].pX,bricks[n].pY,bricks[n].width,bricks[n].height)){
                        bOne.vX = -bOne.vX;
                        bricks[n].alive = false;
                        break;
                    }
                }
            }
            bOne.pY += bOne.vY;

            for(int n=0; n<BRICKS; n++){
                if (bricks[n].alive){
                    if(checkHorizontalCollision(bOne.pX,bOne.pY,bOne.wh,bOne.wh,bricks[n].pX,bricks[n].pY,bricks[n].width,bricks[n].height, bOne.vX)){
                        bOne.vY = -bOne.vY;
                        bricks[n].alive = false;
                        break;
                    }
                }
            }

            //X leaving screen conditions
            if (bOne.pX < 0){
                bOne.vX = -bOne.vX;
            }else if (bOne.pX+bOne.wh > 600){
                bOne.vX = -bOne.vX;
            }
            //Y leaving screen conditions
            if (bOne.pY < 0){
                bOne.vY = -bOne.vY;
            }else if (bOne.pY+bOne.wh > 400){
                //Restart player
                pOne = Player();
                //Restart ball
                bOne = Ball();
                //Restart bricks
                for(int n=0;n<BRICKS;n++){
                    bricks[n].alive = true;
                }
            }

            if(checkVerticalCollision(bOne.pX,bOne.pY,bOne.wh,bOne.wh,pOne.pX,pOne.pY,pOne.width,pOne.height)){
                bOne.vY = -bOne.vY;
            }

            if(checkHorizontalCollision(bOne.pX,bOne.pY,bOne.wh,bOne.wh,pOne.pX,pOne.pY,pOne.width,pOne.height, bOne.vX)){
                bOne.vX = -bOne.vX;
            }



        //RENDERING to the screen
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix(); //Start phase

            glOrtho(0,600,400,0,-1,1); //Set the matrix

            glColor4ub(0,0,0,255); //Player color (black)

            //Player
            if(pOne.gotColorChange) glColor4ub(122,0,255,255); //powered color (purple)
            else glColor4ub(0,0,0,255); //normal color (black)

            glBegin(GL_QUADS);

            glVertex2f(pOne.pX,pOne.pY);
            glVertex2f(pOne.pX+pOne.width,pOne.pY);
            glVertex2f(pOne.pX+pOne.width,pOne.pY+pOne.height);
            glVertex2f(pOne.pX,pOne.pY+pOne.height);

            glEnd();

            //Ball
            glColor4ub(255,0,0,255); //Ball Color (red)
            glBegin(GL_QUADS);

            glVertex2f(bOne.pX,bOne.pY);
            glVertex2f(bOne.pX+bOne.wh,bOne.pY);
            glVertex2f(bOne.pX+bOne.wh,bOne.pY+bOne.wh);
            glVertex2f(bOne.pX,bOne.pY+bOne.wh);

            glEnd();

            //Bricks
            glBegin(GL_QUADS);

            for(int n=0;n<BRICKS;n++){
                if(bricks[n].alive){
                    if(bricks[n].type == 1) glColor4ub(122,0,255,255);//Document Brick color (purple)
                    else if(bricks[n].type == 2) glColor4ub(0,0,255,255);//Picture Brick color (blue)
                    else if(bricks[n].type == 3) glColor4ub(255,0,255,255);//Music Brick color
                    else glColor4ub(122,122,122,255);//Special Brick color

                    glVertex2f(bricks[n].pX,bricks[n].pY);
                    glVertex2f(bricks[n].pX+bricks[n].width,bricks[n].pY);
                    glVertex2f(bricks[n].pX+bricks[n].width,bricks[n].pY+bricks[n].height);
                    glVertex2f(bricks[n].pX,bricks[n].pY+bricks[n].height);
                }
            }

            glEnd();

            glPopMatrix(); //End

            SDL_GL_SwapBuffers();

            SDL_Delay(1); //Delay / pause, so program doesnt go as fast as possible
    }

    std::cin.get();

    SDL_Quit();

    return 0;
}
