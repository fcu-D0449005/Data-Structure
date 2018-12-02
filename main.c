#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define START_ROW 1
#define START_COL 1
#define EXIT_ROW 15
#define EXIT_COL 15
#define MAX_STACK_SIZE 225
#define FALSE 0
#define TRUE 1

typedef struct{
	short int row;
	short int col;
	short int dir;
}element;
element stack[MAX_STACK_SIZE];

typedef struct{
		short int vert;
		short int horiz;
}offsets;
offsets move[8];//�K�Ӥ��
 
int top;
short int maze[17][17] = {0};
short int mark[17][17] = {0};
void init_maze(short int(*)[17]);
void print_maze();
void init_move();
void stack_full();
void stack_empty();
void push(element);
element pop();
void path();

int main(int argc, char *argv[]) 
{
	init_maze(maze);
	printf("the original maze:\n");
	print_maze(maze);
	
	if(maze[START_ROW][START_ROW] == 1)
		printf("This maze has no entrance.\n");
	else if(maze[EXIT_ROW][EXIT_COL] == 1)
		printf("This maze has no exit.\n");
	else
	{
		/*�Y���X�J�f�A�h�}�l�M��path*/
		init_move();
		path();
	}
	system("pause");
	return 0;
}

/*�ϥ�random�s�@maze*/
void init_maze(short int (*m)[17])
{
	int i, j;
	srand(time(NULL));
	for(i = 0;i < 17; i++)
	{
		*(*(m+0)+i) = 1;
		*(*(m+i)+0) = 1;
		*(*(m+16)+i) = 1;
		*(*(m+i)+16) = 1;
	}
	for(i = 1;i < 16; i++)
		for(j = 1;j < 16; j++)
			*(*(m+i)+j) = rand()%3;
}

/*�L�X�}�C*/
void print_maze()
{
	int i,j;
	for(i = 1;i < 16; i++)
	{
		for(j = 1;j < 16; j++)
			printf("%3d",maze[i][j]);
		printf("\n");
	}
	printf("\n");
}

/*�C�Ӥ�쪺���ʤ�V*/
void init_move()
{
	move[0].vert = -1; move[0].horiz =  0;//N
	move[1].vert = -1; move[1].horiz =  1;//NE
	move[2].vert =  0; move[2].horiz =  1;//E
	move[3].vert =  1; move[3].horiz =  1;//SE
	move[4].vert =  1; move[4].horiz =  0;//S
	move[5].vert =  1; move[5].horiz = -1;//SW
	move[6].vert =  0; move[6].horiz = -1;//W
	move[7].vert = -1; move[7].horiz = -1;//NW
}

void stack_full()
{
	printf("The stack is full.  No item added \n");
}

void stack_empty()
{
	printf("The stack is empty.  No item deleted \n");
}

/*add*/
void push(element item)
{
	if(top == MAX_STACK_SIZE)
		stack_full();
	else
		stack[++top] = item;
}

/*delete*/
element pop()
{
	if(top < 0)
		stack_empty();
	else
		return stack[top--];
}

/*�Y�����s�b�N�L�X���|�A�S���h�L�X�����T��*/
void path()
{
	int i, j, row, col, next_row, next_col, dir, found = FALSE;
	element position;
	
	mark[1][1] = 1;
	top = 0;
	stack[0].row = 1;
	stack[0].col = 1;
	stack[0].dir = 0;
	
	while(top > -1 && !found)//�D��stack�B�٥����X�f 
	{
		position = pop();
		row = position.row;
		col = position.col;
		dir = position.dir;
		
		while(dir < 8 && !found)//�b����m�٦���V�i��B�٥����X�f 
		{
			next_row = row + move[dir].vert;
			next_col = col + move[dir].horiz;
				
			if(next_row == EXIT_ROW && next_col == EXIT_COL)
			{
				position.row = row;
				position.col = col;
				push(position);
				maze[EXIT_ROW][EXIT_COL] = 3;
				found = TRUE;//���X�f	
			}
			else if(maze[next_row][next_col] != 1 && mark[next_row][next_col] != 1)//�����i���B�٥����L
			{
				mark[next_row][next_col] = 1;//���O��
				position.row = row;
				position.col = col;
				if(maze[row][col] == 0)//0���K�Ӥ��i�� 
					dir += 1;
				else if(maze[row][col] == 2)//2���|�Ӥ�� 
					dir += 2;
				position.dir = dir;//��V�s�Jdir�A����p�G���즺���n�˰h�|�q�Ӧ�m���U�Ӥ�V�}�l��� 
				/*�T�w�U�@�B�����i���A�N�쥻��m��Jstack�A�ثe��m���U�@��*/
				push(position);
				row = next_row;
				col = next_col;
				dir = 0;//�U�����V�ɥi�H�q0���7 
			}
			else
			{
				if(maze[row][col] == 0)//0���K�Ӥ��i�� 
					dir += 1;
				else if(maze[row][col] == 2)//2���|�Ӥ�� 
					dir += 2;
			}
		} 
	}
	
	if(!found)
			printf("The maze does not have a path\n");
	else
	{
		printf("The maze traversal is: \n");
   		for(i = 0;i <= top; i++)
			maze[stack[i].row][stack[i].col] = 3;
		for(i = 1;i < 16; i++)
		{
			for(j = 1;j < 16; j++)
			{
				if(maze[i][j] != 3)
					printf("  *");
				else
					printf("  3");
			}
			printf("\n");
		}
	}
	 
}

