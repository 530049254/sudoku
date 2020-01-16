#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <istream>
#include <algorithm>
#include <cmath>
#include <time.h>
using namespace std;

string g_row[9] = {     //sudoku template start with 4(1120172119 (10)mod9+1 = 2)
	"234567891",
	"456789123",
	"123456789",
	"789123456",
	"345678912",
	"678912345",
	"912345678",
	"567891234",
	"891234567" };

char g_output[200000000];       //输出
char g_input[200000000];        //输入
int unsolvedSudoku[10][10];     //未解决数独
int rowMark[10][10];            //行状态
int colMark[10][10];            //列状态
int blockMark[10][10];          //区块状态
int row[10], col[10], block[10];//数的数量
int blank[100][3];              //空白
int blankCounter = 0;           //空白数

inline void Write();
inline int BlockNum(int r, int c);       //区块位置计算
inline void Swap(int * a, int * b);
inline void SetMark(int r, int c, int n, bool flag);
void Reset();
bool DFS(int deep);


void Solve(string path)
{
	ifstream ReadFile(path);
	if (!ReadFile.is_open())
	{
		cout << "** 无效路径 **" << endl;
		cout << "检查文件路径" << endl;
		return;
	}
	int counter = 0;
	while (!ReadFile.eof())
	{
		ReadFile >> g_input[counter++];
	}
	ReadFile.close();
	int iPointer = 0;
	int oPointer = 0;

	while (g_input[iPointer])
	{
		for (int r = 1; r < 10; r++)
		{
			for (int c = 1; c < 10; c++)
			{
				unsolvedSudoku[r][c] = g_input[iPointer++] - 48;

				if (unsolvedSudoku[r][c] == 0)   //空白格子处理
				{
					blank[blankCounter][0] = r;
					blank[blankCounter][1] = c;
					blankCounter++;
				}
				else                            //数字处理
				{
					SetMark(r, c, unsolvedSudoku[r][c], 1);
					row[r]++;
					col[c]++;
					block[BlockNum(r, c)]++;
				}
			}
		}

		for (int i = 0; i < blankCounter; i++)
		{
			int r = blank[blankCounter][0], c = blank[blankCounter][1];
			blank[blankCounter][2] = row[r] + col[c] + block[BlockNum(r, c)];
		}

		for (int i = 0; i < blankCounter; i++)   //sort
		{
			int m = i;
			for (int j = i; j < blankCounter - 1; j++)
			{

				if (blank[m][2] < blank[j + 1][2])
					m = j + 1;
			}
			Swap(blank[i], blank[m]);
		}

		if (DFS(0))
		{
			for (int r = 1; r < 10; r++)
			{
				for (int c = 1; c < 10; c++)
				{
					g_output[oPointer++] = unsolvedSudoku[r][c] + '0';
					if (c == 9)
						g_output[oPointer++] = '\n';
					else
						g_output[oPointer++] = ' ';
				}
			}
			g_output[oPointer++] = '\n';
		}
		Reset();
	}
	Write();
}


void Create(int & n)
{
	int arr[] = { 2,1,3,4,5,6,7,8,9 };    //默认转换顺序
	int order[] = { 0,1,2,3,4,5,6,7,8 };    //默认写入顺序
	int trans[9];                        //数字交换表
	int newRow[9][9];                    //新生成的行
	int tempPointer = 0;
	do
	{
		for (int i = 0; i < 9; ++i)        //生成数字交换表
			trans[g_row[0][i] - 49] = arr[i];

		for (int i = 0; i < 9; ++i)        //生成新的终局
			for (int j = 0; j < 9; ++j)
				newRow[i][j] = trans[g_row[i][j] - 49];

		for (int i = 0; i < 2 && n; i++)    //交换行
		{
			for (int j = 0; j < 6 && n; j++)
			{
				for (int k = 0; k < 6 && n; k++)
				{
					for (int m = 0; m < 9; ++m)
					{
						for (int n = 0; n < 9; ++n)
						{
							g_output[tempPointer++] = newRow[order[m]][n] + '0';
							if (n == 8)
								g_output[tempPointer++] = '\n';
							else
								g_output[tempPointer++] = ' ';
						}
					}
					if (--n)
						g_output[tempPointer++] = '\n';
					else
						return;
					next_permutation(order + 6, order + 9);
				}
				next_permutation(order + 3, order + 6);
			}
			next_permutation(order + 1, order + 3);
		}
	} while (next_permutation(arr + 1, arr + 9));    //改变数字交换顺序
	return;
}

int main(int argc, char *argv[])
{
	clock_t start, finish;
	start = clock();

	if (argc != 3)                  //检查参数数量
	{
		cout << "** 无效的参数 **" << endl;
//		cout << "If the file path contains spaces:" << endl;
//		cout << "1.Use backslash before spaces as escape character" << endl;
//		cout << "2.Quotes the path with double quotation mark" << endl;
//		return 0;
	}

	if (strcmp(argv[1], "-c") && strcmp(argv[1], "-s")) //检查命令类型
	{
		cout << "** 无效的命令 **" << endl;
//		cout << "This applicaiton only supports create(-c) and solve(-s) functions!" << endl;
		return 0;
	}

	switch (argv[1][1])
	{
	case 'c':
	{
		int num = atoi(argv[2]);
		if (num <= 0 || strlen(argv[2]) != int(log10(num)) + 1 || num > 1000000)//检查范围
		{
			cout << "** 无效输入 **" << endl;
//			cout << "Make sure the number is in the range of 1-1,000,000!" << endl;
			return 0;
		}
		else
		{
			Create(num);
			Write();
		}
		break;
	}

	case 's':
	{
		Solve(argv[2]);
		break;
	}
	}

	finish = clock();
	cout << "耗时 ： " << double(finish - start) / CLOCKS_PER_SEC << "s" << endl;

	return 0;
}


inline int BlockNum(int r, int c)
{
	return ((r - 1) / 3) * 3 + ((c - 1) / 3);
}

inline void SetMark(int r, int c, int n, bool flag)
{
	rowMark[r][n] = flag;
	colMark[c][n] = flag;
	blockMark[BlockNum(r, c)][n] = flag;
}

inline void Swap(int * a, int * b)
{
	int temp[3];
	temp[0] = a[0];
	temp[1] = a[1];
	temp[2] = a[2];

	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];

	b[0] = temp[0];
	b[1] = temp[1];
	b[2] = temp[2];
	return;
}

void Reset()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			rowMark[i][j] = 0;
			colMark[i][j] = 0;
			blockMark[i][j] = 0;
			unsolvedSudoku[i][j] = 0;
		}
		row[i] = 0;
		col[i] = 0;
		block[i] = 0;
	}
	blankCounter = 0;
	for (int i = 0; i < 100; ++i)
	{
		blank[i][0] = 0;
		blank[i][1] = 0;
		blank[i][2] = 0;
	}
	return;
}

bool DFS(int deep)
{
	if (deep == blankCounter)                       
	{
		return true;
	}

	int r = blank[deep][0], c = blank[deep][1];
	for (int i = 1; i < 10; i++)
	{
		if (!rowMark[r][i] && !colMark[c][i] && !blockMark[BlockNum(r, c)][i])
		{
			unsolvedSudoku[r][c] = i;
			SetMark(r, c, unsolvedSudoku[r][c], 1); 
			if (DFS(deep + 1))return true;
			SetMark(r, c, unsolvedSudoku[r][c], 0); 
			unsolvedSudoku[r][c] = 0;
		}
	}
	return false;
}

void Write()
{
	remove("sudoku.txt");
	ofstream WriteFile("sudoku.txt");
	WriteFile << g_output;
}