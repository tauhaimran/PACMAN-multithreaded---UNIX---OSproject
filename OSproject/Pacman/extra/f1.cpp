// Tauha Imran 22i1239 cs-g OS lab#11
#include<iostream>
#include<pthread.h>
	using namespace std;
	
pthread_t t_id,t_id_1,t_id_2;

//thread#1
void* t1(void*arg)
{
	int * nums;
	nums = (int*)arg;
	cout << "void*arg = {" << nums[0] << "," << nums[1] << "}" << endl;
	cout << "hello from the thread-1." << endl;
	pthread_t tid = pthread_self();
	cout << "thread id1 =" << tid << endl;
	return NULL;
}

//thread#2
void* t2(void*arg)
{
	int * num;
	num = (int*)arg;
	cout << "void*arg = " << num[0] << endl;
	cout << "hello from the thread-2." << endl;
	pthread_t tid = pthread_self();
	cout << "thread id2 =" << tid << endl;
	return NULL;
}

int main()
{

	int arr[]={1,2};
	int num=3;

//thread#1 working
cout << "\n-------------->>\n";
	
	/*w/out arguments*/
	//pthread_create( &t_id_1 , NULL , t1 , NULL);
	//pthread_join(t_id_1,NULL);
	
	/*with arguments*/
	pthread_create( &t_id_1 , NULL , t1 , (void *)arr);
	pthread_join(t_id_1,NULL);
	
	
//thread#2 working
cout << "\n-------------->>\n";

	/*w/out arguments*/
	//pthread_create( &t_id_2 , NULL , t2 , NULL);
	//pthread_join(t_id_2,NULL);


	/*with arguments*/
	pthread_create( &t_id_2 , NULL , t2 , &num);
	pthread_join(t_id_2,NULL);
	
//main working
cout << "\n-------------->>\n";	
	
	cout<< "hello from main..\n";
cout << "\n..................\n";	
return 0;
}

