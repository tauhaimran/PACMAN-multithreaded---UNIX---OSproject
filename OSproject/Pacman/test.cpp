/**Tauha Imram 22i1239 - cs - g - lab13*/
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
//#include <mutex.h>
	using namespace std;

pthread_t t1;
sem_t producer ,consumer, count_check;
int count = 50;


//my thread
void* function(void*arg){ // this is our consumer

		
	while(true){
		sem_wait(&consumer) ; //put consumer to sleep 

		
		if(count>5){
		
			count-=5;
			cout << "*\nA person take 5 liters " << endl;
			cout << "water left : " << count << endl;
			sleep(2);
		}else {cout << "*\nNO WATER AVAILABLE" << endl; sleep(2);}
		
		sem_post(&producer); // wake up  the producer

	 }
	pthread_exit(NULL);


}

int main(){

	//initalize the semaphores
	sem_init(&producer , 0 , 0 ) ;
	sem_init(&count_check , 0 , 1 ) ;
	sem_init(&consumer , 0 , 3 ) ;

cout << "INITIAL STATE"	<< endl;
cout << "water filter : " <<  count << "litres" << endl;
cout << "**********" << endl;

	//making the thread..	
	for(int i=0 ; i<5 ; i++)
	{pthread_create(&t1,NULL,function,NULL);}
	
	//our producer...
	while(true){
		sem_wait(&producer); // put the producer to sleep  

		cout << "----------------------"<< endl;
		if(count < 100)		
		count+=10;
		cout << "water filter : " <<  count << "litres" << endl;


		sem_post(&consumer); // put wake up the consumer
		
	
	}
	
pthread_exit(NULL);
	
}