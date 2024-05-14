#include <SFML/Graphics.hpp>
#include<pthread.h>
#include<string.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include <signal.h>
#include <iostream>
#include <fstream>
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

    void move(sf::Sprite & player, MAP &mapX) {
        

         pthread_mutex_lock(&ghost_mutex);//locking..
        float px = player.getPosition().x , py=player.getPosition().y ;

        if(x>px){ dir = 'R' ;}
        if(x<px){ dir = 'L' ;}
        if(y>py){ dir = 'D' ;}
        if(y>py){ dir = 'U' ;}

	
        // Update the position of the sprite
        

        if(dir=='R'){ x+=0.01; angle = 0;}//Right
        if(dir=='L'){ x-=0.01; angle = 180;}//Left
        if(dir=='U'){ y-=0.01; angle = 270;}//Up
        if(dir=='D'){ y+=0.01; angle = 90;}//Down
        
        
        sprite.setRotation(angle);
        sprite.setPosition(x, y);

        //if colliding wiht the maze...
        if(mapX.internal_collision(sprite) || !(sprite.getGlobalBounds().intersects(mapX.background.getGlobalBounds())) )
        {
            //sprite.setRotation(0);
            if(dir=='R'){ x-=0.1; angle = 0;  dir = 'D';}//Right
		    if(dir=='L'){ x+=0.1; angle = 180; dir = 'U';}//Left
		    if(dir=='U'){ y+=0.1; angle = 270; dir = 'R';}//Up
		    if(dir=='D'){ y-=0.1; angle = 90;  dir = 'L';}//down
            cout << "colliding";
        }

        pthread_mutex_unlock(&ghost_mutex);//unlocking..
        return;

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
        if(x>650){x-=0.2f;  pthread_mutex_unlock(&ghost_mutex);//unlocking..
                            return ;}
        if(x<650){x+=0.2f;  pthread_mutex_unlock(&ghost_mutex);//unlocking..
                            return ;}

        if(y>240){y-=0.2f; pthread_mutex_unlock(&ghost_mutex);//unlocking..
                            return ;}

        if(y==240){exiting_gbox=false; pthread_mutex_unlock(&ghost_mutex);//unlocking..
                                        return;}
       

    }

};
