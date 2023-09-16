#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
using namespace std;
//读取文件中的字符串
string Read_File(ifstream& sp)
{
	string ans = "";
	char ch;
	while (sp.get(ch))
	{
		//过滤标点符号
		if (ch != ' ' and ch != '，' and ch != '。' and ch != '？' and ch != '!' and ch != '\n' and ch != '“' and ch != '”')
			ans.push_back(ch);
	}
	return ans;
}

//对字符串进行分词，统计分词词频
map<string, int> String_Cut(const string& t)
{
	map<string, int> m;
	//每4个字符作为一个分词组
	for (int i = 0; i < t.size() - 4; i += 4)
	{
		string temp = t.substr(i, 4);
		m[temp]++;
	}
	return m;
}

//计算余弦相似度
float Cos_Similarity(const vector<int>& v1, const vector<int>& v2)
{
	float x_y = 0, x_x = 0, y_y = 0;
	for (int i = 0; i < v1.size() and i < v2.size(); i++)
	{
		x_y += v1[i] * v2[i];
		x_x += pow(v1[i], 2);
		y_y += pow(v2[i], 2);
	}
	return x_y / (sqrt(x_x) * sqrt(y_y));
}

//对各个样例测试
void Test_Sample(int argc, char** argv)
{
	//检查命令行参数
	if (argc < 2)
	{
		cout << "can't open file" << endl;
		return;
	}
	//orig为源文件对象，orig_add为抄袭文件对象，ans为答案文件
	ifstream orig, orig_add;
	ofstream ans;
	//打开文件
	orig.open(argv[1], ios::in);
	orig_add.open(argv[2], ios::in);
	ans.open(argv[3], ios::out);
	//检查文件是否打开成功
	if (not orig.is_open() or not orig_add.is_open() or not ans.is_open())
	{
		cout << "can't open file" << endl;
		exit(EXIT_FAILURE);
	}
	//读取源文本和模式文本，将其转换为string类型
	string orig_s = Read_File(orig);
	string orig_add_s = Read_File(orig_add);
	//对两个字符串进行分词
	if (orig_s == "" or orig_add_s == "")
	{
		cout << "至少一个文本为空，无法查重！" << endl;
		return;
	}
	auto m1 = String_Cut(orig_s);
	auto m2 = String_Cut(orig_add_s);
	//计算词频
	vector<int> v1, v2;
	set<string> s;
	for (auto i = m1.begin(); i != m1.end(); i++)
	{
		s.insert(i->first);
	}
	for (auto i = m2.begin(); i != m2.end(); i++)
	{
		s.insert(i->first);
	}
	for (auto i = s.begin(); i != s.end(); i++)
	{
		if (0 == m1.count(*i))
		{
			v1.push_back(0);
		}
		else
		{
			v1.push_back(m1[*i]);
		}
		if (0 == m2.count(*i))
		{
			v2.push_back(0);
		}
		else
		{
			v2.push_back(m2[*i]);
		}
	}
	//求出文本相似度
	float ans_f = Cos_Similarity(v1, v2);
	//将结果输出到答案文件,结果保留两位小数
	ans << setiosflags(ios::fixed) << setprecision(2) << "文本相似度为：" << ans_f * 100 << "%";
	//关闭文件
	orig.close();
	orig_add.close();
	ans.close();
}

int main(int argc, char* argv[])/*命令行参数*/
{
	Test_Sample(argc, argv);
}
