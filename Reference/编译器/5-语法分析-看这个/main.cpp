#define _CRT_SECURE_NO_WARNINGS
#include "Complier.h"

extern jmp_buf jmpbuf;
extern int JmpJud;
extern int Row;
extern pTkWord pcur;
extern TkWord word[MAX];
extern int wordlen;
//使用了C++，代码变短，另外使用了栈数据结构进行语法分析
//打开文件使用了绝对路径，这样代码移植性不好
int main() {
	static TkWord keywords[] = {
	{TK_PLUS, "+"},
	{TK_MINUS, "-"},
	{TK_MUL, "*"},
	{TK_DIVIDE, "/"},
	{TK_MOD, "%"},
	{TK_EQ, "=="},
	{TK_NEQ, "!="},
	{TK_LT, "<"},
	{TK_LEQ, "<="},
	{TK_GT, ">"},
	{TK_GEQ, ">="},
	{TK_ASSIGN, "="},
	{TK_POINTSTO, "->"},
	{TK_DOT, "."},
	{TK_AND, "&"},
	{TK_OPENPA, "("},
	{TK_CLOSEPA, ")"},
	{TK_OPENBR, "["},
	{TK_CLOSEBR, "]"},
	{TK_BEGIN, "{"},
	{TK_END, "}"},
	{TK_SEMICOLON, ";"},
	{TK_COMMA, ","},
	{TK_ELLIPSIS, "..."},
	{TK_REF, "//"},
	{TK_EOF, "End_Of_File"},

	{TK_CINT, "整型常量"},
	{TK_CCHAR, "字符常量"},
	{TK_CSTR, "字符串常量"},

	{KW_CHAR, "char"},
	{KW_SHORT, "short"},
	{KW_INT, "int"},
	{KW_VOID, "void"},
	{KW_STRUCT, "struct"},

	{KW_IF, "if"},
	{KW_ELSE, "else"},
	{KW_FOR, "for"},
	{KW_WHILE, "while"},
	{KW_CONTINUE, "continue"},
	{KW_BREAK, "break"},
	{KW_RETURN, "return"},
	{KW_SIZEOF, "sizeof"},
	{KW_ALIGN, "__align"},
	{KW_CDECL, "__cdecl"},
	{KW_STDCALL, "__stdcall"},
	{0}
	};
	vector <TkWord> hashtable[MAX];
	int keywordsLen = sizeof(keywords) / sizeof(TkWord);
	std::hash<std::string> h;
	int value = 0;

	for (int i = 0; i < keywordsLen - 1; ++i) {
		int value = h(keywords[i].s) % MAX;
		hashtable[value].push_back(keywords[i]);
	}

	int iRow = 1;
	char c[MAX] = { 0 };
	TkWord tmp;
	string s;
	FILE* fp = fopen("code.txt", "r");
	PrintBegin();
	/*------------------------------- 词法分析------------------------------------*/
	while (fgets(c, 1000, fp) != NULL) {
		printf("%-4d", iRow);
		int i = 0;
		while(i < strlen(c)){
			if (c[i] == ' ') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '\n') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '\t') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '/' && c[i + 1] == '/') {
				s += c[i++];
				s += c[i++];
				tmp.token = TK_REF;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, 29);
				while (i < strlen(c)) {
					string t;
					t += c[i];
					Print(t, 29);
					i++;
				}
				tmpClera(&tmp);
				s.clear();
			}
			else if (IS_LETTER(c[i])) {//如果是小写字母
				while (IS_LETTER_OR_NUM(c[i])) {//一直读
					s += c[i++];
				}
				tmp.s = s;
				tmp.row = iRow;
				value = h(s) % MAX;
				int j = 0;
				for(j=0;j<hashtable[value].size();j++)
					if (tmp.s == hashtable[value][j].s) {
						tmp.token = hashtable[value][j].token;
						break;
					}
				if(tmp.token==0){
					tmp.token = TK_IDENTV;
				}
				hashtable[value].push_back(tmp);
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				tmpClera(&tmp);
				s.clear();
			}
			else if (IS_NUM(c[i])) {//如果是数字
				while (IS_NUM(c[i])) {
					s += c[i++];
				}
				tmp.s = s;
				tmp.row = iRow;
				value = h(s) % MAX;
				int j = 0;
				for (j; j < hashtable[value].size(); j++)
					if (s == hashtable[value][j].s) {
						tmp.token = hashtable[value][j].token;
						break;
					}
				if (j == hashtable[value].size()) {
					tmp.token = TK_CINT;
					hashtable[value].push_back(tmp);
				}
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				tmpClera(&tmp);
				s.clear();
			}
			else if (c[i] == 39) {//这里写为单引号比较好
				for (int k = 0; k < 3; k++)
					s += c[i++];
				tmp.token = TK_CCHAR;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				s.clear();
				tmpClera(&tmp);
			}
			else if (c[i] == 34) {//这里写为双引号比较好
				s += c[i++];
				while (c[i] != 34) {
					s += c[i++];
				}
				s += c[i++];
				tmp.token = TK_CSTR;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				s.clear();
				tmpClera(&tmp);
			}
			else {
				if (c[i] == '=' && c[i+1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_EQ;
				}else if (c[i] == '!' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_NEQ;
				}else if (c[i] == '<' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_LEQ;
				}else if (c[i] == '>' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_GEQ;
				}else if (c[i] == '-' && c[i + 1] == '>') {
					s += c[i++];	s += c[i++];	tmp.token = TK_POINTSTO;
				}else { s += c[i++]; }
				tmp.row = iRow;
				tmp.s = s;
				value = h(s) % MAX;
				int j = 0;
				for (j; j < hashtable[value].size(); j++)
					if (s == hashtable[value][j].s) {
						tmp.token = hashtable[value][j].token;
						break;
					}
				Print(s, tmp.token);
				word[wordlen++] = tmp;
				s.clear();
				tmpClera(&tmp);
			}
		}
		iRow++;
		PrintWhite();
	}
	printf("\n");

	for (int i = 0; i < wordlen; i++) {
		if (word[i].token == TK_IDENTV && word[i + 1].token == TK_OPENPA)
			word[i].token = TK_IDENTF;
	}
	/*------------------------------- 语法分析------------------------------------*/
	string res;
	JmpJud = setjmp(jmpbuf);
	switch (JmpJud)
	{
	case SOMETHINGWRONG:res="编译失败，未知错误！";break;
	case LACK_SEMICOLON:res="编译失败，缺少';'！" ; break;
	case EXTER_DEC:res="编译失败，外部变量或函数错误！" ; break;
	case LACK_TYPE:res="编译失败，缺少类型符！" ; break;
	case VRA_WRONG:res="编译失败，变量错误！" ; break;
	case FUNC_WRONG:res = "编译失败，函数错误！"; break;
	case LACK_IDENT:res="编译失败，缺少标识符！" ; break;
	case LACK_OPENPA:res="编译失败，缺少'('！" ; break;
	case LACK_CLOSEPA:res="编译失败，缺少' '！" ; break;
	case LACK_OPENBR:res="编译失败，缺少'['！" ; break;
	case LACK_CLOSEBR:res="编译失败，缺少']'！" ; break;
	case LACK_BEGIN:res="编译失败，缺少'{'！" ; break;
	case LACK_END:res="编译失败，缺少'}'！" ; break;
	case NESTED_FUNC_DECLEAR:res="编译失败，函数嵌套定义！" ; break;
	case LACK_PRIMARY:res="编译失败，初等表达式错误！" ; break;
	case RETURN_ERROR:res="编译失败，return类型错误！" ; break;
	case BREAK_LACK_SEMICOLON:res="编译失败，break缺少';'！" ; break;
	case CONTINUE_LACK_SEMICOLON:res="编译失败，continue缺少';'！" ; break;
	case IF_LACK_OPENPA:res="编译失败，if中缺少'('！" ; break;
	case IF_LACK_CLOSEPA:res="编译失败，if中缺少')'!"; break;
	case FOR_LACK_OPENPA:res="编译失败，for缺少'('！"; break;
	case FOR_LACK_CLOSEPA:res="编译失败，for缺少')'！"; break;
	case FUNCVAR_WRONG:res = "编译失败，函数调用变量错误！"; break;
	case LACK_ST:res = "编译失败，缺少语句！"; break;
	case CONTINUE_ERROR:res = "编译失败，continue使用错误！"; break;
	case ELSE_ERROR:res = "编译失败，else使用错误！"; break;
	case FOR_ERROR:res = "编译失败，for使用错误！"; break;
	case WHILE_LACK_OPENPA:res = "编译失败，while缺少左圆括号！"; break;
	case WHILE_LACK_CLOSEPA:res = "编译失败，while缺少右圆括号！"; break;
	case WHILE_ERROR:res = "编译失败，while使用错误！"; break;
	case PROGRAM_BEGIN:Compile();
	default:
		res="\n==========成功 1 个，失败 0 个，跳过 0 个==========\n";
		break;
	}
	if (SUCC == JmpJud) {
		printf("============已完成项目code.c的编译工作=============");
		printf("\n%s", res.c_str());
		return 0;
	}
	else if (SOMETHINGWRONG == JmpJud) {
		printf("\n%s", res.c_str());
	}
	else {
		printf("\n==========%s==========\n", res.c_str());
		printf("\n=========== 错误行数：%d ============\n", Row);
	}
}