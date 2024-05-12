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

pthread_mutex_t render_mutex = PTHREAD_MUTEX_INITIALIZER ;

/**********************************************************************/

class wall_segment {

    int r ,g ,b;
    int up ;
    int move;
    bool right;
    
public:
    sf::Texture texture;
    sf::Sprite wall; 
    bool horizontal;
    wall_segment(){

              if (!texture.loadFromFile("img/tota.png")) 
            {// Error handling if the image fails to load
             cout << "error loading texture from file"<<endl;
            }
             r=0,g=0,b=255;
             up = 3;
        wall.setTexture(texture);
        wall.setScale(1.1f,1.0f);
        wall.setOrigin(wall.getLocalBounds().width / 2.0f, wall.getLocalBounds().height / 2.0f);
     
        horizontal=true;
        set_move();
    }

    //for the shakyyy
    void set_move(int x=0)
    { if(x==0){move = 0; right=true;}
        else {move = 10; right=false;} };
    //makes the tota go shakey shakey
    void tota_oscilate(){
    }

    //shows the tota with color changings
    void draw_tota(sf::RenderWindow &window)
    {
        wall.setColor(sf::Color(r,g,b));

          sf::Vector2f spritePosition = wall.getPosition();

        // Store x and y coordinates in separate variables
        float xPos = spritePosition.x;
        float yPos = spritePosition.y;

        if(move<60 && right){ move++; 
            if(horizontal){xPos+=0.5f;}//move right
            else {yPos+=0.5f;} }//or down 
        if(move>0 && !right){ move--; 
            if(horizontal){xPos-=0.5f;}//move rleft
            else {yPos-=0.5f;} }//or up 
        if(move==30 && right){ right = false;}//change to lefting
        if(move==0 && !right){ right = true;}//change to righting
        
        wall.setPosition(xPos,yPos);

        if(up==3 & b>0){ g++; b--;}
        if(up==3 & b==0){up=2;}
        if(up==2 & g>0){ g--; b++;}
        if(up==2 & g==0){up=3;}
        //if(up==2 & g>0){ g--; r++;}
        //if(up==2 & g==0){up=1;}
        //if(up==1 & r>0){ r--; b++;}
        //if(up==1 & r==0){up=3;}

      window.draw(wall);  
        
    }
};


/**********************************************************************/

class MAP
{
public:
    sf::Texture texture , box;
    // Create a sprite and set its texture
    sf::Sprite border;
    sf::Sprite gbox;
    sf::RectangleShape background;
    int r ,g ,b;
    int up ;
    wall_segment totas[12];
    wall_segment topas[6];
    

    MAP(){
        
        if (!texture.loadFromFile("img/border.png")) 
        {// Error handling if the image fails to load
            cout << "error loading texture from file"<<endl;
        }
         if (!box.loadFromFile("img/gbox.png")) 
        {// Error handling if the image fails to load
            cout << "error loading texture from file"<<endl;
        }

         r=0,g=0,b=255;
         up = 3;
         

        //horizontal walls 
        totas[0].wall.setPosition(450,100);
        totas[1].wall.setPosition(650,100); totas[1].set_move(1);
        totas[2].wall.setPosition(850,100);

        totas[3].wall.setPosition(450,200);totas[3].set_move(1);
        totas[4].wall.setPosition(650,200); 
        totas[5].wall.setPosition(850,200);totas[5].set_move(1);

        totas[6].wall.setPosition(450,450);
        totas[7].wall.setPosition(650,450); totas[7].set_move(1);
        totas[8].wall.setPosition(850,450);

        totas[9].wall.setPosition(450,550);totas[9].set_move(1);
        totas[10].wall.setPosition(650,550); 
        totas[11].wall.setPosition(850,550);totas[11].set_move(1);

        //vertical walls
        for(int i = 0 ; i<6 ; i++ ){topas[i].horizontal=false; topas[i].wall.setRotation(90);}
        
        topas[0].wall.setPosition(570,150);
        topas[1].wall.setPosition(770,150); 
        topas[2].wall.setPosition(500,300); topas[2].set_move(1);
        topas[3].wall.setPosition(800,300); topas[3].set_move(1);
        topas[4].wall.setPosition(570,470);
        topas[5].wall.setPosition(770,470); 
        
        //other stuff
        background.setFillColor(sf::Color(255, 255, 255, 1));
        background.setSize(sf::Vector2f(520,520));
        background.setOrigin(background.getLocalBounds().width / 2.0f, background.getLocalBounds().height / 2.0f);
        background.setPosition(647,338);
        


        gbox.setTexture(box);
        gbox.setOrigin(gbox.getLocalBounds().width / 2.0f, gbox.getLocalBounds().height / 2.0f);
         gbox.setScale(1.25f,1.25f);
        gbox.setPosition(650,320);

         border.setTexture(texture);
            border.setScale(1.25f,1.25f);
            border.setPosition(337,27);

    }
    
    void drawmap(sf::RenderWindow &window){

       border.setColor(sf::Color(r,g,b));
       gbox.setColor(sf::Color(r,g,b));

        if(up==3 & b>0){ g++; b--;}
        if(up==3 & b==0){up=2;}
        if(up==2 & g>0){ g--; b++;}
        if(up==2 & g==0){up=3;}
        
        pthread_mutex_lock(&render_mutex);//locking..
        for(int i = 0 ; i<12 ; i++){ totas[i].draw_tota(window);}
        for(int i = 0 ; i<6  ; i++){ topas[i].draw_tota(window);}
        window.draw(border);
        window.draw(background);
        window.draw(gbox);
        pthread_mutex_unlock(&render_mutex); //unclocking
    }

    bool internal_collision(sf::Sprite &player ){

        pthread_mutex_lock(&render_mutex);//locking..
        for(int i = 0 ; i<12 ; i++){
             if(player.getGlobalBounds().intersects(totas[i].wall.getGlobalBounds()))
             { pthread_mutex_unlock(&render_mutex); //unclocking
              return true;}
             }
        
        for(int i = 0 ; i<6  ; i++){ 
            if(player.getGlobalBounds().intersects(topas[i].wall.getGlobalBounds()))
             { pthread_mutex_unlock(&render_mutex); //unclocking 
              return true;}
            }
        pthread_mutex_unlock(&render_mutex); //unclocking
         return false;
        
    }


};


