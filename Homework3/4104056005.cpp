/*�����`�׺�k�G�Ҧ��`���`�M���H���I(���tiso)*/ 
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
offsets move[4];//�K�Ӥ��
int map[MAX][MAX] = {0};
int depth[MAX][MAX] = {0};
int n = 0;//�ϥΪ̿�J���I�� 
int cnt = 0;

//�|�Ӳ��ʤ�V 
void init_move()
{
	move[0].vert = -1; move[0].horiz =  0;//N
	move[1].vert =  0; move[1].horiz =  1;//E
	move[2].vert =  1; move[2].horiz =  0;//S
	move[3].vert =  0; move[3].horiz = -1;//W
}
//�Q��random���Xmap�A4���I 5���I 0�_1�F2�n3�� 6���I 
void init_map()
{
	int i, j;
	srand(time(NULL));
	//��l��map������4(���I) 
	for(i = 0;i < MAX; i++)
		for(j = 0;j < MAX; j++)
			map[i][j] = 4;
	map[MAX/2][MAX/2] = 6;//���I 
	//�ϥ�random���X���I 
	for(i = 0;i < n; i++)
	{
		int row = 0, col = 0;
		//�q0~MAX-1���H���D�X�Ʀr
		do
		{
			row = rand() % MAX;
			col = rand() % MAX;
		}
		while(map[row][col] == 5 || map[row][col] == 6);//�Y�H����쪺�I�w�O���I�έ��I�A�A�D�@�� 
		map[row][col] = 5;
	}
}

void BFS()
{
	int i, j, l;
	int k = 0;
	int row, col;
	queue<int> q;
	
	//��J���I�P���I�A�å[�J��V�B���i��C�� 
	for(i = 0;i < 4; i++)
	{
		row = MAX/2 + move[i].vert;
		col = MAX/2 + move[i].horiz;
		
		if(map[row][col] == 5)//�Y���I�����I�A�]�w0~3��V
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
		
		//��X�o���I�P��i�����I(�q4�Ӥ��̧ǧ��I) 
		for(i = 0;i < 4; i++)
		{
			//�s�I 
			row = r[p].row + move[i].vert;
			col = r[p].col + move[i].horiz;
		
			if(row < 0 || row >= MAX || col < 0 || col >= MAX)
				continue;
			else if(map[row][col] == 5)//�Y���I�����I�B�����L 
			{
				map[row][col] = map[r[p].row][r[p].col];
				depth[row][col] = depth[r[p].row][r[p].col] + 1;
				r[k].row = row;
				r[k].col = col;
				q.push(k);
				k++;
			}
			else if(map[row][col] != 4 && map[row][col] != 6 && map[row][col] != 5 && map[row][col] != map[r[p].row][r[p].col])//�D���I���w���L���I 
			{
				//��X�̾A�X����J�Ʀr 
				//���depth
				if(depth[row][col] > depth[r[p].row][r[p].col] + 1)
				{
					//���ܼƦr�β`��(���ݦA��Jqueue��) 
					map[row][col] = map[r[p].row][r[p].col];
					depth[row][col] = depth[r[p].row][r[p].col] + 1;
				} 
				else if(depth[row][col] == depth[r[p].row][r[p].col] + 1)
				{
					//depth�ۦP�A��node�Ƥ���֪�
					int a = 0, b = 0;//�p��node��
					int c;
					//�p��Z���A��ܤj��
					if(row-MAX/2 >= col-MAX/2)
						c = row-MAX/2;
					else
						c = col-MAX/2;
					//�p��ä��node�ƶq�A��ܤp�� 
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
					else if(a == b)//�Ydepth�Mnode���ۦP�A�̷Ӧ�m���t��� 
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
	double c = 1, sum = 0;//���I�]��J���� 
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
	
	cout << "�п�J0~99����ơG" << endl;
	cin >> n;
	init_move();
	init_map();
	BFS();
	//�Ĥ@���� 
	cout << "1.Rorted Paths�G" << endl;
	for(i = 0; i < 4; i++)
	{
		cout << "***Path" << i << "  Hub links:";
		count(i);
		printf("\n");
		printf("Path%d  Hub�� = %d, Max Depth = %d\n", i, cnt ,maxDepth(i));
		printf("\n"); 
	}
	
	//�ĤG���� 
	cout << "2.Isolated Node�G";
	count(5);
	printf("\nIsolated�� = %d\n",cnt);
	printf("\n");
	
	//�ĤT����
	cout << "3.Average Depth = ";
	printf("%.2f\n",aveDepth());
	printf("\n");
	
	//�ĥ|����
	cout << "4.Max Depth = ";
	printf("%d\n",maxd()); 
	printf("\n");
	
	//�Ĥ�����
	cout << "5.Map�G" << endl;
	printMap();
	printf("\n");
	
	//�Ĥ�����
	cout << "6.Depth�G" << endl;
	printDepth();
	
	system("pause");
	return 0;
}
