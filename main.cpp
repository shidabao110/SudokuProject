
//�༶���ƿ��İ�  
//������ʷ����   
//ѧ�ţ�1320170214 ��������������ʱ���Ͻǵ�һ������Ϊ��1+4��%9+1=6  

#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<algorithm>
#include"getopt.h"    //ע��ʹ�÷���
using namespace std;

int space[9][9];
bool flag;
FILE* input, *output_solve;


//��������
//�����뷨�������Ĺ�����ÿ�С�ÿ�С�ÿ��С�Ź����ڲ������ظ������֣���ʵֻҪ�ϸ���������Щ����Ȼ����һ���Ĵ�����Ҽ��ɡ�
//����������ֻ֪�轫a��1��4��7�зֱ������1��2��3�е�λ�ã�Ȼ��2��5��8�н�������3, 3��6��9��������6��Ȼ����������a~i��Щ���ּ��ɡ�
//��Ŀ��Ҫ�����Ͻǵ�һ���̶�Ϊĳ���ض�ֵ����ôһ��������8����40320������ϡ�
//��С�Ź������������л����н����Կ�ʹ������Ź�������Ҫ�󣨵�Ȼ������˵�һ�е�һ�в����Խ�������Ϊ���Ͻ���ֵ�̶�����Ϊ������ĿҪ��ﵽ100, 0000���վ֣�ֻ����4~6��7~9�У��У����ɣ�����ÿ���վֿ�������(2 * C23)*(2 * C23)��36���վ֣�36 * 40320>100, 0000���վ֡�

void create(int n)
{
	FILE* output_create;
	output_create = fopen("sudoku.txt", "w");
	if (!output_create)
	{
		printf("CANNOT open the output file!\n");
		exit(1);
	}
	int record[9] = { 0,3,6,1,4,7,2,5,8 };
	for (int i = 0; i < 6 && n; i++)
	{
		if (i)
			next_permutation(record + 3, record + 6);
		for (int j = 0; j < 6 && n; j++)
		{
			if (j) next_permutation(record + 6, record + 9);
			char num[10] = "612354789";                //ѧ��Ϊ1320170214���Ź����һ�е�һ�е�����Ϊ6
			for (int k = 0; k < 40320 && n; k++)
			{
				if (k)next_permutation(num + 1, num + 9);
				for (int m = 0; m < 9; m++)
				{
					fputc(num[record[m] % 9], output_create);
					for (int n = 1; n < 9; n++)
					{
						fputc(' ', output_create);
						fputc(num[(n + record[m]) % 9], output_create);
					}
					fputc('\n', output_create);
				}
				if (--n) fputc('\n', output_create);
			}
		}
		record[6] = 2, record[7] = 5, record[8] = 8;
	}
	fclose(output_create);
}






bool check_legality(int x, int m, int n)        //
{
	for (int i = 0; i < 9; i++)
		if (space[m][i] == x) return false;
	for (int i = 0; i < 9; i++)
		if (space[i][n] == x) return false;
	int begin_i = m / 3 * 3;
	int begin_j = n / 3 * 3;
	for (int i = begin_i; i < begin_i + 3; i++)
		for (int j = begin_j; j < begin_j + 3; j++)
			if (space[i][j] == x) return false;
	return true;
}


//���������֣�
//����˼·����һ��DFS�Ĺ��̣���apace[9][9]������������Ź��񣬴�0~80���α�ţ����x�����������е�λ����apace[x/9][x%9]��Ȼ��ֻ���ж����λ���ǲ��ǿգ�����ǿ������ĳ�������������Ϸ���ݹ���һ�㣬x = x+1����������Ϊ����ֱ�ӵݹ���һ��x = x+1��
//���˳���һ��ݹ�ʱҪ����λ�����֡����㡱������Ӱ����һ�η��á�


void solve(int count)         
{
	if (flag) return;
	if (count == 81)
	{
		flag = true;
		for (int i = 0; i < 9; i++)
		{
			fputc(space[i][0] + '0', output_solve);
			for (int j = 1; j < 9; j++)
			{
				fputc(' ', output_solve);
				fputc(space[i][j] + '0', output_solve);
			}
			fputc('\n', output_solve);
		}
		fputc('\n', output_solve);
		return;
	}
	int m = count / 9;
	int n = count % 9;
	if (space[m][n] == 0)
	{
		for (int i = 1; i <= 9; i++)
			if (check_legality(i, m, n))
			{
				space[m][n] = i;
				solve(count + 1);
			}
		space[m][n] = 0;
	}
	else solve(count + 1);
}

inline int to_integer(char* arg)
{
	int ret = 0;
	while (*arg)
	{
		if (!isdigit(*arg)) return -1;
		ret = ret * 10 + *arg - '0';
		arg++;
	}
	return ret;
}

int main(int argc, char* argv[])
{
	int opt = getopt(argc, argv, "c:s:");
	if (opt == 'c')
	{

		int n;
		n = to_integer(optarg);
		if (n > 0 && n <= 1000000)
			create(n);
		else printf("Please input a NUMBER from 1 to 1000000!\n");
	}
	else if (opt == 's')
	{
		input = fopen(optarg, "r");
		output_solve = fopen("sudoku.txt", "w");
		if (!input)
		{
			printf("CANNOT find the input file!\n");
			exit(1);
		}
		if (!output_solve)
		{
			printf("CANNOT open the output file!\n");
			exit(1);
		}
		while (fscanf(input, "%d", &space[0][0]) != EOF)
		{
			for (int j = 1; j < 9; j++)
				fscanf(input, "%d", &space[0][j]);
			for (int i = 1; i < 9; i++)
				for (int j = 0; j < 9; j++)
					fscanf(input, "%d", &space[i][j]);
			flag = false;
			solve(0);
		}
		fclose(input);
		fclose(output_solve);
	}
	else printf("INPUT ERROR!\n");
	return 0;
}






