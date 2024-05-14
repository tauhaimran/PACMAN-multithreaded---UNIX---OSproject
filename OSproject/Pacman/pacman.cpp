
//Tauha imran 22i1239-cs-g OS project
#include <SFML/Graphics.hpp>
#include<pthread.h>
#include<string.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>
#include "phase1.h"
    using namespace std;

pthread_mutex_t main_mutex = PTHREAD_MUTEX_INITIALIZER ;
pthread_mutex_t bakery_lock_mutex = PTHREAD_MUTEX_INITIALIZER ;

int thread_tickets[6]={0,1,1,1,1,1};
// 0 - ui/ux
// 1,2,3,4 - ghosts
// 5 - game_engine

void lock_resource( int thread_no ){

 
        
        //if resource locked
        while(thread_tickets[thread_no]==1){
            //busy wait loop loop;
        }
   
     pthread_mutex_lock(&bakery_lock_mutex);
     pthread_mutex_lock(&main_mutex);
        //if resource not locked
        for (int i = 0 ; i<6 ; i++){
            thread_tickets[i]=1;
        }
        
        thread_tickets[thread_no]=0;

    pthread_mutex_lock(&bakery_lock_mutex);
    return;

}

void unlock_resource( int thread_no ){

  pthread_mutex_lock(&bakery_lock_mutex);
            //if resource not locked
        for (int i = 0 ; i<6 ; i++){
            thread_tickets[i]=0;
        }
  pthread_mutex_lock(&bakery_lock_mutex);
  return;

}



int count = 0;
//named semaphores
sem_t SEM_GAME_ENGINE;
sem_t SEM_UI_THREAD;
sem_t SEM_GHOST_THREAD;
sem_t SEM_SC3_GBOX;
sem_t bakery_lock;



//making the threads
pthread_t game_engine_id,ui_ux_id_1,pac_id;
pthread_t g1,g2,g3,g4;	

//global variables for the threads...
bool game_running = true;
bool paused=false;
int gbox_exiting=0;


//globl resources...
MAP mapX;
STATUS status;
PACMAN pacman(670,605,1.0f);
GHOST ghost1(610, 310,5.0f);
GHOST ghost2(640, 330,5.0f);
GHOST ghost3(670, 310,5.0f);
GHOST ghost4(700, 330,5.0f);

food_chain eatabits;

//thread 1 - game engine
void *game_engine(void*arg){
    
    sf::RenderWindow* window = (sf::RenderWindow*)arg;
    int x=650.0 , y=250.0;
    char dir = 'L';


        while (window->isOpen() or true)
        { //consumer consumes
            sem_wait(&SEM_GAME_ENGINE);
            pthread_mutex_lock(&main_mutex);
            //lock_resource(5);//game-engine->5
             //sem_wait(&SEM_GHOST_THREAD); //2->1
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

            if(ghost1.ghost_hit(pacman.sprite)){
                //pthread_mutex_lock(&main_mutex);//unlocking..
                status.lives--;
                pacman.reset();
                sleep(2);
                //pthread_mutex_unlock(&main_mutex);//unlocking..
            }  

            if(ghost2.ghost_hit(pacman.sprite)){
                //pthread_mutex_lock(&main_mutex);//unlocking..
                status.lives--;
                pacman.reset();
                sleep(2);
                //pthread_mutex_unlock(&main_mutex);//unlocking..
            }  

            if(ghost3.ghost_hit(pacman.sprite)){
                //pthread_mutex_lock(&main_mutex);//unlocking..
                status.lives--;
                pacman.reset();
                sleep(2);
                //pthread_mutex_unlock(&main_mutex);//unlocking..
            }   

            if(ghost4.ghost_hit(pacman.sprite)){
                //pthread_mutex_lock(&main_mutex);//unlocking..
                status.lives--;
                pacman.reset();
                sleep(2);
                //pthread_mutex_unlock(&main_mutex);//unlocking..
            }   
           
            //wake up producer
            //unlock_resource(5);//game-engine->5
            pthread_mutex_unlock(&main_mutex);
            sem_post(&SEM_UI_THREAD);
        }
    pthread_exit(NULL);
}

/**********************************************11111111111111111111111111111111111********************************/
/**GHOST THREADS 1-3*/
void *ghost_one(void*arg){
    sf::RenderWindow* window = (sf::RenderWindow*)arg;
    std::cout<<"aoun jee gay"<<endl;
    int x=650.0 , y=605.0;

        while (window->isOpen() or true)
        { //consumer consumes
         //   ghostObj.move('L', mapX.sprite);
         //if(choosing[T_ticket]){   choosing[T_ticket]= ghostObj. }
          sem_wait(&SEM_GHOST_THREAD); //1->0
         // sem_wait(&SEM_GAME_ENGINE);

            //if ghost already exiting sem_check
            //lock_resource(4);//ghost1->4
            pthread_mutex_lock(&main_mutex);

            if(ghost1.exiting_gbox){
                //sem_wait(&SEM_SC3_GBOX); //1->0
                ghost1.exit_gbox();
                gbox_exiting=1;
                //if(!ghost1.exiting_gbox)
                //{sem_post(&SEM_SC3_GBOX);};

            }
            else {
                ghost1.move(pacman.sprite,mapX);
            }
            
            //unlock_resource(4);//ghost1->4
            pthread_mutex_unlock(&main_mutex);
            sem_post(&SEM_GHOST_THREAD);//0->1

         //sem_post(&SEM_UI_THREAD);
        }
    return NULL;
}

/**********************************************22222222222222222222222222222222222222********************************/

void *ghost_two(void*arg){
    sf::RenderWindow* window = (sf::RenderWindow*)arg;
    std::cout<<"aoun jee gay"<<endl;
    int x=650.0 , y=605.0;
  



        while (window->isOpen() or true)
        { //consumer consumes
         //   ghostObj.move('L', mapX.sprite);
         //if(choosing[T_ticket]){   choosing[T_ticket]= ghostObj. }
          sem_wait(&SEM_GHOST_THREAD); //1->0
          pthread_mutex_lock(&main_mutex);
          
          //lock_resource(3);//ghost2->3
            //if ghost already exiting sem_check
            
            if(ghost2.exiting_gbox){
            //sem_wait(&SEM_SC3_GBOX); //1->0
                ghost2.exit_gbox();
              //  if(!ghost2.exiting_gbox)
                 // {sem_post(&SEM_SC3_GBOX);};
            }
            else {
                
                ghost2.move(pacman.sprite,mapX);
            }

            //unlock_resource(3);//ghost2->3
            pthread_mutex_unlock(&main_mutex);
            sem_post(&SEM_GHOST_THREAD);//0->1


        }
    return NULL;
}

/**********************************************33333333333333333333333333333333333333********************************/

void *ghost_three(void*arg){
    sf::RenderWindow* window = (sf::RenderWindow*)arg;
    std::cout<<"aoun jee gay"<<endl;
    int x=650.0 , y=605.0;
  



        while (window->isOpen() or true)
        { //consumer consumes
         //   ghostObj.move('L', mapX.sprite);
         //if(choosing[T_ticket]){   choosing[T_ticket]= ghostObj. }
          sem_wait(&SEM_GHOST_THREAD); //1->0
          
          //lock_resource(2);//ghost3->2
          pthread_mutex_lock(&main_mutex);

            //if ghost already exiting sem_check
            
            if(ghost3.exiting_gbox){
            //sem_wait(&SEM_SC3_GBOX); //1->0
                ghost3.exit_gbox();
              //  if(!ghost2.exiting_gbox)
                 // {sem_post(&SEM_SC3_GBOX);};
            }
            else {
                
                ghost3.move(pacman.sprite,mapX);
            }
            
            //unlock_resource(3);//ghost3->2
            pthread_mutex_unlock(&main_mutex);
            sem_post(&SEM_GHOST_THREAD);//0->1


        }
    return NULL;
}

/**********************************************44444444444444444444444444444444444444********************************/

void *ghost_four(void*arg){
    sf::RenderWindow* window = (sf::RenderWindow*)arg;
    std::cout<<"aoun jee gay"<<endl;
    int x=650.0 , y=605.0;
  



        while (window->isOpen() or true)
        { //consumer consumes
         //   ghostObj.move('L', mapX.sprite);
         //if(choosing[T_ticket]){   choosing[T_ticket]= ghostObj. }
          sem_wait(&SEM_GHOST_THREAD); //1->0
          //lock_resource(1);//ghost4->1
            //if ghost already exiting sem_check

            pthread_mutex_lock(&main_mutex);
            
            if(ghost4.exiting_gbox){
            //sem_wait(&SEM_SC3_GBOX); //1->0
                ghost4.exit_gbox();
              //  if(!ghost2.exiting_gbox)
                 // {sem_post(&SEM_SC3_GBOX);};
            }
            else {
                
                ghost4.move(pacman.sprite,mapX);
            }
            //unlock_resource(1);//ghost4->1
            pthread_mutex_unlock(&main_mutex);
            sem_post(&SEM_GHOST_THREAD);//0->1
            

        }
    return NULL;
}

/**********************************************MMMMMMMMMMMMMMMMMMMMMMMMMMMM********************************/

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
    sem_init(&SEM_GHOST_THREAD,0,2); //initial value 3 - counting semaphore
    sem_init(&SEM_SC3_GBOX,0,1); //this is for scenario
    sem_init(&bakery_lock,0,0); 
    //creating semaphores
    //sem_t* sem_game_engine = sem_open(SEM_GAME_ENGINE, IPC_CREAT , 0660 , 0);



    sf::RenderWindow window(sf::VideoMode(1300,670), "PACAMAN | OS-project fall 24 | 22i1239 & 22i1335 " );
    sf::RenderWindow* xwindow = &window;
    window.setPosition(sf::Vector2i(20,40));
    window.clear(sf::Color(200, 200, 200)); // Light gray color
    
    //getting the window in a thread...
    pthread_create( &game_engine_id, NULL, game_engine , (void*)xwindow );
    
    //our ghost thread...
    pthread_create( &g1, NULL, ghost_one   , (void*)xwindow );
    pthread_create( &g2, NULL, ghost_two   , (void*)xwindow );
    pthread_create( &g3, NULL, ghost_three , (void*)xwindow );
    pthread_create( &g4, NULL, ghost_four  , (void*)xwindow );

    

  
    while (window.isOpen())
    {   //producer
        sem_wait(&SEM_UI_THREAD); //this makes game engine wait
         
        //lock_resource(0); //ui/ux -> 0
        //cout << "Producer\n";
        pthread_mutex_lock(&main_mutex);
        
        window.clear(); // Light gray color


        //ghost.draw(window);
        ghost1.draw(window);
        ghost2.draw(window);
        ghost3.draw(window);
        ghost4.draw(window);
        
        window.draw(right_panel);
        window.draw(left_panel);
        status.display(window);

        mapX.drawmap(window);
        eatabits.draw_food(window);
        
        pacman.draw(window);

        
         
        window.display();  
        
        //lock_resource(0); //ui/ux -> 0

        //wake up consumer
        pthread_mutex_unlock(&main_mutex);
        sem_post(&SEM_GAME_ENGINE); //lets ui thread run
    }

    //closing the semaphores
    sem_destroy(&SEM_GAME_ENGINE);
    sem_destroy(&SEM_UI_THREAD);
    sem_destroy(&SEM_GHOST_THREAD);

   pthread_exit(NULL);
   // return 0;
}
