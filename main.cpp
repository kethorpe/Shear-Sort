#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "mpi.h"
#include "quickSort.h"

using namespace std;

const int MAX = 64;

void Write(int *sortedArray){
   ofstream myfile;

   myfile.open("out.txt");
   if(myfile.fail()){
        cout << "Unable to open file to write";
        return;
   }

   for(int i = 0; i < MAX; i++){
       for(int j = 0; j < MAX; j++){
          myfile << sortedArray[i * 64 + j] << " ";
       }
   }

   myfile.close();
}

void PrintArray(int arr[MAX * MAX]){
  for (int i = 0; i < MAX; ++i)
    {
       // cout << i << ": ";
        for (int j = 0; j < MAX; ++j)
        {
            cout << arr[i * 64 + j] << ' ';
        }
        //cout << endl;
    }
}

void RandomWrite(){
  /* initialize random seed: */
  srand(time(NULL));

   int random;
   ofstream myfile;

   myfile.open("in.txt");
   if(myfile.fail()){
        cout << "Unable to open file to write";
        return;
   }

   for(int i = 0; i < MAX; i++){
     for(int j = 0; j < MAX; j++){
        random = rand() % 100;
        myfile << random << " ";
      }
   }

   myfile.close();
}

void ReadInFile(int arr[MAX * MAX]){
   cout << "inside ReadInFile function" << endl;
   int num;
   ifstream din;
 
   din.open("in.txt");
   if (din.fail()) {
        cout << "Unable to open file to read";
        return;
    }

   for(int i = 0; i < MAX; i++){
      for(int j = 0; j < MAX; j++){
        din >> num;
        arr[i * 64 + j] = num;
      }
   }

  din.close();
}

main(int argc, char* argv[])
{
  int my_rank;       /* rank of process      */
  int p;             /* number of processes  */
  int source;        /* rank of sender       */
  int dest;          /* rank of receiver     */
  int tag = 0;       /* tag for messages     */
  MPI_Status status;        /* return status for receive */
  int elementsPerProc = MAX; //length of local array
  int *localArray;   //local array for each processor
  int numElements = MAX * MAX;   //total Num of elements in final array

  int unsortedArray[numElements];
  int sortedArray[numElements];

  /* Start up MPI */
  MPI_Init(&argc, &argv);
  
  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  
  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  
  elementsPerProc = MAX;

  if(my_rank == 0){

    RandomWrite();

    ReadInFile(unsortedArray);

    PrintArray(unsortedArray);
    cout << "about to start scatter process" << endl;
   }

   Scatter the data w/ Process 0
   localArray = (int*)malloc(sizeof(int) * elementsPerProc);
   MPI_Scatter(unsortedArray, elementsPerProc, MPI_INT, localArray, elementsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
    
   quickSort qs;

   if(my_rank % 2 == 0){
      qs.quickSortForward(localArray, 0, MAX - 1);
   }
   if(my_rank % 2 == 1){
      qs.quickSortBackward(localArray, 0,MAX - 1);
   }

   for(int i = 1; i < 13; i++){
      MPI_Alltoall(localArray, 1, MPI_INT, localArray, 1, MPI_INT, MPI_COMM_WORLD); //transpose the 2d array

      if(i % 2 == 0){ //sorting rows
          if(my_rank % 2 == 0){ //sort the row forwards
            qs.quickSortForward(localArray, 0, MAX - 1);
          }
          if(my_rank % 2 == 1){ //sort the row backwards
            qs.quickSortBackward(localArray, 0,MAX - 1);
          }
      }
      if(i % 2 == 1){ //sorting columns
          qs.quickSortForward(localArray, 0, MAX - 1);
      }
   }

    //qs.quickSortForward(localArray, 0, MAX - 1);

   //Gather the data w/ Process 0
   MPI_Gather(localArray, elementsPerProc, MPI_INT, sortedArray, elementsPerProc, MPI_INT, 0, MPI_COMM_WORLD);

  if(my_rank == 0){
    //cout << "After MPI Gather" << endl;
    //PrintArray(sortedArray);
      Write(sortedArray);
  }
 /* Shut down MPI */

   MPI_Finalize();

}
