#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <time.h>
using namespace std;

struct def{
	char date[20];//���
	char station[5];//����
	int num[24];//24�p�ɸ�� 
	double ave;//���� 
};
typedef struct def Data;
struct purple{
	char station[5];//���W 
	int day;//�Ѽ� 
};
typedef struct purple Pur;
vector<Data> data;
vector<Pur> pur;
char s[1000];
FILE *fptr1 = NULL, *fptr2 = NULL;

void PMdata()
{
	cout << "�U�aPM2.5��ƳB�z�G" << endl;
	cout << "�d�U�U�a�C�p��PM2.5��ơA�Y�ƾڸ�Ƥ��X�{�D�Ʀr�r���A�Ӥp��PM2.5�ƭȬ�0" << endl; 
	fptr1 = fopen("input.txt","r");//�}�ҥiŪ���ɮ�
	fptr2 = fopen("PM2.5data.txt","w");//�}�ҥi�g�J�ɮ� 
	if(fptr1 == NULL)
	{
		printf("Error opening this file!\n");
		exit(0);
	}
	
	fgets(s, 1000, fptr1);//���D��
	fputs(s,fptr2); 
	while (fgets(s,1000,fptr1) != NULL)//�ɮפ��٦����  
	{
		int i = 0, j = 0, k = 0;
		
		//��XPM2.5����� 
		if(s[16] == 'P' && s[17] == 'M' && s[18] == '2' && s[19] == '.' && s[20] == '5')
		{	
			Data temp;
			while(i < 10)//�x�s��� 
			{
				temp.date[j] = s[i];
				putc(s[i],fptr2);
				i++;
				j++;
			}
		 	putc(',',fptr2);
			i++;//���L','
			j = 0;
			while(s[i] != ',')//�x�s�����W�� 
			{
				temp.station[j] = s[i];
				putc(s[i],fptr2);
				i++;
				j++;
			}
			temp.station[j] = '\0';
			putc(',',fptr2);
			i++;//���L','
			fputs("PM2.5",fptr2);
			while(s[i] != ',')//���LPM2.5 
				i++;
			putc(',',fptr2);
			i++;//���L','
			for(j = 0;j < 24; j++)//��J�Ʀr
			{
				int sum = 0;
				int  flag = 0;
				while(s[i] != ',')
				{
					int f = 0;
					int h;
					h = i;
					while(s[h] != ',')//���ˬd�@���Ʀr�O�_���Ÿ� 
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
					if(f == 1)//�Ʀr���Ÿ��A�]��0�ø��L�� 
					{
						while(s[i] != ',')
							i++;
						break;
					}
					else if(s[i] == '\0')
						break;
					else if(s[i] >= '0' && s[i] <= '9')//���r���O�Ʀr�ɤ~��J�A�_�h���L 
					{
						sum = sum*10 + s[i]-'0';
						putc(s[i],fptr2);//�g�J�ɮ� 
						flag = 1;
					}
					i++; 
				}
				//�B�z�L�ƭȪ���� 
				if(!flag)
					putc('0',fptr2);
				if(j != 23)
				{
					putc(',',fptr2);
					i++;//���L','
				}	
				temp.num[j] = sum;				
			}
			putc('\n',fptr2);//�b�ɮפ�����
				
			data.push_back(temp);
		}		
    }
    
    fclose(fptr1);
	fclose(fptr2);
	cout << "���Ū�������APM2.5��Ƥw�s�JPM2.5data.txt��" << endl; 
	printf("\n");
}
void average()
{
	cout << "�p��U�a�����ȡG" << endl;
	fptr2 = fopen("average.txt","w");//�}�ҥi�g�J�ɮ�
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
	//�N��ƿ�J�ɮפ� 
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
		fputs(" PM2.5������ = ",fptr2);
		int tmp;
		char r[10];
		tmp = data[i].ave * 1000000;//����p���I��6��
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
	cout << "�p�⧹���A��Ƥw�s�Javerage.txt��(�����Ȭ�����p���I�᤻��)" << endl; 
	printf("\n");
}

vector<int> t;//quick�M�� 
void quick_r(int left, int right, int len) 
{  
    if(left < right) 
	{  
        int i = left;  
        int j = right + 1;  
        while(1) {  
            while(i+1 < len && data[t[++i]].ave > data[t[left]].ave);//�V�k   
            while(j-1 > -1 && data[t[--j]].ave < data[t[left]].ave);//�V�� 
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
	cout << "Quick sort�G" << endl;
	fptr2 = fopen("Quick.txt","w");//�}�ҥi�g�J�ɮ�
	int i, j;
	clock_t t1, t2;
	
	for(i = 0;i < data.size(); i++)
	{
		t.push_back(i);
	}
	
	t1 = clock();//�p�� 
	//�Ƨ� 
	quick_r(0, t.size()-1, t.size());
	t2 = clock();
	
	//�N�Ƨǫ��ƿ�J�ɮ� 
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
		fputs(" PM2.5������ = ",fptr2);
		int temp;
		char r[10];
		temp = data[t[i]].ave * 1000000;//����p���I��5��
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
	cout << "�Ƨǧ����A��Ƥw�s�Jquick.txt��(�����Ȭ�����p���I�Ĥ���)" << endl; 
	printf("Quick sort�һݮɶ� %lf secs\n",(t2-t1)/(double)(CLOCKS_PER_SEC));
	printf("\n");
}

vector<int> m;//merge�M��
vector<int> tt; 
void merge_r(int start, int end) 
{
	if (start >= end)
		return;
	int len = end - start, mid = (len >> 1) + start;
	int start1 = start, end1 = mid;
	int start2 = mid + 1, end2 = end;
	//���}�}�C����
	merge_r(start1, end1);
	merge_r(start2, end2);
	int k = start;
	//�}�l�X�� 
	while (start1 <= end1 && start2 <= end2)
		tt[k++] = data[m[start1]].ave > data[m[start2]].ave ? m[start1++] : m[start2++];
	//��J�ѤU���}�C
	while (start1 <= end1)
		tt[k++] = m[start1++];
	while (start2 <= end2)
		tt[k++] = m[start2++];
	for (k = start; k <= end; k++)
		m[k] = tt[k];
}

void Mergesort()
{
	cout << "Merge sort�G" << endl;
	fptr2 = fopen("Merge.txt","w");//�}�ҥi�g�J�ɮ�
	int i, j;
	clock_t t1, t2;
	
	for(i = 0;i < data.size(); i++)
	{
		m.push_back(i);
	}
	for(i = 0;i < data.size(); i++)//�Ȧs 
	{
		tt.push_back(i);
	}
	
	t1 = clock();//�p�� 
	//�Ƨ� 
	merge_r(0, m.size()-1);
	t2 = clock();
	
	//�N�Ƨǫ��ƿ�J�ɮ� 
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
		fputs(" PM2.5������ = ",fptr2);
		int temp;
		char r[10];
		temp = data[m[i]].ave * 1000000;//����p���I��5��
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
	cout << "�Ƨǧ����A��Ƥw�s�JMerge.txt��(�����Ȭ�����p���I�Ĥ���)" << endl; 
	printf("Merge sort�һݮɶ� %lf secs\n",(t2-t1)/(double)(CLOCKS_PER_SEC));
	printf("\n");
}

void Bubblesort()
{
	cout << "Bubble sort�G" << endl;
	fptr2 = fopen("Bubble.txt","w");//�}�ҥi�g�J�ɮ�
	int i, j;
	vector<int> tmp;
	clock_t t1, t2;
	
	for(i = 0;i < data.size(); i++)
	{
		tmp.push_back(i);
	} 
	
	t1 = clock();//�p�� 
	//�ƧǳB�z	
	for(i = 0;i < data.size()-1; i++)
		for(j = 0;j < data.size()-1-i;j++)
			if(data[tmp[j]].ave < data[tmp[j+1]].ave)
				swap(tmp[j], tmp[j+1]);
	t2 = clock();
	//�N�Ƨǫ��ƿ�J�ɮ� 
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
		fputs(" PM2.5������ = ",fptr2);
		int temp;
		char r[10];
		temp = data[tmp[i]].ave * 1000000;//����p���I��5��
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
	cout << "�Ƨǧ����A��Ƥw�s�JBubble.txt��(�����Ȭ�����p���I�᤻��)" << endl; 
	printf("Bubble sort�һݮɶ� %lf secs\n",(t2-t1)/(double)(CLOCKS_PER_SEC));
	printf("\n");
}

void purple()
{
	cout << "���z�ѼƱƧǡG" << endl;
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
			if(strcmp(data[i].station,data[j].station) == 0)//���W�۵� 
			{
				if(data[j].ave > 60)
					tmp.day++;
			}
			else//�Y����U�ӯ� 
			{
				if(tmp.day > 0)
					pur.push_back(tmp);//�N��Ʃ�J�}�C�� 
				break;
			}
			j++;
		}
		i = j;
	}
	//�ƧǳB�z	
	for(i = 0;i < pur.size()-1; i++)
		for(j = 0;j < pur.size()-1-i;j++)
			if(pur[j].day < pur[j+1].day)
				swap(pur[j], pur[j+1]);
	//��X��� 
	cout<<"�a�W �Ѽ�"<<endl; 
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
	            
