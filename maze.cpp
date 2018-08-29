#include "maze.h"

/*~~~~~GLOBAL CONSTANTS~~~~~*/
const string USERNAME = "ebmy83";
const string STUDENT_ID = "12422230";
const char NIOBE = 'N';
const char END = 'E';
const char MARKER = '@';

/*~~~~CUSTOM FUNCTION DECLARATIONS~~~~*/

/*
   Takes in a value 1-4 from within a for loop and returns
   a direction in the order: NORTH, SOUTH, EAST, WEST.
*/
direction getDirection(int dir_index);

/*
  Checks the given direction to make sure that there is an available space to move into
  and moves the character into that space. This function has a few checks to ensure proper movement. 
    -- Returns false if the current space is on an intersection, a corner, or a dead end.
	-- When at an intersection, the function uses the boolean parameter, new_branch, to determine if
	   if should continue moving on in a new direction. This helps prevent the recursive function from 
	   calling itself again whenever it has just returned false back to an intersection.
*/
bool makeMove(string *matrix, int &x, int &y, direction dir, bool new_branch);

/*
   Moves the character one space backwards from the passed in direction if valid. 
   A valid move in this case refers to if a space contains the character '@'.
*/
bool undoMove(string *matrix, int &x, int &y, direction dir);


void get_identity(string my_id[])
{
  my_id[0] = USERNAME;
  my_id[1] = STUDENT_ID;
}

string * build_matrix(int rows)
{
  string * matrix = new string[rows];
  return matrix;
}


void fill_matrix(string *matrix, int rows)
{
  cin.ignore();
  for(int i = 0; i < rows; i++){
    if(getline(cin, matrix[i], '\n'));	
  }
}


void print_matrix(string *matrix, int rows)
{
  for(int i = 0; i < rows; i++){
	cout << matrix[i] << endl;		
  }
}


void delete_matrix(string *matrix, int rows)
{
  delete[] matrix;
}


void find_start(string *matrix, int rows, int &x, int &y)
{
  bool start_found = false;
  
  x = 0;
  while(x < rows && !start_found){
	y = 0;
    while(y < matrix[x].length() && !start_found){
      if(matrix[x][y] == NIOBE)
		start_found = true;	
	  y++;
	}	  	
	x++;
  } 
  
  if(!start_found){
   //If no start found, send a flag.
   x = y = -1;
  }	 
  else{
   //Update X and Y to the correct row and column indices.
   x -= 1;
   y -= 1;
  }
  
}

bool find_exit(string *matrix, int x, int y)
{
  //Set a flag for the make move function to understand whether or not
  //it is on a new instance of a decision point.
  static bool new_branch = true;
  
  //Check each direction for a valid move, if possible make moves in that direction
  //until recursive function call is returned false, then check another direction.
  for(int i = 1; i <= 4; i++)
  {
	//Temporary locations for each individual movement until
	//the next decision point. This helps to save the position for each time the
	//recursive function is called. 
	int temp_x = x;
	int temp_y = y;
	
	//Determine which direction to move
	direction next_dir = getDirection(i);
	
	//Mark the current location
	if(matrix[temp_x][temp_y] != NIOBE)
	  matrix[temp_x][temp_y] = MARKER;

    //Search if the next space is a valid move in the current direction
    if(valid_move(matrix, temp_x, temp_y, next_dir)){	
	
	  //If move is valid, travel in a straight line until
	  //The next decision branch and mark the path traveled thus far.	   
	  int moveCount = 0;
	  while(makeMove(matrix, temp_x, temp_y, next_dir, new_branch)){
		
		//If not at the end, mark the current location 
		if(!at_end(matrix, temp_x, temp_y)){
		  matrix[temp_x][temp_y] = MARKER; 
		}
		//If we are at the end then return true, total success.
	    else
          return true;

	    //Increment move counter and reset the branch flag.
        moveCount++;
        new_branch = true;		
	  }
	  	 
      //Recursive call, this happens whenever you hit a corner or an intersection,
	  //these calls are referred to as "decision points" because they represent an instance
	  //where there is a change of direction or there are multiple options available. 
	  //The function returns to the last intersection or corner when false.  
	  if(find_exit(matrix, temp_x, temp_y))
		//If true then the maze has been successfully solved.
		return true;
	  else{
		//Traverse back to the last intersection or corner.
		new_branch = false;
	    for(int i = 0; i < moveCount; i++){
	      undoMove(matrix, temp_x, temp_y, next_dir);
		  matrix[temp_x][temp_y] = ' ';
		}		
	  } 
	} 	
 	
  }

  //If there are no more valid moves, return to previous branch and reset the current position
  //back to an empty space.
  if(matrix[x][y] != NIOBE)
	matrix[x][y] = ' ';
  
  return false;	

}

bool at_end(string *matrix, int x, int y)
{
  bool solved = false;
  if(matrix[x][y] == END)
   solved = true;

  return solved;
}

bool valid_move(string *matrix, int x, int y, direction d)
{  
  bool valid = false;
  
  switch(d){
    case NORTH:
	  if(matrix[x-1][y] == ' ' || matrix[x-1][y] == END)
		valid = true;
	  break;
    case WEST:
	  if(matrix[x][y-1] == ' ' || matrix[x][y-1] == END)
		valid = true;
	  break;
	case SOUTH:
	  if(matrix[x+1][y] == ' ' || matrix[x+1][y] == END)
		valid = true;
	  break;
	case EAST:
	  if(matrix[x][y+1] == ' ' || matrix[x][y+1] == END)
		valid = true;
	  break;
	default: break;
  }

  return valid;
}

direction getDirection(int dir_index)
{
  direction nextDir;
  
  if(dir_index == 1)
	nextDir = NORTH;
  else if(dir_index == 2)
	nextDir = WEST;
  else if(dir_index == 3)
    nextDir = SOUTH;
  else if(dir_index == 4)
	nextDir = EAST;
  
  return nextDir;
}

bool makeMove(string *matrix, int &x, int &y, direction dir, bool new_branch)
{
   bool valid = false;                        //Flag to be returned if the attempted move is valid or not
   static bool at_fork = false;               //Flag to determine if the current location is at an intersection
   
   //Allows the function to understand whenever the location has 
   //been returned back to the last intersection, so it doesn't try to
   //return back without trying the a new direction.
   if(!new_branch)                          
	 at_fork = true;
 
   switch(dir){
    case NORTH:
	  //Moves the character as long as there isn't a wall or an intersection.
	  if(!at_fork && (matrix[x-1][y] == ' ' || matrix[x-1][y] == END) && !(matrix[x][y-1] == ' ' || matrix[x][y+1] == ' ' || matrix[x][y-1] == END || matrix[x][y-1] == END)){
		x -= 1;
	    valid = true;
	  }
	  //If the character just landed on the fork, signify that a fork has been located.
	  else if(!at_fork && (matrix[x][y-1] == ' ' || matrix[x][y+1] == ' ' || matrix[x][y-1] == END || matrix[x][y-1] == END)){
		at_fork = true;
	  }
	  //Moves onward passed the fork in the given direction.
	  else if(at_fork && (matrix[x-1][y] == ' ' || matrix[x-1][y] == END)){
		x -= 1;
		at_fork = false;
		valid = true;
	  }
	  break;
    case WEST:
	  if(!at_fork && (matrix[x][y-1] == ' ' || matrix[x][y-1] == END) && !(matrix[x-1][y] == ' ' || matrix[x+1][y] == ' ' || matrix[x-1][y] == END || matrix[x-1][y] == END)){
		y -= 1;
		valid = true;
	  }
	  else if(!at_fork && (matrix[x-1][y] == ' ' || matrix[x+1][y] == ' ' || matrix[x-1][y] == END || matrix[x-1][y] == END)){
		at_fork = true;
	  }
	  else if(at_fork && (matrix[x][y-1] == ' ' || matrix[x][y-1] == END)){
		y -= 1;
		at_fork = false;
		valid = true;
	  }
	  break;
	case SOUTH:
	  if(!at_fork && (matrix[x+1][y] == ' ' || matrix[x+1][y] == END) && !(matrix[x][y-1] == ' ' || matrix[x][y+1] == ' ' || matrix[x][y-1] == END || matrix[x][y+1] == END)){
		x += 1;
	    valid = true;
	  }
	  else if(!at_fork && (matrix[x][y-1] == ' ' || matrix[x][y+1] == ' ' || matrix[x][y-1] == END || matrix[x][y+1] == END)){
		at_fork = true;
	  }
	  else if(at_fork && (matrix[x+1][y] == ' ' || matrix[x+1][y] == END)){
	    x += 1;
		at_fork = false;
		valid = true;
	  }
	  break;
	case EAST:
	  if(!at_fork && (matrix[x][y+1] == ' ' || matrix[x][y+1] == END) && !(matrix[x-1][y] == ' ' || matrix[x+1][y] == ' ' || matrix[x-1][y] == END || matrix[x+1][y] == END)){
		y += 1;  
		valid = true;
	  }
	  else if(!at_fork && (matrix[x-1][y] == ' ' || matrix[x+1][y] == ' ' || matrix[x-1][y] == END || matrix[x+1][y] == END)){
		at_fork = true;
	  }
	  else if(at_fork && (matrix[x][y+1] == ' ' || matrix[x][y+1] == END)){
		y += 1;
        at_fork = false;
        valid = true;
      }		
	  break;
	default: break;
  }
	
  return valid;
}

bool undoMove(string *matrix, int &x, int &y, direction dir)
{
   //Traverse backwards in the opposite of the direction passed.
   bool valid = false;
   
   switch(dir){
    case NORTH:
	//Move South
	  if(matrix[x+1][y] == MARKER){
		x += 1;
	    valid = true;
	  }	
	  break;
    case WEST:
	//Move East
	  if(matrix[x][y+1] == MARKER){
		y += 1;
		valid = true;
	  }
	  break;
	case SOUTH:
	//Move North
	  if(matrix[x-1][y] == MARKER){
		x -= 1;
		valid = true;
	  }
	  break;
	case EAST:
	//Move West
	  if(matrix[x][y-1] == MARKER){
		y -= 1;
		valid = true;
	  }
	  break;
	default: break;
  }
	
  return valid;
}

