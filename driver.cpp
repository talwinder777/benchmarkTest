/*
@author: Varun Shanbhag (https://github.com/Varun-Shanbhag-007/),
         Tushar Nitave (https://github.com/tushar-nitave),
         Talwinder Singh ()

Date: March 31 2020

Course: Cloud Computing CS553 Spring 20

Task: Performing MyDiskBench benchmark of the storage systems using C++.
*/


#include<stdio.h>
#include<iostream>
#include <chrono>
#include <cstring>
#include <fstream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
#include<sstream>
using namespace std;
using namespace std::chrono;

// Utiliy Method Declaration
void write(int files, int record);

void read(int files, int record);

void write_random(int files, int record);

void read_random(int files, int record);

//This is thread struct
struct thread_data {
    int id;
    long long int rec;
    long long int fileSize;
};

//Write Sequential Method Helper run by every spawned thread
void *WriteHelper(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    //Values fetched from thread struct
    int threadid = my_data->id;
    long long int size = my_data->fileSize;
    long long int recordSize = my_data->rec;

    stringstream temp_str;
    temp_str<<(threadid);
    string str = temp_str.str();
    const char* cstr2 = str.c_str();

    char *buf1 = (char *)malloc(recordSize*sizeof(char));
    void *buffer;

    long long int i =0;
    //create a record with given record size
    for(i =0;i<recordSize;i++){

        buf1[i] = 'a';
    }

    buf1[i] = '\0';

    //Assign memory analogs to record size which is in multiples of 512
    posix_memalign(&buffer,recordSize,recordSize);

    memcpy(buffer, buf1,recordSize*sizeof(char));

    long long int hops=size/recordSize; //10GB/1M for 1 thread

    /*
     * O_CREAT  -- Creates new file if the file does not exists
     * O_TRUNC  -- Truncates the pre-existing file
     * O_DIRECT -- Minimises the cache effect of IO
     * O_WRONLY -- Open the file in write-only mode
     * S_IRWXU  -- File permissions of the created file
     */
    int fileDescriptor = open(cstr2, O_CREAT|O_TRUNC|O_DIRECT|O_WRONLY, S_IRWXU);

    // Writing file sequentially
    for(int i = 0; i < hops ;i++){
        write(fileDescriptor, buffer, recordSize);
    }

    //Close the file and corresponding buffers
    close(fileDescriptor);
    free(buffer);
    free(buf1);

    pthread_exit(NULL);
}

void *WriteRandomHelper(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    //Values fetched from thread struct
    long long int rand_num = 0;
    int threadid = my_data->id;
    long long int size = my_data->fileSize;
    long long int recordSize = my_data->rec;

    stringstream temp_str;
    temp_str<<(threadid);
    string str = temp_str.str();
    const char* cstr2 = str.c_str();

    char *buf1 = (char *)malloc(recordSize*sizeof(char));
    void *buffer;

    long long int i =0;

    //create a record with given record size
    for(i =0;i<recordSize;i++){

        buf1[i] = 'a';
    }

    buf1[i] = '\0';

    //Assign memory analogs to record size which is in multiples of 512
    posix_memalign(&buffer, recordSize, recordSize);

    memcpy(buffer, buf1,recordSize*sizeof(char));

    long long int hops=size/recordSize; //10GB/1M for 1 thread

    /*
     * O_CREAT  -- Creates new file if the file does not exists
     * O_TRUNC  -- Truncates the pre-existing file
     * O_DIRECT -- Minimises the cache effect of IO
     * O_WRONLY -- Open the file in write-only mode
     * S_IRWXU  -- File permissions of the created file
     */
    int fileDescriptor = open(cstr2, O_CREAT|O_TRUNC|O_DIRECT|O_WRONLY, S_IRWXU);

    long long int true_random = 0;

    for(int i = 0; i < hops ;i++){
        rand_num = (rand()%(size-recordSize));
        //we do this step to ensure that generated random number is multiple of record size
        // as O_DIRECT requires strict memory alignment and this is how we ensure it's achieved
        true_random = (rand_num/recordSize) * recordSize;
        //lseek is  used to randomly seek and reposition the file descriptor to random location in file
        lseek(fileDescriptor, true_random, SEEK_SET);
        write(fileDescriptor, buffer, recordSize);
    }

    //Close the file and corresponding buffers
    close(fileDescriptor);
    free(buffer);
    free(buf1);

    pthread_exit(NULL);
}


void *ReadHelper(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    //Values fetched from thread struct
    int threadid = my_data->id;
    long long int size = my_data->fileSize;
    long long int recordSize = my_data->rec;

    stringstream temp_str;
    temp_str<<(threadid);
    string str = temp_str.str();
    const char* cstr2 = str.c_str();

    void *buffer;

    //Assign memory analogs to record size which is in multiples of 512
    posix_memalign(&buffer,recordSize,recordSize);

    long long int hops=size/recordSize; //10GB/1M for 1 thread

    /*
    * O_DIRECT -- Minimises the cache effect of IO
    * O_RDONLY -- Open the file in read-only mode
    * S_IRWXU  -- File permissions of the created file
    */
    int fileDescriptor = open(cstr2,O_DIRECT|O_RDONLY, S_IRWXU);

    //read file in chunks of record size sequentially.
    for(int i = 0; i < hops ;i++){
        read(fileDescriptor, buffer, recordSize);
    }

    //Close the file and corresponding buffers
    close(fileDescriptor);
    free(buffer);

    pthread_exit(NULL);
}


void *ReadRandomHelper(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    //Values fetched from thread struct
    long long int rand_num = 0;
    int threadid = my_data->id;
    long long int size = my_data->fileSize;
    long long int recordSize = my_data->rec;

    stringstream temp_str;
    temp_str<<(threadid);
    string str = temp_str.str();
    const char* cstr2 = str.c_str();

    void *buffer;

    //Assign memory analogs to record size which is in multiples of 512
    posix_memalign(&buffer,recordSize,recordSize);

    long long int hops=size/recordSize; //10GB/1M for 1 thread

    /*
   * O_DIRECT -- Minimises the cache effect of IO
   * O_RDONLY -- Open the file in read-only mode
   * S_IRWXU  -- File permissions of the created file
   */
    int fileDescriptor = open(cstr2,O_DIRECT|O_RDONLY, S_IRWXU);

    //read file in chunks of record size randomly.
    for(int i = 0; i < hops ;i++){
        rand_num = rand()%(size-recordSize);
        //lseek is  used to randomly seek and reposition the file descriptor to random location in file
        lseek(fileDescriptor, rand_num, recordSize);
        read(fileDescriptor, buffer, recordSize);
    }

    //Close the file and corresponding buffers
    close(fileDescriptor);
    free(buffer);

}




int main(int argc, char *argv[])
{
    char *accessPattern = argv[1];
    if (strcmp(accessPattern, "W") == 0)
    {
        cout<<"Write Sequential Method Call"<<"\n";
        write(atoi(argv[2]),atoi(argv[3]));
    }
    else if (strcmp(accessPattern, "R") == 0)
    {
        cout<<"Read Sequential Method Call"<<"\n";
        read(atoi(argv[2]),atoi(argv[3]));
    }
   else if (strcmp(accessPattern, "WR") == 0)
    {
        cout<<"Write Random Method Call"<<"\n";
        write_random(atoi(argv[2]),atoi(argv[3]));
    }
   else if (strcmp(accessPattern, "RR") == 0)
    {
        cout<<"Read Random Method Call"<<"\n";
        read_random(atoi(argv[2]),atoi(argv[3]));
    }

     return(0);
}

void write(int files, int record) 
{
   //put this value below prior too submission : 10485760000
   long long tenGb = 10485760000;   
   long long fileSize = tenGb/files;

   //spawn threads and execute helper method to do the benchmarking
   pthread_t threads[files];
   struct thread_data td[files];
   int rc;
   int i;
   for( i = 0; i < files; i++ ) {
      td[i].id = i;
      td[i].rec = record;
      td[i].fileSize = fileSize;
      rc = pthread_create(&threads[i], NULL, WriteHelper, (void *)&td[i]);      
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   auto start = high_resolution_clock::now();
   //wait for all threads to complete
   for (int i = 0; i < files; i++)
       pthread_join(threads[i], NULL);

   auto stop = high_resolution_clock::now();

   //capture the duration of code-runtime
   auto duration = duration_cast<microseconds>(stop - start);

   double speed = (float) tenGb/(float)duration.count();

   double IOPS =  (speed/(record/1024))*1024;

   cout << endl << "Write Speed is : "  << speed << "MB/sec"<<endl;

   cout<< "IOPS : " << IOPS<<" OPS/sec"<<endl;
   
   pthread_exit(NULL);
}

void write_random(int files, int record) 
{   

   long long int tenGb = 10485760000;
   long long fileSize = tenGb/files;

    //spawn threads and execute helper method to do the benchmarking
   pthread_t threads[files];
   struct thread_data td[files];
   int rc;
   int i;   
   for( i = 0; i < files; i++ ) {
      td[i].id = i;
      td[i].rec = record;
      td[i].fileSize = fileSize;
      rc = pthread_create(&threads[i], NULL, WriteRandomHelper, (void *)&td[i]);      
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   auto start = high_resolution_clock::now();
   //wait for all threads to complete
   for (int i = 0; i < files; i++)
       pthread_join(threads[i], NULL);
   
   auto stop = high_resolution_clock::now();
   //capture the duration of code-runtime
   auto duration = duration_cast<microseconds>(stop - start);

    double speed = (float) tenGb/(float)duration.count();

    double IOPS =  (speed/(record/1024))*1024;

    cout << endl << "Write Random Speed is : " <<speed<<"MB/sec"<<endl;

    cout<< "IOPS : "<<IOPS<<" OPS/sec"<<endl;

    pthread_exit(NULL);
}

void read(int files, int record)
{
   //put this value below prior too submission : 10485760000    
   long long int tenGb = 10485760000;
   long long int fileSize = tenGb/files;

   pthread_t threads[files];
    //spawn threads and execute helper method to do the benchmarking
   struct thread_data td[files];
   int rc;
   int i;
   for( i = 0; i < files; i++ ) {
      td[i].id = i;
      td[i].rec = record;
      td[i].fileSize = fileSize;
      rc = pthread_create(&threads[i], NULL, ReadHelper, (void *)&td[i]);
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   auto start = high_resolution_clock::now();

    //wait for all threads to complete
   for (int i = 0; i < files; i++)
       pthread_join(threads[i], NULL);
   
   auto stop = high_resolution_clock::now();
    //capture the duration of code-runtime
   auto duration = duration_cast<microseconds>(stop - start);

    double speed = (float) tenGb/(float)duration.count();

    double IOPS =  (speed/(record/1024))*1024;
   
    cout << endl << "Read Speed is : "  << speed << "MB/sec"<<endl;

    cout<< "IOPS : "<<IOPS<<" OPS/sec"<<endl;


    pthread_exit(NULL);
}

void read_random(int files, int record)
{
   //put this value below prior too submission : 10485760000    
   long long int tenGb = 10485760000;
   long long int fileSize = tenGb/files;

   pthread_t threads[files];
    //spawn threads and execute helper method to do the benchmarking
   struct thread_data td[files];
   int rc;
   int i;
   for( i = 0; i < files; i++ ) {
      td[i].id = i;
      td[i].rec = record;
      td[i].fileSize = fileSize;
      rc = pthread_create(&threads[i], NULL, ReadRandomHelper, (void *)&td[i]);
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }

   auto start = high_resolution_clock::now();

    //wait for all threads to complete
   for (int i = 0; i < files; i++)
       pthread_join(threads[i], NULL);

   auto stop = high_resolution_clock::now();
    //capture the duration of code-runtime
   auto duration = duration_cast<microseconds>(stop - start);

    double speed = (float) tenGb/(float)duration.count();

    double IOPS =  (speed/(record/1024))*1024;

    cout << endl << "Read Random Speed is : " <<speed<< "MB/sec"<<endl;
    cout<< "IOPS : "<<IOPS<<" OPS/sec"<<endl;


    pthread_exit(NULL);
}
