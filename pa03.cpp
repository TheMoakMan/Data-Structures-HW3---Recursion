#include "maze.h"

int main()
{
  int numRows;                   //Number of rows the matrix will contain
  int numCols;                   //This value is irrelevant as the matrix will be built with strings
  int numTrials = 0;             //Number of cases the user inputs.
  int rowPos = 0;                //Will hold the row Index
  int colPos = 0;                //Will hold the column Index
  
  do{
	//Read in the size of the matrix
    cin >> numRows;
    cin >> numCols;
    
	if(numRows > 0  && numCols > 0){
      //Build the matrix
	  string *grid = build_matrix(numRows);
	
	  //Read in the maze and fill the matrix.
	  fill_matrix(grid, numRows);
	
	  //Find the start of the maze.
	  find_start(grid, numRows, rowPos, colPos);
	
	  //Attempt to solve the maze
	  cout << "Map " << numTrials << " -- ";
	  if(find_exit(grid, rowPos, colPos))
	   cout << "Solution found: " << endl;
      else
	    cout << "No solution found: " << endl;
  
      //Print out the final version of the matrix
      print_matrix(grid, numRows);
 
      //Increment the number of Trials
      numTrials++;	
	
	  cout << endl;
	  
	  //Delete the Matrix
	  delete_matrix(grid, numRows);
	}
  }while(!(numRows == 0 && numCols == 0));
	
	
  return 0;
}

