#ifndef __Compress_H_
#define __Compress_H_

#include <string>
typedef long long LongType;

struct CharInfo
{
	unsigned char _ch;//字符
	LongType _count;//出现次数
	string _code;//哈夫曼编码
	CharInfo(unsigned char ch = '0')
		:_ch(ch)
		, _count(0)
		, _code("")
	{}
	CharInfo(LongType count)
		:_count(count)
		, _ch(0)
		, _code("")
	{}
	//重载运算符“>”
	bool operator >(const CharInfo& com)const
	{
		return _count > com._count;
	}
	//重载运算符“!=”
	bool operator !=(const CharInfo com)const 
	{
		return _count != com._count;
	}
	//重载运算符“+”
	CharInfo operator+(const CharInfo& com)const
	{
		return CharInfo(_count + com._count);
	}
};

class FileCompress
{
public:
	//压缩
	FileCompress()
	{
		for (size_t i = 0; i < 256; i++)
		{
			_info[i]._ch = i;
		}
	}
	void Compress(const char* file)
	{
		//读取文件
		FILE *fout = fopen(file, "rb");
		unsigned char ch = 0;
		assert(fout);
		//统计每个字符出现的次数
		ch = fgetc(fout);
		while (!feof(fout))
		{
			_info[(unsigned char)ch]._count++;
			ch = fgetc(fout);
		}
		//用次数建立哈夫曼树
		const CharInfo invalid((unsigned char)0);
		HaffmanTree<CharInfo> tree(_info, 256,invalid);
		//生成哈夫曼编码
		string tmp;
		_CreateHaffmanCode(tree.GetRoot(), tmp);
		//压缩
		fseek(fout, 0, SEEK_SET);//从文件的首字符开始压缩
		//压缩后的文件名
		string comfile = file;
		comfile += ".haffman";
		FILE *fin = fopen(comfile.c_str(), "wb");
		assert(fin);
		unsigned char value = 0;
		size_t index = 0;//标记当前位
		ch = fgetc(fout);
		while (!feof(fout))
		{
			string code = _info[ch]._code;
			for (size_t i = 0; i < code.size(); i++)
			{
				if (code[i] == '1')
				{
					value <<= 1;
					value |= 1;
				}
				else
				{
					value <<= 1;
				}
				//满8个字节，将其写入到压缩文件中
				if (++index == 8)
				{
						fputc(value, fin);
					    value = 0;
					     index = 0;
				}
			}
			ch = fgetc(fout);
		}
		//如果写入完，value 没有写满8位，把剩余的压入压缩文件中
		if (index != 0)
		{
			value <<= (8 - index);
			fputc(value, fin);
		}
		//配置文件
		string configfile = file;
		configfile += ".config";
		FILE* finfo = fopen(configfile.c_str(), "wb");
		assert(finfo);
		//将字符的总个数写进配置文件
		char infostr[32];
		//将出现的字符以及次数写进配置文件
		string str;
		for (size_t j = 0; j < 256; j++)
		{
				if (_info[j]._count > 0)
				{
					str.push_back((unsigned char)j);
					str.push_back(',');
					_itoa(_info[j]._count, infostr, 10);
					str += infostr;
					fputs(str.c_str(), finfo);
					fputc('\n', finfo);
					str.clear();
				}
		}
		fclose(fout);
		fclose(fin);
		fclose(finfo);
	}
	//解压
	void UnCompress(const char *file)
	{
		unsigned char ch = 0;
		string fconfig = file;
		fconfig += ".config";
		//读压缩文件
		FILE* fout = fopen(fconfig.c_str(), "rb");
		assert(fout);
		string tmp;
		//字符出现的次数
		while (ReadLine(fout, tmp))
		{
			if (!tmp.empty())
			{
				//那到字符
				ch = tmp[0];
				//获取字符的次数
				_info[(unsigned char)ch]._count = atoi(tmp.substr(2).c_str());
				tmp.clear();
			}
			else
			{
				tmp = '\n';
			}
		}
		//重建哈夫曼树
		CharInfo invalid((unsigned char)0);
		HaffmanTree<CharInfo> h(_info, 256, invalid);
		HaffmanTreeNode<CharInfo>*root = h.GetRoot();
		//解压
		string output = file;
		output += ".uncom";
		FILE* fin = fopen(output.c_str(), "wb");
		assert(fin);
		//根据压缩文件，还原文件
		string Haffmanfile = file;
		Haffmanfile += ".haffman";
		FILE* fcom = fopen(Haffmanfile.c_str(), "rb");
		assert(fcom);
		HaffmanTreeNode<CharInfo>*cur = root;
		ch = fgetc(fcom);
		int pos = 8;
		LongType len = root->_weight._count;
		while (len > 0)
		{
			while (cur->_left &&cur->_right)
			{
				if (pos == 0)
				{
					ch = fgetc(fcom);
					pos = 8;
				}
				--pos;
				//与1，走右树
				if (ch&(1 << pos))
				{
					cur = cur->_right;
				}
				//与0，走左树
				else
				{
					cur = cur->_left;
				}
			}
			if (cur)
			{
				fputc(cur->_weight._ch, fin);//写进解压文件
				cur = root;
			}
			--len;
		}
		fclose(fout);
		fclose(fin);
		fclose(fcom);
	}
protected:
	//创建哈夫曼编码
	void _CreateHaffmanCode(HaffmanTreeNode<CharInfo>*root, string tmp)
	{
		//判空
		if (root == NULL)
		{
			return;
		}
		if (root->_left == NULL&&root->_right == NULL)
		{
			//找到下标，把编码写到_code中
			int index = (root->_weight)._ch;
			_info[index]._code = tmp;
		}
		else
		{
			//左树写0
			if (root->_left)
			{
				tmp.push_back('0');
				_CreateHaffmanCode(root->_left, tmp);
				//tmp.pop_back();
			}
			//右树写1
			if (root->_right)
			{
				tmp.push_back('1');
				_CreateHaffmanCode(root->_right, tmp);
				//tmp.pop_back();
			}
		}
	}
	//读一行
	bool ReadLine(FILE* file, string& tmp)
	{
		assert(file);
		char ch = fgetc(file);
		if (feof(file))
		{
			return false;
		}
		while (ch != '\n')
		{
			tmp += ch;
			ch = fgetc(file);
		}
		return true;
	}
protected:
	CharInfo _info[256];
};
#endif //__Compress_H_
