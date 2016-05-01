// Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LZW_decipher.h"
#include "LZW_encipher.h"
#include <atlimage.h>
#include <cstdio>
#include <string>
CImage m_Image;

int m_height, m_width, m_Length;
const static int MAXLEN = 3003000;
vector<uint> ciphertext;
FILE *fp;
char m_text[MAXLEN];
char password[256];
string extensions[] = { "bmp","gif","jpg","jpeg","png" };
string get_extension(string s)
{
	int p = s.size() - 1;
	while (p >= 0 && s[p] != '.') p--;
	if (p < 0) return "";
	return s.substr(p + 1);
}
string Lower(string s)
{
	for (auto &x : s) if ('A' <= x && x <= 'Z') x = x - 'a' + 'A';
	return s;
}
bool isImage(string s)
{
	string t = Lower(get_extension(s));
	for (string x : extensions) if (x == t) return true;
	return false;
}
void ReadImage(CString S)
{
	auto hResult = m_Image.Load(S);
	puts("OK...");
	if (FAILED(hResult)) {
		puts("调用图像文件失败！ ");
		return ;
	}
	auto p = &m_Image;
	printf("图片高度：%d   图片宽度：%d\n", p->GetHeight(), p->GetWidth());

	int W = p->GetWidth(), H = p->GetHeight();
	m_height = H, m_width = W;
	m_Length = 3 * W * H;
	if (m_Length > MAXLEN)
	{
		printf("图片太大，不能加密！\n");
		return;
	}
	m_text[m_Length] = 0;
	int count = 0;
	for (int i = 0; i < W; i++)
	{
		for (int j = 0; j < H; j++)
		{
			auto x = p->GetPixel(i, j);
			char y = GetRValue(x);
			m_text[count++] = y;
			y = GetGValue(x);
			m_text[count++] = y;
			y = GetBValue(x);
			m_text[count++] = y;
		}
	}
}
void out(CString str)
{
	int len = str.GetLength();
	for (int i = 0; i < len; i++) putchar(str.GetAt(i));
	putchar(10);
}
CString Copy(string s)
{
	CString str;
	for (auto x : s) str += x;
	return str;
}
int main(int argc, char **argv)
{
	printf("输入要加密的文本文件或图像文件的文件名：\n");
	string name; cin >> name;
	if (isImage(name))
	{		
		ReadImage(Copy(name));
	}
	
	printf("输入密码：\n");
	cin >> password;

	printf("开始加密！明文长度：%d\n", m_Length);
	double time_start = clock() / (double)CLOCKS_PER_SEC;
	ciphertext = LZW_encipher(m_text, m_Length, "password", 10);
	double time_end = clock() / (double)CLOCKS_PER_SEC;
	printf("加密完成！ 密文长度：%d   加密用时：%.1f秒\n", ciphertext.size(), time_end - time_start);
	
	m_Image.Destroy();
	m_Image.Create(m_width, m_height, 32,  CImage::createAlphaChannel);
	puts("创建成功！！！！");
	int c = 0;
	for (int i = 0; i < m_width; i++)
		for (int j = 0; j < m_height; j++)
		{
			unsigned char r, g, b;
			r = m_text[c++];
			g = m_text[c++];
			b = m_text[c++];
			m_Image.SetPixelRGB(i, j, r, g, b);
		}
	m_Image.Save(_T("output.jpg"));
    return 0;
}

