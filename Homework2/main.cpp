#include <iostream>
#include <vector>
#include <queue>
#include <cmath> 
using namespace std;

struct node{
	char asc;//�r��
	int value;//�X�{���� 
	int parent, left, right;//�𪺵��c 
	int bit;//�Ӹ`�I��bit 
	int leaf;//���I���l�`�� 
	int len;//���I���s�X���� 
};
typedef struct node Node;
Node data[95*2-1];
struct function
{
	bool operator()(const int& i, const int& j)
	{
		if(data[i].value != data[j].value)//�Y�X�{���Ƥ��۵� 
			return data[i].value > data[j].value;//����X�{���� 
		return data[i].len > data[j].len;//�Y�۵��A����s�X���� 
	}
};

FILE *fptr1 = NULL, *fptr2 = NULL;
char inFile[20] = "";//���W�� 
char outFile[20] = ""; 
int cnt[95]={0};//�x�s�r���X�{�W�v
vector<int> code[95];//�X��(�A��) 
int s;//����8�����ơA�h�ɥR��0�Ӽ� 
int b = 0;

void codeTree()
{
	priority_queue<int, vector<int>, function> pq;//�Ѥp��j�ƦC 
	
	int i, j;
	//��l��data
	for(i = 0;i < 95*2-1; i++) 
		data[i].value = 0;
	for(i = 0;i < 95; i++)
	{
		data[i].value = cnt[i];
		data[i].asc = (char)i+32;
		data[i].leaf = 1;
		data[i].len = 0;
		pq.push(i);
	}
	
	//�إ�Huffman tree 
	for(i = 95;i < 95*2-1; i++)
	{
		//�̤p������I 
		int small1 = pq.top();
		pq.pop();
		int small2 = pq.top();
		pq.pop();
		data[i].value = data[small1].value + data[small2].value;
		data[small1].parent = i;
		data[small2].parent = i;
		data[i].left = small1;
		data[i].right = small2;
		data[small1].bit = 0;
		data[small2].bit = 1;
		data[i].leaf = data[small1].leaf + data[small2].leaf;
		data[i].len = data[small1].len + data[small2].len + data[i].len;
		pq.push(i);
	}
	
	//�q���`�I����ڽs�X
	for(i = 0; i < 95; i++)//0~94�����`�I 
	{
		code[i].clear();//��l�� 
		for(j = i; j != 95*2-2; j = data[j].parent)
			code[i].push_back(data[j].bit);
	} 
	
}
void printCode()
{
	int i, j;
	for(i = 0; i < 95; i++)
	{
		if(data[i].value != 0)
		{
			printf("%c (%d)\t-> ",(char)i+32,cnt[i]);
			for(j = code[i].size()-1; j >= 0; j--)
			{
				cout << code[i][j];
			}
			printf("\n");
		}
		else
			continue;
	}			
}
void compress()
{
	int i, j;
	char c;
	vector<int> temp;//�Ȧs�r��s�X
	vector<int> cint;//�x�s�n�নchar���Q�i��Ʀr 
	temp.clear(); 
	cint.clear();
	
	//��J�ɮצW�� 
	printf("  input: ");
	scanf("%s",inFile);
	printf("\n\n");
	printf("  output: ");
	scanf("%s",outFile);
	printf("\n\n");
	
	//�p��r���X�{����
	fptr1 = fopen(inFile,"r");//�}�ҥiŪ���ɮ� 
	if(fptr1 != NULL)
	{
		while((c = fgetc(fptr1)) != EOF)
		{
			cnt[(int)c-32]++;
		}
		//��X�r�Ʋέp  
		printf("�r�Ʋέp:\n");
		for(i = 0; i < 95;i ++)//��l�ƨæL�X�r�Ʋέp 
		{
			printf("%c : %d\n",(char)i+32,cnt[i]);
		}
		fclose(fptr1);
	}
	else
		printf("Error opening this file!\n");
		
	//���Y 
	fptr1 = fopen(inFile,"r");//�}�ҥiŪ���ɮ� 
	fptr2 = fopen(outFile,"w");//�}�ҥi�g�J�ɮ� 
	if(fptr1 != NULL && fptr2 != NULL)
	{
		//�إ�Huffman tree�ýs�X 
		codeTree();
		//��X�N�Ұҽs�X
		printf("\nHuffman�s�X:\n"); 
		printCode();
		
		//�ഫ�s�X 
		while((c = fgetc(fptr1)) != EOF)
		{
			for(i = code[(int)c-32].size()-1;i >= 0; i--)
				temp.push_back(code[(int)c-32][i]);
		}
		
		if(temp.size()%8 != 0)
		{
			s = 8 - (temp.size())%8;//����s��0
			for(i = 0;i < s; i++)
				temp.push_back(0); 
		}
		
		//�N���Y����x�s�i�ɮפ� 
		for(i = 0;i < 95; i++)
		{
			int x[3] = {0};
			int k;
			for(k = 3;k > 0; k--)
			{
				if(cnt[i] != 0)
				{
					x[k] = cnt[i] % 256;
					cnt[i] /= 256;
				}
				else
				{
					x[k] = 0;
				}
			}
			for(j = 1;j <= 3; j++)
			{
				fputc((char)x[j],fptr2);
			}
		} 
		
		fputc((char)s,fptr2);

		//8��0�B1�@���নint���A���Q�i�� 
		for(i = 0;i <= temp.size()-1; i=i+8)
		{
			int num = 0;
			for(j = 0;j < 8; j++)
			{
				num = num + temp[i+j] * pow(2,7-j);
			}
			cint.push_back(num);
		}
		
		//�N�Q�i���নchar�����A��X��outfile�� 
		for(i = 0;i < cint.size(); i++)
		{
			fputc((unsigned char)cint[i], fptr2);
		}
		
		fclose(fptr1);
		fclose(fptr2);
		printf("complete!\n");
	}
	else
		printf("Error opening this file!\n");
}
char useTree(vector<int> &bint, int inode)
{
	if(data[inode].leaf == 1)
	{
		return data[inode].asc;
	}
	else if(bint[b] == 1)
	{
		b++;
		useTree(bint, data[inode].right);
	}
	else if(bint[b] == 0)
	{
		b++;
		useTree(bint, data[inode].left);
	}
}
void decompress()
{
	int i, j;
	unsigned char c;
	vector<int> temp;//�Ȧs�r����Q�i��
	vector<int> bint;//�x�s�Q�i����G�i�� 
	temp.clear(); 
	bint.clear();
	
	//��l�� 
	for(i = 0;i < 95; i++)
	{
		cnt[i] = 0;
	}
	
	//��J�ɮצW�� 
	printf("  input: ");
	scanf("%s",inFile);
	printf("\n\n");
	printf("  output: ");
	scanf("%s",outFile);
	printf("\n\n");
	
	fptr1 = fopen(inFile,"rb");//�}�ҥiŪ���ɮ� 
	fptr2 = fopen(outFile,"w");//�}�ҥi�g�J�ɮ� 
	if(fptr1 != NULL && fptr2 != NULL)
	{
		//��J���ɮצr�Ʋέp 
		for(i = 0;i < 95; i++)
		{
			for(j = 0; j < 3; j++)
			{
					fread(&c, sizeof(char), 1, fptr1);
					cnt[i] = cnt[i] + (int)c * pow(256, 2-j);
			}
		}
		
		//�ɹs�� 
		fread(&c, sizeof(char), 1, fptr1);
		s = (int)c;
			
		//Ū�J���Y�ɤ��e�A�Hint���A�x�s 
		while(!feof(fptr1))
		{
			fread(&c, sizeof(char), 1, fptr1);
			temp.push_back((int)c);	
		}
		
		//�إ�Huffman tree�ýs�X 
		codeTree();
		
		//�Q�i���ন�G�i�� 
		for(i = 0;i < temp.size()-1; i++)
		{
			int x[8] = {};
			for(j = 7;j >= 0; j--)
			{
				if(temp[i] != 0)
				{
					x[j] = temp[i] % 2;
					temp[i] /= 2;	
				}
				else
					x[j] = 0;
					
			}
			for(j = 0;j < 8; j++)
				bint.push_back(x[j]);	
		}
		
		//�R�����Y�e�[�W��0;
		 while(s != 0)
		 {
		 	bint.pop_back();
		 	s--;
		 } 
		 
		 //�q��ڶ}�l���A�J�츭�N��X
		 while(b < bint.size())
		 {
			int root = 95*2-2;
			fputc(useTree(bint, root), fptr2);
		 }
	
		fclose(fptr1);
		fclose(fptr2);
		printf("complete!\n");
	}
	else
		printf("Error opening this file!\n");
	
	
}

int main(int argc, char** argv) 
{
	printf("Compress => 1  |  Decompress => 0  |  Exit => 9 :\n");
	int ucmd;
	while(scanf("%d",&ucmd) != EOF)
	{
		switch(ucmd)
		{
			case 1:
				printf("Compress\n");
				compress();
				printf("\n");
				printf("Compress => 1  |  Decompress => 0  |  Exit => 9 :\n");
				break;
			case 0:
				printf("Decompress\n");
				decompress();
				printf("\n");
				printf("Compress => 1  |  Decompress => 0  |  Exit => 9 :\n");
				break;
			case 9:
				printf("Bye Bye\n");
				exit(1);
				break;
			default:
				printf("Error!Please try again.\n");
				printf("Compress => 1  |  Decompress => 0  |  Exit => 9 :\n");
		}
	}
	system("pause");
	return 0;
}
