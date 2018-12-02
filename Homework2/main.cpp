#include <iostream>
#include <vector>
#include <queue>
#include <cmath> 
using namespace std;

struct node{
	char asc;//字元
	int value;//出現次數 
	int parent, left, right;//樹的結構 
	int bit;//該節點的bit 
	int leaf;//該點葉子總數 
	int len;//該點的編碼長度 
};
typedef struct node Node;
Node data[95*2-1];
struct function
{
	bool operator()(const int& i, const int& j)
	{
		if(data[i].value != data[j].value)//若出現次數不相等 
			return data[i].value > data[j].value;//比較出現次數 
		return data[i].len > data[j].len;//若相等，比較編碼長度 
	}
};

FILE *fptr1 = NULL, *fptr2 = NULL;
char inFile[20] = "";//文件名稱 
char outFile[20] = ""; 
int cnt[95]={0};//儲存字元出現頻率
vector<int> code[95];//碼表(顛倒) 
int s;//未滿8的倍數，多補充的0個數 
int b = 0;

void codeTree()
{
	priority_queue<int, vector<int>, function> pq;//由小到大排列 
	
	int i, j;
	//初始化data
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
	
	//建立Huffman tree 
	for(i = 95;i < 95*2-1; i++)
	{
		//最小的兩個點 
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
	
	//從葉節點往樹根編碼
	for(i = 0; i < 95; i++)//0~94為葉節點 
	{
		code[i].clear();//初始化 
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
	vector<int> temp;//暫存字串編碼
	vector<int> cint;//儲存要轉成char的十進位數字 
	temp.clear(); 
	cint.clear();
	
	//輸入檔案名稱 
	printf("  input: ");
	scanf("%s",inFile);
	printf("\n\n");
	printf("  output: ");
	scanf("%s",outFile);
	printf("\n\n");
	
	//計算字元出現次數
	fptr1 = fopen(inFile,"r");//開啟可讀取檔案 
	if(fptr1 != NULL)
	{
		while((c = fgetc(fptr1)) != EOF)
		{
			cnt[(int)c-32]++;
		}
		//輸出字數統計  
		printf("字數統計:\n");
		for(i = 0; i < 95;i ++)//初始化並印出字數統計 
		{
			printf("%c : %d\n",(char)i+32,cnt[i]);
		}
		fclose(fptr1);
	}
	else
		printf("Error opening this file!\n");
		
	//壓縮 
	fptr1 = fopen(inFile,"r");//開啟可讀取檔案 
	fptr2 = fopen(outFile,"w");//開啟可寫入檔案 
	if(fptr1 != NULL && fptr2 != NULL)
	{
		//建立Huffman tree並編碼 
		codeTree();
		//輸出霍夫曼編碼
		printf("\nHuffman編碼:\n"); 
		printCode();
		
		//轉換編碼 
		while((c = fgetc(fptr1)) != EOF)
		{
			for(i = code[(int)c-32].size()-1;i >= 0; i--)
				temp.push_back(code[(int)c-32][i]);
		}
		
		if(temp.size()%8 != 0)
		{
			s = 8 - (temp.size())%8;//須補s個0
			for(i = 0;i < s; i++)
				temp.push_back(0); 
		}
		
		//將壓縮資料儲存進檔案中 
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

		//8個0、1一組轉成int型態的十進位 
		for(i = 0;i <= temp.size()-1; i=i+8)
		{
			int num = 0;
			for(j = 0;j < 8; j++)
			{
				num = num + temp[i+j] * pow(2,7-j);
			}
			cint.push_back(num);
		}
		
		//將十進位轉成char的型態輸出到outfile中 
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
	vector<int> temp;//暫存字元轉十進位
	vector<int> bint;//儲存十進位轉二進位 
	temp.clear(); 
	bint.clear();
	
	//初始化 
	for(i = 0;i < 95; i++)
	{
		cnt[i] = 0;
	}
	
	//輸入檔案名稱 
	printf("  input: ");
	scanf("%s",inFile);
	printf("\n\n");
	printf("  output: ");
	scanf("%s",outFile);
	printf("\n\n");
	
	fptr1 = fopen(inFile,"rb");//開啟可讀取檔案 
	fptr2 = fopen(outFile,"w");//開啟可寫入檔案 
	if(fptr1 != NULL && fptr2 != NULL)
	{
		//輸入原檔案字數統計 
		for(i = 0;i < 95; i++)
		{
			for(j = 0; j < 3; j++)
			{
					fread(&c, sizeof(char), 1, fptr1);
					cnt[i] = cnt[i] + (int)c * pow(256, 2-j);
			}
		}
		
		//補零數 
		fread(&c, sizeof(char), 1, fptr1);
		s = (int)c;
			
		//讀入壓縮檔內容，以int型態儲存 
		while(!feof(fptr1))
		{
			fread(&c, sizeof(char), 1, fptr1);
			temp.push_back((int)c);	
		}
		
		//建立Huffman tree並編碼 
		codeTree();
		
		//十進位轉成二進位 
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
		
		//刪除壓縮前加上的0;
		 while(s != 0)
		 {
		 	bint.pop_back();
		 	s--;
		 } 
		 
		 //從樹根開始走，遇到葉就輸出
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
