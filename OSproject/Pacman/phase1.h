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
#include "player.h"
    using namespace std;

//helping mutexes



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
    GHOST(float startX, float startY, float movementSpeed) 
        : x(startX), y(startY), speed(movementSpeed)
    {
        // Load the Pacman texture
        if (!texture1.loadFromFile("img/ghost.png") && !texture2.loadFromFile("img/ghost1.png")) {
            // Error handling if the image fails to load
            throw std::runtime_error("Failed to load Pacman texture.");
        }
        texture2.loadFromFile("img/ghost1.png");
        // Set the texture to the sprite
        sprite.setTexture(texture1);
        sprite.setScale(0.09f,0.09f);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
        frame1 = 10;
        frame2 = 0;
        // Set the initial position of the sprite
        sprite.setPosition(x, y);
        sprite.setScale(0.1f, 0.1f);
    }

    void move(char dr,sf::Sprite& maze) {
        
        float oldx = x , oldy=y ;
	
        // Update the position of the sprite
        if(dr=='R'){ x+=100.0; angle = 0;}//Right
        if(dr=='L'){ x-=100.0; angle = 180;}//Left
        if(dr=='U'){ y-=100.0; angle = 270;}//Up
        if(dr=='D'){ y+=100.0; angle = 90;}//Down
        
        
        dir = dr;
        sprite.setRotation(angle);
        sprite.setPosition(x, y);

        //if colliding wiht the maze...
        if(sprite.getGlobalBounds().intersects(maze.getGlobalBounds()))
        {
            //sprite.setRotation(0);
            //if(dr=='R'){ x-=0.1; angle = 0;}//Right
		//if(dr=='L'){ x+=0.1; angle = 180;}//Left
		//if(dr=='U'){ y+=0.1; angle = 270;}//Up
		//if(dr=='D'){ y-=0.1; angle = 90;}
        }

    }

    void draw(sf::RenderWindow& window) {
        // Draw the sprite to the window
        if(frame1>0 && frame2==0){frame1--;}
        if(frame1==0 && frame2==0){ sprite.setTexture(texture2);frame2=5;}
        if(frame2>0 && frame1==0){frame2--;}
        if(frame1==0 && frame2==0){ sprite.setTexture(texture1); frame1=5;}

        window.draw(sprite);
    }

};
