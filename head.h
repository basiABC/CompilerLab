#pragma once
#include <map>
#include <string>
#include <stack>
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

struct node
{
	string nodeName;
};

struct edge
{
	node *startNode;
	node *endNode;
	char tranSymbol;
};

//NFA����ɵ�Ԫ��һ��NFAҲ�ô����ݽṹ��ʾ
struct elem
{
	int edgeNum;
	edge* edges[100];
	node *startNode;
	node *endNode;
};



node* new_node();
elem*new_elem();
//no know
elem*act_elem(char);
elem*act_unit(elem, elem);
elem*act_join(elem, elem);
elem*act_star(elem);
void elem_copy(elem&, elem);
string add_join_symbol(string);
elem*express_to_NFA(string);
void generate_dotFile(elem*);

//���ڴ�����׺���ʽת��׺���ʽ����
class infixToPostfix {
public:
	infixToPostfix(const string& in_expr);

	int is_letter(char check);
	int ispFunc(char c);
	int icpFunc(char c);
	void inToPost();
	string getResult();
private:
	string infix;
	string postfix;
	map<char, int> isp;  //ջ�����ȼ�
	map<char, int> icp;  //ջ�����ȼ�
};