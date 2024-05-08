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
    using namespace std;

class MAP
{
public:
    sf::Texture texture;
    // Create a sprite and set its texture
    sf::Sprite sprite;
    int r ,g ,b;
    int up ;

    MAP(){
        if (!texture.loadFromFile("img/map.png")) 
        {// Error handling if the image fails to load
            cout << "error loading texture from file"<<endl;
        }
        r=0,g=0,b=255;
        up = 3;
        sprite.setTexture(texture);
        sprite.setScale(1.25f,1.2f);
        sprite.setPosition(350,1);
    }
    
    void drawmap(sf::RenderWindow &window)
    {
        sprite.setColor(sf::Color(r,g,b));

        if(up==3 & b>0){ g++; b--;}
        if(up==3 & b==0){up=2;}
        if(up==2 & g>0){ g--; b++;}
        if(up==2 & g==0){up=3;}
        //if(up==2 & g>0){ g--; r++;}
        //if(up==2 & g==0){up=1;}
        //if(up==1 & r>0){ r--; b++;}
        //if(up==1 & r==0){up=3;}


      window.draw(sprite);  
    }

};

class PACMAN{

private:
    sf::Texture texture1;
    sf::Texture texture2;
    sf::Sprite sprite;
    int frame1,frame2;
    float x, y;
    float speed;

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
        sprite.setScale(0.09f,0.09f);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
        frame1 = 10;
        frame2 = 0;
        // Set the initial position of the sprite
        sprite.setPosition(x, y);
    }

    void move(char dr,sf::Sprite& maze) {
        
        int oldx = x , oldy=y ;

        // Update the position of the sprite
        if(dr=='R'){ x++; sprite.rotate(180);}//Right
        if(dr=='L'){ x--; sprite.rotate(0);}//Left
        if(dr=='U'){ y--; sprite.rotate(270);}//Up
        if(dr=='D'){ y++; sprite.rotate(90);}//Down
        
        sprite.setPosition(x, y);

        //if colliding wiht the maze...
        if(sprite.getGlobalBounds().intersects(maze.getGlobalBounds())){
            sprite.setPosition(oldx, oldy);
        }

    }

    void draw(sf::RenderWindow& window) {
        // Draw the sprite to the window
        if(frame1>0 && frame2==0){frame1--;}
        if(frame1==0 && frame2==0){ sprite.setTexture(texture2);frame2=10;}
        if(frame2>0 && frame1==0){frame2--;}
        if(frame1==0 && frame2==0){ sprite.setTexture(texture1); frame1=10;}

        window.draw(sprite);
    }

};