#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <time.h>
using namespace std;

struct def{
	char date[20];//日期
	char station[5];//測站
	int num[24];//24小時資料 
	double ave;//平均 
};
typedef struct def Data;
struct purple{
	char station[5];//站名 
	int day;//天數 
};
typedef struct purple Pur;
vector<Data> data;
vector<Pur> pur;
char s[1000];
FILE *fptr1 = NULL, *fptr2 = NULL;

void PMdata()
{
	cout << "各地PM2.5資料處理：" << endl;
	cout << "留下各地每小時PM2.5資料，若數據資料中出現非數字字元，該小時PM2.5數值為0" << endl; 
	fptr1 = fopen("input.txt","r");//開啟可讀取檔案
	fptr2 = fopen("PM2.5data.txt","w");//開啟可寫入檔案 
	if(fptr1 == NULL)
	{
		printf("Error opening this file!\n");
		exit(0);
	}
	
	fgets(s, 1000, fptr1);//標題行
	fputs(s,fptr2); 
	while (fgets(s,1000,fptr1) != NULL)//檔案中還有資料  
	{
		int i = 0, j = 0, k = 0;
		
		//找出PM2.5的資料 
		if(s[16] == 'P' && s[17] == 'M' && s[18] == '2' && s[19] == '.' && s[20] == '5')
		{	
			Data temp;
			while(i < 10)//儲存日期 
			{
				temp.date[j] = s[i];
				putc(s[i],fptr2);
				i++;
				j++;
			}
		 	putc(',',fptr2);
			i++;//跳過','
			j = 0;
			while(s[i] != ',')//儲存測站名稱 
			{
				temp.station[j] = s[i];
				putc(s[i],fptr2);
				i++;
				j++;
			}
			temp.station[j] = '\0';
			putc(',',fptr2);
			i++;//跳過','
			fputs("PM2.5",fptr2);
			while(s[i] != ',')//跳過PM2.5 
				i++;
			putc(',',fptr2);
			i++;//跳過','
			for(j = 0;j < 24; j++)//填入數字
			{
				int sum = 0;
				int  flag = 0;
				while(s[i] != ',')
				{
					int f = 0;
					int h;
					h = i;
					while(s[h] != ',')//先檢查一次數字是否有符號 
					{
						if(s[h] == '\0' || s[h] == '\n')
							break;
						if(s[h] < '0' || s[h] > '9')
						{
							sum = 0;
							f = 1;	
							break;
						}
						else
							h++; 
					}
					if(f == 1)//數字有符號，設為0並跳過它 
					{
						while(s[i] != ',')
							i++;
						break;
					}
					else if(s[i] == '\0')
						break;
					else if(s[i] >= '0' && s[i] <= '9')//此字元是數字時才填入，否則跳過 
					{
						sum = sum*10 + s[i]-'0';
						putc(s[i],fptr2);//寫入檔案 
						flag = 1;
					}
					i++; 
				}
				//處理無數值的資料 
				if(!flag)
					putc('0',fptr2);
				if(j != 23)
				{
					putc(',',fptr2);
					i++;//跳過','
				}	
				temp.num[j] = sum;				
			}
			putc('\n',fptr2);//在檔案中換行
				
			data.push_back(temp);
		}		
    }
    
    fclose(fptr1);
	fclose(fptr2);
	cout << "資料讀取完畢，PM2.5資料已存入PM2.5data.txt中" << endl; 
	printf("\n");
}
void average()
{
	cout << "計算各地平均值：" << endl;
	fptr2 = fopen("average.txt","w");//開啟可寫入檔案
	int i, j;
	double sum;
	for(i = 0;i < data.size(); i++)
	{
		sum = 0;
		for(j = 0;j < 24; j++)
		{
			sum+=data[i].num[j];
		}
		sum/=24;
		data[i].ave = sum;
	}
	//將資料輸入檔案中 
	for(i = 0;i < data.size(); i++)
	{
		for(j = 0;j < 10; j++)
		{
			putc(data[i].date[j],fptr2);
		}
		putc(' ',fptr2);
		for(j = 0;j < 4; j++)
		{
			putc(data[i].station[j],fptr2);
		}
		fputs(" PM2.5平均值 = ",fptr2);
		int tmp;
		char r[10];
		tmp = data[i].ave * 1000000;//取到小數點後6位
		for(j = 0;j < 10; j++)
		{
			r[j] = (tmp % 10) + '0';
			tmp/=10;
		}
		
		int flag = 0;
		for(j = 9;j >= 0; j--)
		{
			if(r[j] != '0')
				flag = 1;
			if(j == 5)
				putc('.',fptr2);
			if(j <= 6 && flag == 0)
				putc('0',fptr2);
			else if(flag == 0)
			{
				putc(' ',fptr2);
				continue;	
			}
			if(flag == 1)
				putc(r[j],fptr2);
		}
		putc('\n',fptr2);	
	}
	fclose(fptr2);
	cout << "計算完畢，資料已存入average.txt中(平均值紀錄到小數點後六位)" << endl; 
	printf("\n");
}

vector<int> t;//quick專用 
void quick_r(int left, int right, int len) 
{  
    if(left < right) 
	{  
        int i = left;  
        int j = right + 1;  
        while(1) {  
            while(i+1 < len && data[t[++i]].ave > data[t[left]].ave);//向右   
            while(j-1 > -1 && data[t[--j]].ave < data[t[left]].ave);//向左 
            if(i > j)  
                break;  
            swap(t[i], t[j]);  
        }  
        swap(t[left], t[j]);  
        quick_r(left, j-1, len);  
        quick_r(j+1, right, len);  
    }  
}  
void Quicksort()
{
	cout << "Quick sort：" << endl;
	fptr2 = fopen("Quick.txt","w");//開啟可寫入檔案
	int i, j;
	clock_t t1, t2;
	
	for(i = 0;i < data.size(); i++)
	{
		t.push_back(i);
	}
	
	t1 = clock();//計時 
	//排序 
	quick_r(0, t.size()-1, t.size());
	t2 = clock();
	
	//將排序後資料輸入檔案 
	for(i = 0;i < data.size(); i++)
	{ 
		for(j = 0;j < 10; j++)
		{
			putc(data[t[i]].date[j],fptr2);
		}
		putc(' ',fptr2);
		for(j = 0;j < 4; j++)
		{
			putc(data[t[i]].station[j],fptr2);
		}
		fputs(" PM2.5平均值 = ",fptr2);
		int temp;
		char r[10];
		temp = data[t[i]].ave * 1000000;//取到小數點後5位
		for(j = 0;j < 10; j++)
		{
			r[j] = (temp % 10) + '0';
			temp/=10;
		}
		
		int flag = 0;
		for(j = 9;j >= 0; j--)
		{
			if(r[j] != '0')
				flag = 1;
			if(j == 5)
				putc('.',fptr2);
			if(j <= 6 && flag == 0)
				putc('0',fptr2);
			else if(flag == 0)
			{
				putc(' ',fptr2);
				continue;	
			}
			if(flag == 1)
				putc(r[j],fptr2);
		}
		putc('\n',fptr2);		
	}
	
	fclose(fptr2);
	cout << "排序完畢，資料已存入quick.txt中(平均值紀錄到小數點第六位)" << endl; 
	printf("Quick sort所需時間 %lf secs\n",(t2-t1)/(double)(CLOCKS_PER_SEC));
	printf("\n");
}

vector<int> m;//merge專用
vector<int> tt; 
void merge_r(int start, int end) 
{
	if (start >= end)
		return;
	int len = end - start, mid = (len >> 1) + start;
	int start1 = start, end1 = mid;
	int start2 = mid + 1, end2 = end;
	//分開陣列元素
	merge_r(start1, end1);
	merge_r(start2, end2);
	int k = start;
	//開始合併 
	while (start1 <= end1 && start2 <= end2)
		tt[k++] = data[m[start1]].ave > data[m[start2]].ave ? m[start1++] : m[start2++];
	//放入剩下的陣列
	while (start1 <= end1)
		tt[k++] = m[start1++];
	while (start2 <= end2)
		tt[k++] = m[start2++];
	for (k = start; k <= end; k++)
		m[k] = tt[k];
}

void Mergesort()
{
	cout << "Merge sort：" << endl;
	fptr2 = fopen("Merge.txt","w");//開啟可寫入檔案
	int i, j;
	clock_t t1, t2;
	
	for(i = 0;i < data.size(); i++)
	{
		m.push_back(i);
	}
	for(i = 0;i < data.size(); i++)//暫存 
	{
		tt.push_back(i);
	}
	
	t1 = clock();//計時 
	//排序 
	merge_r(0, m.size()-1);
	t2 = clock();
	
	//將排序後資料輸入檔案 
	for(i = 0;i < data.size(); i++)
	{ 
		for(j = 0;j < 10; j++)
		{
			putc(data[m[i]].date[j],fptr2);
		}
		putc(' ',fptr2);
		for(j = 0;j < 4; j++)
		{
			putc(data[m[i]].station[j],fptr2);
		}
		fputs(" PM2.5平均值 = ",fptr2);
		int temp;
		char r[10];
		temp = data[m[i]].ave * 1000000;//取到小數點後5位
		for(j = 0;j < 10; j++)
		{
			r[j] = (temp % 10) + '0';
			temp/=10;
		}
		
		int flag = 0;
		for(j = 9;j >= 0; j--)
		{
			if(r[j] != '0')
				flag = 1;
			if(j == 5)
				putc('.',fptr2);
			if(j <= 6 && flag == 0)
				putc('0',fptr2);
			else if(flag == 0)
			{
				putc(' ',fptr2);
				continue;	
			}
			if(flag == 1)
				putc(r[j],fptr2);
		}
		putc('\n',fptr2);		
	}
	
	fclose(fptr2);
	cout << "排序完畢，資料已存入Merge.txt中(平均值紀錄到小數點第六位)" << endl; 
	printf("Merge sort所需時間 %lf secs\n",(t2-t1)/(double)(CLOCKS_PER_SEC));
	printf("\n");
}

void Bubblesort()
{
	cout << "Bubble sort：" << endl;
	fptr2 = fopen("Bubble.txt","w");//開啟可寫入檔案
	int i, j;
	vector<int> tmp;
	clock_t t1, t2;
	
	for(i = 0;i < data.size(); i++)
	{
		tmp.push_back(i);
	} 
	
	t1 = clock();//計時 
	//排序處理	
	for(i = 0;i < data.size()-1; i++)
		for(j = 0;j < data.size()-1-i;j++)
			if(data[tmp[j]].ave < data[tmp[j+1]].ave)
				swap(tmp[j], tmp[j+1]);
	t2 = clock();
	//將排序後資料輸入檔案 
	for(i = 0;i < data.size(); i++)
	{ 
		for(j = 0;j < 10; j++)
		{
			putc(data[tmp[i]].date[j],fptr2);
		}
		putc(' ',fptr2);
		for(j = 0;j < 4; j++)
		{
			putc(data[tmp[i]].station[j],fptr2);
		}
		fputs(" PM2.5平均值 = ",fptr2);
		int temp;
		char r[10];
		temp = data[tmp[i]].ave * 1000000;//取到小數點後5位
		for(j = 0;j < 10; j++)
		{
			r[j] = (temp % 10) + '0';
			temp/=10;
		}
		
		int flag = 0;
		for(j = 9;j >= 0; j--)
		{
			if(r[j] != '0')
				flag = 1;
			if(j == 5)
				putc('.',fptr2);
			if(j <= 6 && flag == 0)
				putc('0',fptr2);
			else if(flag == 0)
			{
				putc(' ',fptr2);
				continue;	
			}
			if(flag == 1)
				putc(r[j],fptr2);
		}
		putc('\n',fptr2);		
	}
	fclose(fptr2);
	cout << "排序完畢，資料已存入Bubble.txt中(平均值紀錄到小數點後六位)" << endl; 
	printf("Bubble sort所需時間 %lf secs\n",(t2-t1)/(double)(CLOCKS_PER_SEC));
	printf("\n");
}

void purple()
{
	cout << "紫爆天數排序：" << endl;
	int i, j, k;
	Pur tmp;
	i = 0;
	j = 0;
	while(i < data.size())
	{
		strcpy(tmp.station, data[i].station);
		tmp.day = 0;
		while(j < data.size())
		{
			if(strcmp(data[i].station,data[j].station) == 0)//站名相等 
			{
				if(data[j].ave > 60)
					tmp.day++;
			}
			else//若換到下個站 
			{
				if(tmp.day > 0)
					pur.push_back(tmp);//將資料放入陣列中 
				break;
			}
			j++;
		}
		i = j;
	}
	//排序處理	
	for(i = 0;i < pur.size()-1; i++)
		for(j = 0;j < pur.size()-1-i;j++)
			if(pur[j].day < pur[j+1].day)
				swap(pur[j], pur[j+1]);
	//輸出資料 
	cout<<"地名 天數"<<endl; 
	for(k = 0;k < pur.size(); k++)
		cout<< pur[k].station << " " << pur[k].day << endl;

	printf("\n");
}

int main(int argc, char** argv) 
{
	PMdata();
    average();
    Quicksort();
    Mergesort();
	Bubblesort();
	purple();
	system("pause");
	return 0;
}
	            
