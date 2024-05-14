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
#include <iomanip>
#include <sstream>
// #include "player.h"
#include "ghost.h"

    using namespace std;

//helping mutexes
pthread_mutex_t status_mutex = PTHREAD_MUTEX_INITIALIZER ;
int gbox_exiting=0;
//helper function...
bool get_movement_access(GHOST & G , int g_id){
    if(gbox_exiting==0 and G.exiting_gbox){
        gbox_exiting=g_id;
        return true;
    }
    else if(gbox_exiting==g_id and G.exiting_gbox){
        return true;
    }
    
    if(gbox_exiting!=g_id || !G.exiting_gbox)
    { return false;}

     if(gbox_exiting==g_id || !G.exiting_gbox)
    { gbox_exiting=0; return false;}

    return false;
}

class STATUS {

public:
    int lives;
    int score;
    sf::Font font;
    sf::Text text;
    sf::Clock clock;
    
    STATUS(){
 
        //variables
        lives=3;
        score=0;

        if (!font.loadFromFile("font/Roboto-CondensedItalic.ttf")) 
        { /* error handling */ }

        
        text.setFont(font);
        text.setCharacterSize(35);
        text.setFillColor(sf::Color::White);
    }

    void display(sf::RenderWindow &window){

     pthread_mutex_lock(&status_mutex); //locking        
        text.setString(std::to_string(lives));
        text.setPosition(170, 110);
        window.draw(text);
        
        text.setString(std::to_string(score));
        text.setPosition(170, 190);
        window.draw(text);

        sf::Time elapsed = clock.getElapsedTime();
        int seconds = elapsed.asSeconds();
        int minutes = seconds / 60;
        int hours = minutes / 60;
        seconds %= 60;
        minutes %= 60;

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << hours << ":"
            << std::setw(2) << std::setfill('0') << minutes << ":"
            << std::setw(2) << std::setfill('0') << seconds;


           text.setString(oss.str());
           text.setPosition(170, 265);
         window.draw(text);

           pthread_mutex_unlock(&status_mutex); //unclocking
        return;
    }

};


