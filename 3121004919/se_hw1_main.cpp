#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;
//读取文件中的字符串
string readsp(ifstream& sp)
{
	string ans;
	char ch;
	while (sp.get(ch))
	{
		//过滤标点符号
		if (ch != ' ' and ch != ',' and ch != '。' and ch != '？' and ch != '!' and ch != '\n' and ch != '“' and ch != '”')
			ans.push_back(ch);
	}
	return ans;
}

/*
	计算编辑距离和文本相似度
	基于编辑距离算法
	返回浮点小数，1表示完全相同
*/
float Countmindis(string t1, string t2)
{
	//获取字符串长度
	int n = t1.size();
	int m = t2.size();
	//cout << endl << n << endl << m << endl;
	//cout << t1 << endl << t2;
	//有一个字符串为空
	if (n * m == 0)
		return 1.0 / (static_cast<float>(n + m) + 1);
	//初始化dp
	vector<vector<int>> dp;
	dp.resize(1, vector<int>(m + 1, 0));
	for (int i = 0; i < m + 1; i++)
	{
		dp[0][i] = i;
	}
	int up, left, up_left;
	for (int i = 1; i < n + 1; i++)
	{
		dp.push_back(vector<int>(m + 1, 0));
		dp[1][0] = i;
		for (int j = 1; j < m + 1; j++)
		{
			up = dp[0][j] + 1;
			left = dp[1][j - 1] + 1;
			up_left = dp[0][j - 1] + (t1[i] != t2[j]);
			dp[1][j] = min({ up,left,up_left });
		}
		dp.erase(dp.begin());
	}
	return 1.0 / (static_cast<float>(dp[0][m]) + 1);
}


//对各个样例测试
void testsp(int argc, char** argv)
{
	//检查命令行参数
	if (argc < 2)
	{
		cout << "can't open file\n";
		return;
	}
	//orig为源文件对象，orig_add为抄袭文件对象，ans为答案文件
	ifstream orig, orig_add;
	ofstream ans;
	//打开文件
	orig.open(argv[1],ios::in);
	orig_add.open(argv[2],ios::in);
	ans.open(argv[3],ios::out);
	//检查文件是否打开成功
	if (not orig.is_open() or not orig_add.is_open() or not ans.is_open())
	{
		cout << "can't open file" << endl;
		exit(EXIT_FAILURE);
	}
	//读取源文本和模式文本，将其转换为string类型
	string orig_s = readsp(orig);
	string orig_add_s = readsp(orig_add);
	//求出文本相似度
	float ans_f = Countmindis(orig_s, orig_add_s);
	//将结果输出到答案文件,结果保留两位小数
	ans << setiosflags(ios::fixed) << setprecision(2) << "文本相似度为：" << ans_f * 100 << "%";
	//关闭文件
	orig.close();
	orig_add.close();
	ans.close();
}
int main(int argc,char* argv[])/*命令行参数*/
{
	testsp(argc, argv);
}
