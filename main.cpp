
//班级：计科四班  
//姓名：史二美   
//学号：1320170214 ，即在生成数独时左上角第一个数字为（1+4）%9+1=6  

#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<algorithm>
#include"getopt.h"    //注意使用方法
using namespace std;

int space[9][9];
bool flag;
FILE* input, *output_solve;


//生成数独
//基本想法：数独的规则是每行、每列、每个小九宫格内不出现重复的数字，其实只要严格遵守了这些规则然后按照一定的次序打乱即可。
//经过分析可知只需将a在1、4、7行分别放置在1、2、3列的位置，然后2、5、8行将列数加3, 3、6、9行列数加6，然后依次排列a~i这些数字即可。
//题目中要求左上角第一个固定为某个特定值，那么一共可以有8！（40320）种组合。
//将小九宫格内任意两行或两列交换仍可使整个大九宫格满足要求（当然这里除了第一行第一列不可以交换，因为左上角数值固定）。为满足题目要求达到100, 0000种终局，只交换4~6、7~9行（列）即可，这样每种终局可衍生出(2 * C23)*(2 * C23)共36种终局，36 * 40320>100, 0000种终局。

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
			char num[10] = "612354789";                //学号为1320170214，九宫格第一行第一列的数字为6
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


//解数独部分：
//基本思路就是一个DFS的过程，用apace[9][9]数组代表整个九宫格，从0~80依次编号，则第x个数在数组中的位置是apace[x/9][x%9]，然后只需判断这个位置是不是空，如果是空则放置某个数，若该数合法则递归下一层，x = x+1；若该数不为空则直接递归下一层x = x+1。
//在退出上一层递归时要将该位置数字“清零”，以免影响下一次放置。


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






