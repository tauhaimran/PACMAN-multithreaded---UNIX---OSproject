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
    using namespace std;

pthread_mutex_t food_mutex = PTHREAD_MUTEX_INITIALIZER ;


class food_pellet {

public:
    
   sf::Texture texture;
    sf::Sprite food_bit; 
    bool eaten;

    food_pellet(){
        
        if (!texture.loadFromFile("img/food.png")) 
            {// Error handling if the image fails to load
             cout << "error loading texture from file"<<endl;
            }

        //food_bit.setPosition(600.0f,600.f);
        food_bit.setTexture(texture);
        food_bit.setScale(0.2f,0.2f);
        food_bit.setColor(sf::Color(255,255,0));
        eaten=false;
    
    }

    void drawbit( sf::RenderWindow &window){
       if(!eaten) 
        {window.draw(food_bit);}
    }

    void is_eaten()
    { eaten=true; }



};


/****************************/



class food_chain{
public:
    food_pellet eatabits[100];

    food_chain(){
        
        //row1
        eatabits[0].food_bit.setPosition(400.0f,60.f);
        eatabits[1].food_bit.setPosition(450.0f,60.f);
        eatabits[2].food_bit.setPosition(500.0f,60.f);
        eatabits[3].food_bit.setPosition(550.0f,60.f);
        eatabits[4].food_bit.setPosition(600.0f,60.f);
        eatabits[5].food_bit.setPosition(650.0f,60.f);
        eatabits[6].food_bit.setPosition(700.0f,60.f);
        eatabits[7].food_bit.setPosition(750.0f,60.f);
        eatabits[8].food_bit.setPosition(800.0f,60.f);
        eatabits[9].food_bit.setPosition(850.0f,60.f);
        eatabits[10].food_bit.setPosition(900.0f,60.f);

        //row2    
        eatabits[11].food_bit.setPosition(400.0f,150.f);
        eatabits[12].food_bit.setPosition(450.0f,150.f);
        eatabits[13].food_bit.setPosition(500.0f,150.f);
            eatabits[14].food_bit.setPosition(610.0f,150.f);
            eatabits[15].food_bit.setPosition(660.0f,150.f);
            eatabits[16].food_bit.setPosition(710.0f,150.f);
                eatabits[17].food_bit.setPosition(810.0f,150.f);
                eatabits[18].food_bit.setPosition(860.0f,150.f);
                eatabits[19].food_bit.setPosition(900.0f,150.f);

        //row3
        eatabits[20].food_bit.setPosition(400.0f,230.f);     
        eatabits[21].food_bit.setPosition(450.0f,230.f);
        eatabits[22].food_bit.setPosition(500.0f,230.f);
        eatabits[23].food_bit.setPosition(550.0f,230.f);
        eatabits[24].food_bit.setPosition(600.0f,230.f);
        eatabits[25].food_bit.setPosition(650.0f,230.f);
        eatabits[26].food_bit.setPosition(700.0f,230.f);
        eatabits[27].food_bit.setPosition(750.0f,230.f);
        eatabits[28].food_bit.setPosition(800.0f,230.f);
        eatabits[29].food_bit.setPosition(850.0f,230.f);
        eatabits[30].food_bit.setPosition(900.0f,230.f);
        
        //row4
        eatabits[31].food_bit.setPosition(400.0f,400.f);     
        eatabits[32].food_bit.setPosition(450.0f,400.f);
        eatabits[33].food_bit.setPosition(500.0f,400.f);
        eatabits[34].food_bit.setPosition(550.0f,400.f);
        eatabits[35].food_bit.setPosition(600.0f,400.f);
        eatabits[36].food_bit.setPosition(650.0f,400.f);
        eatabits[37].food_bit.setPosition(700.0f,400.f);
        eatabits[38].food_bit.setPosition(750.0f,400.f);
        eatabits[39].food_bit.setPosition(800.0f,400.f);
        eatabits[40].food_bit.setPosition(850.0f,400.f);
        eatabits[41].food_bit.setPosition(900.0f,400.f);
        //eatabits[41].food_bit.setPosition(900.0f,400.f);
 
        

        //row5

        //row6

    }

    void draw_food(sf::RenderWindow &window)
    {
        //eatabits[0].food_bit.setColor(255,255,255);
        for(int i = 0 ; i<42 ; i++)
        eatabits[i].drawbit(window);
        
    }
    


    bool if_eaten(sf::Sprite &player){

        

        return false;
    }

};