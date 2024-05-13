
//Tauha imran 22i1239-cs-g OS project
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
#include "phase1.h"
    using namespace std;

int count = 0;
//named semaphores
sem_t SEM_GAME_ENGINE;
sem_t SEM_UI_THREAD;

//making the threads
pthread_t game_engine_id,ui_ux_id_1,pac_id;
pthread_t red_id,blue_id,yellow_id,pink_id;	

//global variables for the threads...
bool game_running = true;
bool paused=false;
//globl resources...
MAP mapX;
STATUS status;
PACMAN pacman(670,605,1.0f);
GHOST ghostObj (660, 610,5.0f);
food_chain eatabits;

//thread 1 - game engine
void *game_engine(void*arg){
    sf::RenderWindow* window = (sf::RenderWindow*)arg;
    int x=650.0 , y=605.0;
    char dir = 'L';

        while (window->isOpen() or true)
        { //consumer consumes
            sem_wait(&SEM_GAME_ENGINE);
            sf::Event event;
            while (window->pollEvent(event))
            {
                if(event.type == sf::Event::Closed){
                    window->close();
                    pthread_exit(NULL);
                }

                 // Check for keyboard input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    // Handle left arrow key press
                    dir='L';
                    
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    // Handle right arrow key press
                    dir='R';
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    // Handle up arrow key press
                     dir='U';
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    // Handle down arrow key press
                    dir='D';
                }

                


            }
            pacman.move(dir,mapX); //moving pacman

            //checking food +  updating score
            if(eatabits.if_eaten(pacman.sprite))
            {status.score+=10;}
            //cout << "plz work\n";
           
            //wake up producer
            sem_post(&SEM_UI_THREAD);
        }
    pthread_exit(NULL);
}

void *ghost(void*arg){
    sf::RenderWindow* window = (sf::RenderWindow*)arg;
    std::cout<<"aoun jee gay"<<endl;
    int x=650.0 , y=605.0;
    char dir = 'L';

        while (window->isOpen() or true)
        { //consumer consumes
         //   ghostObj.move('L', mapX.sprite);
        }
    return NULL;
}
/*MAIN FUNCTION*/
int main(int argc, char const *argv[])
{

    sf::Sprite right_panel;
    sf::Texture right_texture;
    sf::Sprite left_panel;
    sf::Texture left_texture;

     if (!right_texture.loadFromFile("img/1.png")) {
            // Error handling if the image fails to load
            throw std::runtime_error("Failed to load Pacman texture.");
        }
    
    if (!left_texture.loadFromFile("img/2.png")) {
            // Error handling if the image fails to load
            throw std::runtime_error("Failed to load Pacman texture.");
        }

        right_panel.setTexture(right_texture);
        right_panel.setPosition(975,10);
        right_panel.setScale(0.32f,0.32f);

        left_panel.setTexture(left_texture);
        left_panel.setPosition(5,25);
        left_panel.setScale(0.32f,0.32f);


    //unlinking all the previous semaphores
    sem_init(&SEM_GAME_ENGINE,0,0); //initial value 0 - will wait
    sem_init(&SEM_UI_THREAD,0,1); //initial value 1 - will run first

    //creating semaphores
    //sem_t* sem_game_engine = sem_open(SEM_GAME_ENGINE, IPC_CREAT , 0660 , 0);



    sf::RenderWindow window(sf::VideoMode(1300,670), "PACAMAN | OS-project fall 24 | 22i1239 & 22i1335 " );
    sf::RenderWindow* xwindow = &window;
    window.setPosition(sf::Vector2i(20,40));
    window.clear(sf::Color(200, 200, 200)); // Light gray color
    //getting the window in a thread...
    pthread_create( &game_engine_id, NULL, game_engine , (void*)xwindow );
    pthread_create( &red_id, NULL, ghost , (void*)xwindow );
    //pthread_join(game_engine_id,NULL);

  
    while (window.isOpen())
    {   //producer
        sem_wait(&SEM_UI_THREAD); //this makes game engine wait
        
        //cout << "Producer\n";
        
        window.clear(); // Light gray color


        //ghost.draw(window);
        //ghostObj.draw(window);
        
        window.draw(right_panel);
        window.draw(left_panel);
        status.display(window);

        mapX.drawmap(window);
        eatabits.draw_food(window);
        
        pacman.draw(window);

    
         
        window.display();  
        
        //wake up consumer
        sem_post(&SEM_GAME_ENGINE); //lets ui thread run
    }

    //closing the semaphores
    sem_destroy(&SEM_GAME_ENGINE);
    sem_destroy(&SEM_UI_THREAD);

   pthread_exit(NULL);
   // return 0;
}
