#include <SFML/Graphics.hpp>
#include<pthread.h>
#include<string.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <stdio.h>
#include <X11/Xlib.h>
#include<stdlib.h>
#include <time.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <random>
#include"player.h"

pthread_mutex_t ghost_mutex = PTHREAD_MUTEX_INITIALIZER ;

class GHOST{

private:
    sf::Texture texture1;
    sf::Texture texture2;
    sf::Sprite sprite;
    int frame1,frame2;
    float x, y;
    float speed; 
    char dir;
    float angle;

public:
    bool exiting_gbox;
    bool speed_boost;

    GHOST(float startX, float startY, float movementSpeed) 
        : x(startX), y(startY), speed(movementSpeed),exiting_gbox(true)
    {
        // Load the Pacman texture
        if (!texture1.loadFromFile("img/ghostx.png") ) 
        {// Error handling if the image fails to load
            throw std::runtime_error("Failed to load Pacman texture.");}

        if(!texture2.loadFromFile("img/ghosty.png"))
        {// Error handling if the image fails to load
            throw std::runtime_error("Failed to load Pacman texture.");}


        // Set the texture to the sprite
        sprite.setTexture(texture1);
        sprite.setScale(0.035f,0.035f);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
        frame1 = 10;
        frame2 = 0;
        // Set the initial position of the sprite
        sprite.setPosition(x, y);
        
    }

    void reset (float startX, float startY){
        x=startX;
        y=startY;
        exiting_gbox=true;
        return;
    }

    bool ghost_hit(sf::Sprite & player)
    {
          if( ( abs(sprite.getPosition().x - player.getPosition().x)<=10 )
                    && ( abs(sprite.getPosition().y - player.getPosition().y)<=10) ){
                    return true;
                }
            else {return false;}
    }

    //function to move the ghost...
    void move(sf::Sprite & player, MAP &mapX) {
        static bool hv = true; //true - horizontal.. // false vertical

         pthread_mutex_lock(&ghost_mutex);//locking..
        
        float oldx = x , oldy = y;
        float px = player.getPosition().x , py=player.getPosition().y ;

        /*if(px>x ){ dir = 'R' ;}
         else if(px<x){ dir = 'L' ;}
         else if(py>y){ dir = 'D' ;}

         else if(py<y){ dir = 'U' ;}*/

          //dir = rand_dir()  ;
        // Update the position of the sprite
        

        if(dir=='R'){ x+=0.0005; angle = 0;}//Right
        if(dir=='L'){ x-=0.0005; angle = 0;}//Left
        if(dir=='U'){ y-=0.0005; angle = 0;}//Up
        if(dir=='D'){ y+=0.0005; angle = 0;}//Down
        
        
        sprite.setRotation(angle);
        sprite.setPosition(x, y);

        //if colliding wiht the maze...
        
        if(mapX.internal_collision(sprite) )
        {
            //sprite.setRotation(0);
           
            x = oldx , y = oldy;
            sprite.setPosition(x, y);

            if(px>x & hv ){ dir = 'R' ;  hv=!hv;}
                else if(px<x & hv ){ dir = 'L' ; hv=!hv;}
                else if(py>y & !hv){ dir = 'D' ; hv=!hv;}
                else if(py<y & !hv){ dir = 'U' ;hv=!hv;}

            /**
            if(dir=='R'){ x-=0.0005; angle = 0;  }//Right
		    if(dir=='L'){ x+=0.0005; angle = 0; }//Left
		    if(dir=='U'){ y+=0.0005; angle = 0; }//Up
		    if(dir=='D'){ y-=0.0005; angle = 0; }//down
             sprite.setPosition(x, y);*/
             
            //cout << !(sprite.getGlobalBounds().intersects(mapX.background.getGlobalBounds())) << endl; //"colliding";
        }

        if(!(sprite.getGlobalBounds().intersects(mapX.background.getGlobalBounds())))
        {
            //x = oldx , y = oldy;
            //sprite.setPosition(x, y);

                cout << "here...";
            
            
                if(dir=='R'){ x-=25; angle = 0; }//Right
                if(dir=='L'){ x+=25; angle = 0; }//Left
                if(dir=='U'){ y+=25; angle = 0; }//Up
                if(dir=='D'){ y-=25; angle = 0; }//down
             //sprite.setPosition(x, y);
    
                
            if(px>x & hv ){ dir = 'R' ;  hv=!hv;}
                else if(px<x & hv ){ dir = 'L' ; hv=!hv;}
                else if(py>y & !hv){ dir = 'D' ; hv=!hv;}
                else if(py<y & !hv){ dir = 'U' ;hv=!hv;}
            
            sprite.setPosition(x, y);
            //cout << !(sprite.getGlobalBounds().intersects(mapX.background.getGlobalBounds())) << endl; //"colliding";
        }

        pthread_mutex_unlock(&ghost_mutex);//unlocking..
        return;

    }

//for the random direction change
        char rand_dir(){

            //the random number generator
            int randomValue = (rand()%4)+1;
            

            if(randomValue==1){return 'R';}
            if(randomValue==2){return 'L';}
            if(randomValue==3){return 'U';}
            if(randomValue==4){return 'P';}

            return 'R';
        }


    void draw(sf::RenderWindow& window) {

        pthread_mutex_lock(&ghost_mutex);//locking..
         sprite.setPosition(x, y);

        // Draw the sprite to the window
        if(frame1>0 && frame2==0){frame1--;}
        if(frame1==0 && frame2==0){ sprite.setTexture(texture2);frame2=30;}
        if(frame2>0 && frame1==0){frame2--;}
        if(frame1==0 && frame2==0){ sprite.setTexture(texture1); frame1=30;}

        window.draw(sprite);
        pthread_mutex_unlock(&ghost_mutex);//unlocking..
    }


    void exit_gbox(){

        pthread_mutex_lock(&ghost_mutex);//locking..
        if(x>650){x-=0.0001f; sleep(0.01); pthread_mutex_unlock(&ghost_mutex);//unlocking..
                            return ;}
        if(x<650){x+=0.0001f; sleep(0.01); pthread_mutex_unlock(&ghost_mutex);//unlocking..
                            return ;}

        if(y>240){y-=0.0001f; sleep(0.01); pthread_mutex_unlock(&ghost_mutex);//unlocking..
                            return ;}

        if(y<=240){exiting_gbox=false; dir = rand_dir(); pthread_mutex_unlock(&ghost_mutex);//unlocking..
                            return;}
        sleep(0.01);
        //diagnostic output..
        cout << "gbox-exit-ifs-not-checking.." << endl;
        pthread_mutex_unlock(&ghost_mutex);//unlocking..
        return;       

    }

};
