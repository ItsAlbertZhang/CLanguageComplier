#include "Complier.h"

jmp_buf jmpbuf;
int JmpJud;//是否编译成功的标志
int Row;
pTkWord pcur;//指向始终要解析的词
TkWord word[MAX] = { 0 };//把代码解析为一个一个词，存到word结构体数组
int wordlen;
int pend = 0, err = 0;
string FT;
stack <string> st;		//存储{
stack <string> stelse;	//存储 if 用于配对else
stack <string> expre;		//表达式内部的括号匹配

/*---------------------------------词法分析函数----------------------------------------*/
void Print(string ch, int token) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (token >= TK_IDENTV)
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);//函数灰色
	else if (token >= KW_CHAR)
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);//关键字绿色
	else if (token >= TK_CINT)
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);//常量褐色
	else
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	if (-1 == ch[0]) {
		printf("\n	End_Of_File!");
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	cout << ch;
}

void PrintWhite() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void tmpClera(pTkWord tmp) {
	tmp->token = 0;
	tmp->row = 0;
	tmp->s.clear();
}

/*---------------------------------词法分析函数----------------------------------------*/
//编译函数
void Compile() {
	JmpJud = SUCC;
	pcur = word;
	External_dec();
}

void NextNode() {
	pcur++;
}

//外部声明
void External_dec() {   //退出外部声明时 pcur为当前声明的:或函数的} 
	if (pcur == word + wordlen) {//走到最后就结束了
		longjmp(jmpbuf, SUCC);
		return;
	}
	else if (pcur->token == TK_REF) {//是否是注释
	}
	else if (pcur->token== TK_SEMICOLON) {//是否是分号
		Row = pcur->row;
		longjmp(jmpbuf, EXTER_DEC);
	}
	else if (pcur->token == TK_END) {//是否是右大括号
		Row = pcur->row;
		longjmp(jmpbuf, LACK_BEGIN);
	}
	else if (!IS_TYPE(pcur->token)) {//是否是类型
		Row = pcur->row;
		longjmp(jmpbuf, LACK_TYPE);
	}
	else {
		type_Statement();
	}
	NextNode();
	External_dec();
}

//外部声明调用 变量/函数  变量 pcur为分号退出   函数 pcur为分号or}退出
void type_Statement() {
	if ((pcur+1)->token == TK_IDENTV) {//是否是变量名
		int i = pcur->token;
		NextNode();
		NextNode();
		Variate(i);
	}
	else if ((pcur + 1)->token == TK_IDENTF) {//是否是函数名
		Func();
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, EXTER_DEC);
	}
}

//是变量名  pcur为变量名后
void Variate(int i) {
	if (pcur->token == TK_SEMICOLON) {
		return;
	}
	else if (pcur->token == TK_ASSIGN) {
		NextNode();
		switch (i) {
		case KW_INT:
			if (pcur->token == TK_CINT) {//看赋给变量的常量类型是一致的
				NextNode();
				Variate(i);
			}
			else {
				Row = pcur->row;
				longjmp(jmpbuf, VRA_WRONG);
			}
			break;
		case KW_CHAR:
			if (pcur->token == TK_CCHAR|| pcur->token == TK_CINT) {
				NextNode();
				Variate(i);
			}
			else {
				Row = pcur->row;
				longjmp(jmpbuf, VRA_WRONG);
			}
			break;
		default:
			Row = pcur->row;
			longjmp(jmpbuf, VRA_WRONG);
			break;
		}
	}
	else if (pcur->token == TK_COMMA) {
		NextNode();
		if (pcur->token != TK_IDENTV) {
			Row = pcur->row;
			longjmp(jmpbuf, VRA_WRONG);
		}
		else {
			NextNode();
			Variate(i);
		}
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_SEMICOLON);
	}
}

//是函数名  此时pcur为int/类型
void Func() {
	FT = pcur->s;//FT是函数返回值类型
	NextNode();
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_OPENPA);
	}
	else {  //是左括号
		NextNode();
		if (pcur->token == TK_CLOSEPA) {  //是右括号
			NextNode();
		}
		else {  //不是右括号  含形参 处理  
			Func_Virate();
			NextNode();
		}
		if (pcur->token == TK_SEMICOLON) { //是分号 为函数声明 return
			return;
		}
		else if(pcur->token == TK_BEGIN){  //是{ 处理函数体 直至处理到pcur为}
			Func_Body();
			if ((pcur + 1)->token == TK_SEMICOLON) {
				NextNode();
			}
			return;
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, FUNC_WRONG);
		}
	}
}

//函数形参变量处理 pcur为第一个  处理到)返回
void Func_Virate() {
	if (!IS_TYPE(pcur->token)) {
		Row = pcur->row;
		longjmp(jmpbuf, FUNC_WRONG);
	}
	else {
		NextNode();
		if (pcur->token < TK_IDENTV) {
			Row = pcur->row;
			longjmp(jmpbuf, FUNC_WRONG);
		}
		NextNode();
		if (pcur->token == TK_CLOSEPA)//右括号就结束
			return;
		else if(pcur->token == TK_COMMA){//逗号代表还有下一个形参
			NextNode();
			Func_Virate();
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, FUNC_WRONG);
		}
	}
}

//函数体处理 pcur为{    处理到 }退出
void Func_Body() {
	st.push(pcur->s);
	NextNode();
	if (pcur->token == TK_END) {//定义为空 直接返回
		st.pop(); 
		return;
	}
	else {  
		while (!st.empty()) {
			switch (pcur->token)//pcur为函数体内(不包括第一个{ )的每行首字符
			{
			case TK_REF:
				NextNode();
				break;
			case KW_CHAR:
				NextNode();
				Func_char();
				break;
			case KW_INT:
				NextNode();
				Func_int();
				NextNode();
				break;
			case KW_IF:
				If_Process();
				break;
			case KW_ELSE:
				Else_Process();
				break;
			case KW_FOR:
				For_Process();
				break;
			case KW_WHILE:
				While_Process();
				break;
			case KW_RETURN:
				NextNode();
				Return_Process();
				break;
			case TK_BEGIN:
				st.push(pcur->s);
				NextNode();
				break;
			case TK_END:
				if (st.top()=="{") {
					st.pop();
					if (st.size() > 0)
						NextNode();
					break;
				}
				else {
					Row = pcur->row;
					longjmp(jmpbuf, LACK_BEGIN);
				}
			case TK_IDENTV:
				Primary_Assign();
				NextNode();
				break;
			case TK_IDENTF:
				Func_func();
				break;
			case KW_CONTINUE:
				NextNode();
				if (pcur->token != TK_SEMICOLON) {
					Row = pcur->row;
					longjmp(jmpbuf, CONTINUE_LACK_SEMICOLON);
				}
				NextNode();
				break;
			case KW_BREAK:
				NextNode();
				if (pcur->token != TK_SEMICOLON) {
					Row = pcur->row;
					longjmp(jmpbuf, BREAK_LACK_SEMICOLON);
				}
				NextNode();
				break;
			default:
				Row = pcur->row;
				longjmp(jmpbuf, SOMETHINGWRONG);
				break;
			}
		}
	}
	while (!st.empty())
		st.pop();
	while (!stelse.empty())
		stelse.pop();
}

void Func_char() { //处理函数体内部的char 当前pcur为char后的字符
	if (pcur->token == TK_IDENTV) {
		NextNode();
		if (pcur->token == TK_ASSIGN) {
			NextNode();
			if (pcur->token == TK_CCHAR|| pcur->token== TK_CINT) {
				NextNode();
				if (pcur->token == TK_COMMA) {
					NextNode();
					Func_char();
				}
				else if (pcur->token == TK_SEMICOLON) {
					NextNode();
					return;
				}
				else {
					Row = pcur->row;
					longjmp(jmpbuf, VRA_WRONG);
				}
			}
			else {
				Row = pcur->row;
				longjmp(jmpbuf, VRA_WRONG);
			}
		}
		else if (pcur->token == TK_COMMA) {
			NextNode();
			Func_char();
		}
		else if (pcur->token == TK_SEMICOLON) {
			NextNode();
			return;
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, VRA_WRONG);
		}
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, VRA_WRONG);
	}
}

void Func_int() { //处理函数体内部的int 当前pcur为int后的字符
	if (pcur->token == TK_IDENTV) {
		Primary_Assign();
		while (pcur->s == ",") {
			NextNode();
			Primary_Assign();
		}
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, VRA_WRONG);
	}
}

void Func_func() {  
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, FUNC_WRONG);
	}
	NextNode();
	if (pcur->token == TK_CLOSEPA) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			if (pcur->token == TK_BEGIN) {
				Row = pcur->row;
				longjmp(jmpbuf, NESTED_FUNC_DECLEAR);
			}
			Row = pcur->row;
			longjmp(jmpbuf, LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else VariInFR();
}

void VariInFR() { //    函数调用参数判断 pcur为当前第一个变量
	if (pcur->token == TK_CINT || pcur->token == TK_IDENTV|| pcur->token == TK_CSTR) {
		NextNode();
		if (pcur->token == TK_COMMA) {
			NextNode();
			VariInFR();
		}
		else if (pcur->token == TK_CLOSEPA) {
			NextNode();
			if (pcur->token != TK_SEMICOLON) {
				Row = pcur->row;
				longjmp(jmpbuf, LACK_SEMICOLON);
			}
			NextNode();
			return;
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, FUNCVAR_WRONG);
		}
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, FUNCVAR_WRONG);
	}
}

void If_Process(){
	stelse.push("if");
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, IF_LACK_OPENPA);
	}
	NextNode();
	Primary_Assign();
	if (pcur->token != TK_CLOSEPA) {
		Row = pcur->row;
		longjmp(jmpbuf, IF_LACK_CLOSEPA);
	}
	NextNode();
	if (pcur->token == KW_ELSE) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_END) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_SEMICOLON) {
		NextNode();
	}
	else if (pcur->token == TK_BEGIN) {
		st.push(pcur->s);
		NextNode();
	}
	else return;
}

void Else_Process(){
	if (stelse.empty()) {
		Row = pcur->row;
		longjmp(jmpbuf, ELSE_ERROR);
	}
	stelse.pop();
	NextNode();

	if (pcur->token == TK_END) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_SEMICOLON) {
		NextNode();
	}
	else if (pcur->token == TK_BEGIN) {
		st.push(pcur->s);
		NextNode();
	}
	else return;
}

void For_Process() {
	int count = 0;
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, FOR_LACK_OPENPA);
	}
	NextNode();	//第一个表达式
	if (pcur->token == TK_SEMICOLON) {
		count++;
		NextNode();
	}
	else {
		if (IS_TYPE(pcur->token)) {
			NextNode();
		}
		Primary_Assign();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, FOR_ERROR);
		}
		count++;
		NextNode();
	}	//第二个表达式
	if (pcur->token == TK_SEMICOLON) {
		count++;
		NextNode();
	}
	else {
		Primary_Assign();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, FOR_ERROR);
		}
		count++;
		NextNode();
	}//第三个表达式
	if (pcur->token == TK_CLOSEPA) {
		if (count != 2) {
			Row = pcur->row;
			longjmp(jmpbuf, FOR_ERROR);
		}
		NextNode();
	}
	else {
		Primary_Assign();
		if (pcur->token != TK_CLOSEPA) {
				Row = pcur->row;
				longjmp(jmpbuf, FOR_LACK_CLOSEPA);
		}
		if (count != 2) {
			Row = pcur->row;
			longjmp(jmpbuf, FOR_ERROR);
		}
		NextNode();
	}// for(;;)已处理完毕 pcur为下一个字符
	if (pcur->token == KW_CONTINUE) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, CONTINUE_LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else if (pcur->token == KW_BREAK) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, BREAK_LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else if (pcur->token == TK_END) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_SEMICOLON) {
		NextNode();
	}
	else if (pcur->token == TK_BEGIN) {
		st.push(pcur->s);
		NextNode();
	}
	else return;
}

void While_Process(){
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, WHILE_LACK_OPENPA);
	}
	NextNode();
	if (pcur->token == TK_CLOSEPA) {
		Row = pcur->row;
		longjmp(jmpbuf, WHILE_ERROR);
	}
	Primary_Assign();
	if (pcur->token != TK_CLOSEPA) {
		Row = pcur->row;
		longjmp(jmpbuf, WHILE_ERROR);
	}
	NextNode();
	if (pcur->token == KW_CONTINUE) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, CONTINUE_LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else if (pcur->token == KW_BREAK) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, BREAK_LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else if (pcur->token == TK_END) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_SEMICOLON) {
		NextNode();
	}
	else if (pcur->token == TK_BEGIN) {
		st.push(pcur->s);
		NextNode();
	}
	else return;
}

void Return_Process() {
	if (FT == "void") {
		if (pcur->token == TK_SEMICOLON) {
			NextNode();
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, RETURN_ERROR);
		}
	}
	else {
		if (pcur->token == TK_CINT || pcur->token == TK_IDENTV) {
			NextNode();
			if (pcur->token != TK_SEMICOLON) {
				Row = pcur->row;
				longjmp(jmpbuf, RETURN_ERROR);
			}
			NextNode();
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, RETURN_ERROR);
		}
	}
}

void PrintBegin() {
	printf("|——————————————————————————————|\n");
	printf("|——————————Virtual Compiler 6.0——————————|\n");
	printf("|——————————————————————————————|\n");
}

void Primary_Assign() {
	E();
	if (pend == 1 && err == 0)
		return;
	else {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_PRIMARY);
	}
}

void Next(){
	pcur++;
	if (pcur->s == ";"|| pcur->s == "," || pcur->s == ")")
		pend = 1;
}

void E(){
	C();
	while (pcur->token >= TK_EQ&& pcur->token <= TK_GEQ){
		Next();
		C();
	}
}

void C() {
	T(); 
	while (pcur->token == TK_PLUS || pcur->token == TK_MINUS || pcur->token == TK_ASSIGN){
		Next();
		T();
	}
}
 
void T(){
	F();
	while (pcur->token == TK_MUL || pcur->token == TK_DIVIDE){
		Next();
		F();
	}
}

void F(){
	if (pcur->token == TK_CINT || pcur->token== TK_IDENTV){
		Next();
	}
	else if (pcur->token == TK_OPENPA){
		Next();
		E();
		if (pcur->token == TK_CLOSEPA){
			Next();
		}
		else{	
			Row = pcur->row;
			err = -1;
		}
	}
	else{
		err = -1;
		Row = pcur->row;
		longjmp(jmpbuf, LACK_PRIMARY);
	}
}
