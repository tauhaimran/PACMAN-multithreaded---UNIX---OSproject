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
#include "map.h"
#include "food.h"
    using namespace std;
pthread_mutex_t logic_mutex = PTHREAD_MUTEX_INITIALIZER ;
class PACMAN{

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
    PACMAN(float startX, float startY, float movementSpeed) 
        : x(startX), y(startY), speed(movementSpeed)
    {
        // Load the Pacman texture
        if (!texture1.loadFromFile("img/pac1.png") && !texture2.loadFromFile("img/pac2.png")) {
            // Error handling if the image fails to load
            throw std::runtime_error("Failed to load Pacman texture.");
        }
        texture2.loadFromFile("img/pac2.png");
        // Set the texture to the sprite
        sprite.setTexture(texture1);
        sprite.setScale(0.075f,0.075f);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
        frame1 = 10;
        frame2 = 0;
        // Set the initial position of the sprite
        sprite.setPosition(x, y);
        //sprite.setScale(0.1f, 0.1f);
    }

    void move(char dr, MAP &map) {
        
        float oldx = x , oldy=y ;
	
        
        
       
            char xdr = dr;
            float x_error = 0.0;
            float y_error = 0.0;
                if(dr=='R'){ x+=1; angle = 0;}//Right
                if(dr=='L'){ x-=1; angle = 180;}//Left
                if(dr=='U'){ y-=1; angle = 270;}//Up
                if(dr=='D'){ y+=1; angle = 90;}//Down
                
                sprite.setPosition(x, y);
                sprite.setRotation(angle);
            
            //if colliding with the maze...
            pthread_mutex_lock(&logic_mutex);//locking..
            if(!(sprite.getGlobalBounds().intersects(map.background.getGlobalBounds()))
                    || (sprite.getGlobalBounds().intersects(map.gbox.getGlobalBounds())) || map.internal_collision(sprite))
                 {
                        
                    // Update the position of the sprite
                    if(dr=='R'){ x-=1; angle = 0;}//Right
                    if(dr=='L'){ x+=1; angle = 180;}//Left
                    if(dr=='U'){ y+=1; angle = 270;}//Up
                    if(dr=='D'){ y-=1; angle = 90;}//Down
                     sprite.setPosition(x, y);
                     sprite.setRotation(angle);
                }
        pthread_mutex_unlock(&logic_mutex); //unclocking
        //cout << "x - " << x << "-" << y << endl;
            


    }

    void draw(sf::RenderWindow& window) {
        // Draw the sprite to the window
        if(frame1>0 && frame2==0){frame1--;}
        if(frame1==0 && frame2==0){ sprite.setTexture(texture2);frame2=50;}
        if(frame2>0 && frame1==0){frame2--;}
        if(frame1==0 && frame2==0){ sprite.setTexture(texture1); frame1=50;}

        window.draw(sprite);
    }

};
