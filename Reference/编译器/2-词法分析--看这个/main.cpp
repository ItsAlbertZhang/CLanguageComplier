#include <Windows.h>
#include "complier.h"

TkWord *tk_HashTable[MAXKEY] = { 0 };//���ʵĹ�ϣ��
//����ַ����Ķ�̬����
vector<string> word;
//���handle��Ϊ����ɫ��ӡ
HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
//��ÿһ�з��Ž���token���룬���ж�Ӧ������ֵ
static TkWord keywords[]={
	{TK_PLUS,"+",NULL},
	{TK_MINUS,"-",NULL},
	{TK_STAR,"*",NULL},
	{TK_DIVIDE,"/",NULL},
	{TK_MOD,"%",NULL},
	{TK_EQ,"==",NULL},
	{TK_NEQ,"!=",NULL},
	{TK_LT,"<",NULL},
	{TK_LEQ,"<=",NULL},
	{TK_GT,">",NULL},
	{TK_GEQ,">=",NULL},
	{TK_ASSIGN,"=",NULL},
	{TK_POINTSTO,"->",NULL},
	{TK_DOT,".",NULL},
	{TK_AND,"&",NULL},
	{TK_OPENPA,"(",NULL},
	{TK_CLOSEPA,")",NULL},
	{TK_OPENBR,"[",NULL},
	{TK_CLOSEBR,"]",NULL},
	{TK_BEGIN,"{",NULL},
	{TK_END,"}",NULL},
	{TK_SEMICOLON,";",NULL},
	{TK_COMMA,",",NULL},
	{TK_ELLIPSIS,"...",NULL},
	{TK_EOF,"End Of File",NULL},

	{TK_CINT,"���γ���",NULL},
	{TK_CFLOAT,"�����ͳ���",NULL},
	{TK_CCHAR,"�ַ�����",NULL},
	{TK_CSTR,"�ַ�������",NULL},

	{KW_CHAR,"char",NULL},
	{KW_SHORT,"short",NULL},
	{KW_INT,"int",NULL},
	{KW_VOID,"void",NULL},
	{KW_STRUCT,"struct",NULL},

	{KW_IF,"if",NULL},
	{KW_ELSE,"else",NULL},
	{KW_FOR,"for",NULL},
	{KW_CONTINUE,"continue",NULL},
	{KW_BREAK,"break",NULL},
	{KW_RETURN,"return",NULL},
	{40}//TK_IDENT
};

//���ƣ�����̣ܶ�����
//���ƣ���ʵ����������������д�﷨������
int main(){
	int keywordsLen = sizeof(keywords) / sizeof(TkWord);
	InitKeywords(keywords, keywordsLen, tk_HashTable);//��ʼ����ϣ��
	ifstream file;
	file.open("test.txt",ios::in);//FILE *fp=fopen("test.txt","r+");
	if(!file.is_open()){//�ļ���ʧ�ܣ�����
		return 0;
	}
	string str;//char str[1000];//��һ���ַ���
	
	while(getline(file,str)){//һ�ζ�ȡһ�У�fgets(str,sizeof(str),fp)
		if(str.empty()){
			continue;
		}
		word.clear();
		//split�ǰ�ÿһ�а���Ϊһ��һ���ĵ���
		int err_num=split(word,str);
		for(int i=0;i<word.size();i++){
			string temp=word.at(i);
			//cout<<temp;
			pTkWord cur=tk_HashTable[elf_hash(temp)];
			if(NULL!=cur&&cur->spelling==temp){//����hash table�е�����
				tokencode token=(tokencode)cur->tkcode;
				printColor(temp,token);
			}else if(temp[0]==34||isdigit(temp[0])||temp[0]==39){//���ͳ����������ͳ������ַ����������ַ�����
				SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_GREEN);
				cout<<temp;
			}else{//�ո� ������ ���ߺ�����
				SetConsoleTextAttribute(h,FOREGROUND_INTENSITY);
				cout<<temp;
			}
		}
		cout<<endl;
	}
	system("pause");
	return 0;
}