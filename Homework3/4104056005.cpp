/*平均深度算法：所有深度總和除以活點(不含iso)*/ 
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <queue> 
#define MAX 10 
using namespace std;

typedef struct{
	int row;
	int col;
}element;
element r[MAX*MAX];
typedef struct{
	int vert;
	int horiz;
}offsets;
offsets move[4];//八個方位
int map[MAX][MAX] = {0};
int depth[MAX][MAX] = {0};
int n = 0;//使用者輸入活點數 
int cnt = 0;

//四個移動方向 
void init_move()
{
	move[0].vert = -1; move[0].horiz =  0;//N
	move[1].vert =  0; move[1].horiz =  1;//E
	move[2].vert =  1; move[2].horiz =  0;//S
	move[3].vert =  0; move[3].horiz = -1;//W
}
//利用random做出map，4死點 5活點 0北1東2南3西 6原點 
void init_map()
{
	int i, j;
	srand(time(NULL));
	//初始化map全部為4(死點) 
	for(i = 0;i < MAX; i++)
		for(j = 0;j < MAX; j++)
			map[i][j] = 4;
	map[MAX/2][MAX/2] = 6;//原點 
	//使用random做出活點 
	for(i = 0;i < n; i++)
	{
		int row = 0, col = 0;
		//從0~MAX-1中隨機挑出數字
		do
		{
			row = rand() % MAX;
			col = rand() % MAX;
		}
		while(map[row][col] == 5 || map[row][col] == 6);//若隨機選到的點已是活點或原點，再挑一個 
		map[row][col] = 5;
	}
}

void BFS()
{
	int i, j, l;
	int k = 0;
	int row, col;
	queue<int> q;
	
	//放入原點周圍的點，並加入方向、收進佇列中 
	for(i = 0;i < 4; i++)
	{
		row = MAX/2 + move[i].vert;
		col = MAX/2 + move[i].horiz;
		
		if(map[row][col] == 5)//若此點為活點，設定0~3方向
		{
			map[row][col] = i;
			depth[row][col] = 1;
			r[k].row = row;
			r[k].col = col;
			q.push(k);
			k++;
		}
	}
	
	while(!q.empty())
	{
		int p;
		p = q.front();
		q.pop();
		
		//找出這個點周圍可走的點(從4個方位依序找點) 
		for(i = 0;i < 4; i++)
		{
			//新點 
			row = r[p].row + move[i].vert;
			col = r[p].col + move[i].horiz;
		
			if(row < 0 || row >= MAX || col < 0 || col >= MAX)
				continue;
			else if(map[row][col] == 5)//若此點為活點且未走過 
			{
				map[row][col] = map[r[p].row][r[p].col];
				depth[row][col] = depth[r[p].row][r[p].col] + 1;
				r[k].row = row;
				r[k].col = col;
				q.push(k);
				k++;
			}
			else if(map[row][col] != 4 && map[row][col] != 6 && map[row][col] != 5 && map[row][col] != map[r[p].row][r[p].col])//非原點的已走過活點 
			{
				//找出最適合的填入數字 
				//比較depth
				if(depth[row][col] > depth[r[p].row][r[p].col] + 1)
				{
					//改變數字及深度(不需再放入queue中) 
					map[row][col] = map[r[p].row][r[p].col];
					depth[row][col] = depth[r[p].row][r[p].col] + 1;
				} 
				else if(depth[row][col] == depth[r[p].row][r[p].col] + 1)
				{
					//depth相同，找node數比較少的
					int a = 0, b = 0;//計算node數
					int c;
					//計算距離，選擇大的
					if(row-MAX/2 >= col-MAX/2)
						c = row-MAX/2;
					else
						c = col-MAX/2;
					//計算並比較node數量，選擇小的 
					for(j = MAX/2-c;j <= MAX/2+c; j++)
						for(l = MAX/2-c;l <= MAX/2+c; l++)
						{
							if(j == row && l == col)
								continue;
							else if(map[j][l] == map[row][col])
								a++;
							else if(map[j][l] == map[r[p].row][r[p].col])
								b++;
						}
					if(a > b)
						map[row][col] = map[r[p].row][r[p].col];
					else if(a == b)//若depth和node都相同，依照位置分配方位 
					{ 
						if(row < MAX/2 && col <= MAX/2)
							map[row][col] = 0;
						else if(row <= MAX/2 && col > MAX/2)
							map[row][col] = 1;
						else if(row > MAX/2 && col >= MAX/2)
							map[row][col] = 2;
						else if(row >= MAX/2 && col < MAX/2)
							map[row][col] = 3;
					}
				} 
			}
			 
		}
		
	}
	
}

void count(int n)
{
	int i, j;
	cnt = 0;
	for(i = 0; i < MAX; i++)
		for(j = 0; j < MAX; j++)
		{
			if(map[i][j] == n)
			{
				printf("< %d,%d >",i, j);
				cnt++;
			}
		}
}

int maxDepth(int n)
{
	int i, j, max = 0;
	for(i = 0; i < MAX; i++)
		for(j = 0; j < MAX; j++)
		{
			if(map[i][j] == n)
			{
				if(depth[i][j] > max)
					max = depth[i][j];
			}
		}
	return max;
}

double aveDepth()
{
	int i, j;
	double c = 1, sum = 0;//原點也算入分母 
	double ave = 0;
	for(i = 0; i < MAX; i++)
		for(j = 0; j < MAX; j++)
		{
			if(depth[i][j] != 0)
			{
				sum += depth[i][j];
				c++;
			}
		}
	if(c == 0)
		ave = 0;
	else
		ave = sum / c;
	return ave;
}

int maxd()
{
	int i, j, max = 0;
	for(i = 0; i < MAX; i++)
		for(j = 0; j < MAX; j++)
		{
			if(depth[i][j] != 0)
			{
				if(max < depth[i][j])
					max = depth[i][j];
			}
		}
	return max;
}

void printMap()
{
	int i, j;
	for(i = 0; i < MAX; i++)
	{
		for(j = 0; j < MAX; j++)
		{
			if(map[i][j] == 6)
				printf(" o ");
			else if(map[i][j] == 4)
				printf(" - ");
			else if(map[i][j] == 5)
				printf("iso");
			else if(map[i][j] == 0)
				printf(" 0 ");
			else if(map[i][j] == 1)
				printf(" 1 ");
			else if(map[i][j] == 2)
				printf(" 2 ");
			else if(map[i][j] == 3)
				printf(" 3 ");
		}
		printf("\n");
	}
		
}

void printDepth()
{
	int i, j;
	for(i = 0; i < MAX; i++)
	{
		for(j = 0; j < MAX; j++)
		{
			if(depth[i][j] >= 10)
				printf(" %d",depth[i][j]);
			else
				printf(" %d ",depth[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char** argv) 
{
	int i;
	
	cout << "請輸入0~99的整數：" << endl;
	cin >> n;
	init_move();
	init_map();
	BFS();
	//第一部分 
	cout << "1.Rorted Paths：" << endl;
	for(i = 0; i < 4; i++)
	{
		cout << "***Path" << i << "  Hub links:";
		count(i);
		printf("\n");
		printf("Path%d  Hub數 = %d, Max Depth = %d\n", i, cnt ,maxDepth(i));
		printf("\n"); 
	}
	
	//第二部分 
	cout << "2.Isolated Node：";
	count(5);
	printf("\nIsolated數 = %d\n",cnt);
	printf("\n");
	
	//第三部分
	cout << "3.Average Depth = ";
	printf("%.2f\n",aveDepth());
	printf("\n");
	
	//第四部分
	cout << "4.Max Depth = ";
	printf("%d\n",maxd()); 
	printf("\n");
	
	//第五部分
	cout << "5.Map：" << endl;
	printMap();
	printf("\n");
	
	//第六部分
	cout << "6.Depth：" << endl;
	printDepth();
	
	system("pause");
	return 0;
}
