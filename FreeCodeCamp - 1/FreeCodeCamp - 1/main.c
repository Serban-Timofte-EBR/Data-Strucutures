#include <stdio.h>
#include <stdlib.h>

//List as Abstract Data Type (ADT)
	//store a given number of elementss of a given data type
	//write/modify element at a position
	//read element at a position
	//EX: int v[10];		v[0] = 2;		print(a[i]); 

//Features of my list
	//empty list has size 0
	//insert in the list
	//remove elements in the list
	//cout
	//read/modify elements at a position
	//specify data - type


//Costs of using arrays
	//Acces - read/write at index - constant time O(1)
	//Insert - Proportion to length of list (n) - O(n)
	//Remove - O(n)
	//Add element - O(n)

//Linked Lists 
/*MEMORY
*			a
{---------------------------}
|------|------|------|------|------|------|
|	   |	  |	     | 	    |	   |	  |
| a[0] | a[1] | a[2] | a[3] |x = 8 |	  |
|______|______|______|______|______|______|
   201    205    209	213	   217

   Memory Manager - keeps track of what part of memory is free and what part is allocated
				  - anyone who needs memory wokrs with memory manager

	Programmer - needs to store data
			   - declare and int x; => Memory manager looks for 4 free B in manager
									- Considering that the first B is at adress 217
									- int a[4] => look for a block of 16B (start at 201)
									- a[3] = 2 => acces adress of 201 + 3x4 = 213
			  - now, want to extend the array a with one more element
					- Memory manager => we have x near last element of a, so cannot extend memory of a
										- will look for a new block of 5 int and copy the current a, free the memory and insert the new element
										- this problem will repeat and repeat
	SOLUTION: Linked List
		- Programmer will ask memory for one unit of data at a time
		- Each element will be alocated separated in memory, not continious 
		- The new problem is how we link these elements?
			- We store extra info in a block
			- The value in a part of the block and the adress to the next element in other part of block
			- block is a node
				struct Node {
					int data;
					Node* next;
				}
			- the programmer will ask for a block


		  204				
	|------|------|
	|	   |	  |
	|  6   |  217 |			NODE
	|______|______|
	  HEAD

		  217
	|------|------|
	|	   |	  |
	|  5   |  232 |			NODE
	|______|______|
	  HEAD

		  232
	|------|------|
	|	   |	  |
	|  4   |  242 |			NODE
	|______|______|
	  HEAD

		  242
	|------|------|
	|	   |	  |
	|  2   |  252 |			NODE
	|______|______|
	  HEAD

		  252
	|------|------|
	|	   |	  |
	|  3   |NULL 0|			NODE
	|______|______|
	  HEAD

	  //We cannot acces a specific element in constant time
			// We have to start at the head and ask each element for the next element 
			// Like a treasure hunt 
			// Acces O(n)
			// Insert O(n) - but simpler
			// no extra use of memory
*/

	//Array vs Linked List
/*
						ARRAY										LINKED LIST

ACCESING			Constant time							We have to read all the elements before				
				(based on calculation)						Proportional to number of elem => O(n)
				!!Beter for accesing!!

MEMORY				Fixed size									No unused memory
USAGE			Garbage memory or unused memory				Extra memory for pointer to the node header
				Memory may not be available as				Memory may be available as multiple small blocks
					one large block

INSERTING
	BEGIN			Shift each element O(n)					Only create a new node ad link to the list O(1)
	END				Not full list O(1)							O(n)
					Full List O(n)
	MIDDLE			Shift n/2 elements O(n)					Travel to position an link the new node O(n)

DELETE									Same for both on each scenario	

															Prone to errors 

*/

//Implementation of Linked List

typedef struct {
	int data;
	struct node* link;
} node;

int main() {
	node* a = NULL; // pointer to the first element of linked list

	node* temp = (node*)malloc(sizeof(node));	//first node in the list

	(*temp).data = 12; //write value to the head of the fist node of the list
	(*temp).link = NULL; // it is the first and last element in list

	a = temp;

	free(temp);

	temp = (node*)malloc(sizeof(node));
	(*temp).data = 4;
	(*temp).link = NULL;

	//Go the the end of the list

	node* temp1 = a;

	while ((*temp1).link != NULL) {
		temp1 = temp1->link;
	}

	return 0;
}