#include "head.h"

int totalNodeNum = 0;

node* new_node() {
	node* newNode = new node;
	newNode->nodeName = totalNodeNum +65;
	cout << newNode->nodeName << endl;
	totalNodeNum++;
	return newNode;
}

elem* new_elem() {
	elem* newelem = new elem;
	newelem->edgeNum = 0;
	return newelem;
}

edge* new_edge() {
	return new edge;
}

void elem_copy(elem* dest, elem*source) {
	for (int i = 0; i < source->edgeNum; i++) {
		dest->edges[dest->edgeNum + i] = source->edges[i];
	}
	dest->edgeNum += source->edgeNum;
}

elem* act_elem(char c) {
	node*startNode = new_node();
	node*endNode = new_node();

	edge* newEdge = new_edge();
	elem* newElem= new_elem() ;
	newEdge->endNode = endNode;
	newEdge->startNode = startNode;
	newEdge->tranSymbol = c;

	newElem->edgeNum = 0;
	newElem->edges[newElem->edgeNum++] = newEdge;
	newElem->startNode = newElem->edges[0]->startNode;
	newElem->endNode = newElem->edges[0]->endNode;

	return newElem;
}

elem*act_unit(elem*first, elem*second) {
	elem* newElem = new_elem();
	newElem->edgeNum = 0;
	edge* edge1 = new_edge();
	edge* edge2 = new_edge();
	edge* edge3 = new_edge();
	edge* edge4=new_edge();
	node*startNode = new_node();
	node*endNode = new_node();

	edge1->startNode = startNode;
	edge1->endNode = first->startNode;
	edge1->tranSymbol = '#';

	edge2->startNode = first->endNode;
	edge2->endNode = endNode;
	edge2->tranSymbol = '#';

	edge3->startNode = startNode;
	edge3->endNode = second->startNode;
	edge3->tranSymbol = '#';

	edge4->startNode = second->endNode;
	edge4->endNode = endNode;
	edge4->tranSymbol = '#';

	elem_copy(newElem, first);
	elem_copy(newElem, second);

	newElem->edges[newElem->edgeNum++] = edge1;
	newElem->edges[newElem->edgeNum++] = edge2;
	newElem->edges[newElem->edgeNum++] = edge3;
	newElem->edges[newElem->edgeNum++] = edge4;

	newElem->startNode = startNode;
	newElem->endNode = endNode;

	return newElem;
}

elem*act_join(elem*first, elem*second) {
	for (int i = 0; i < second->edgeNum; i++) {
		if (second->edges[i]->startNode->nodeName.compare(second->startNode->nodeName) == 0 ) {
			second->edges[i]->startNode = first->endNode;
		}
		else if (second->edges[i]->endNode->nodeName.compare(second->startNode->nodeName) == 0) {
			second->edges[i]->endNode = first->endNode;
		}
	}
	second->startNode = first->endNode;
	elem_copy(first, second);
	first->endNode = second->endNode;

	return first;
}

//处理闭包运算
elem*act_star(elem*Elem) {
	elem*newElem = new_elem();
	newElem->edgeNum = 0;
	edge* edge1 = new_edge();
	edge* edge2 = new_edge();
	edge* edge3 = new_edge();
	edge* edge4=new_edge();

	node*startNode = new_node();
	node*endNode = new_node();

	edge1->startNode = Elem->endNode;
	edge1->endNode = Elem->startNode;
	edge1->tranSymbol = '#';

	edge2->startNode = startNode;
	edge2->endNode = Elem->startNode;
	edge2->tranSymbol = '#';

	edge3->startNode = Elem->endNode;
	edge3->endNode = endNode;
	edge3->tranSymbol = '#';

	edge4->startNode = startNode;
	edge4->endNode = endNode;
	edge4->tranSymbol = '#';

	elem_copy(newElem, Elem);

	newElem->edges[newElem->edgeNum++] = edge1;
	newElem->edges[newElem->edgeNum++] = edge2;
	newElem->edges[newElem->edgeNum++] = edge3;
	newElem->edges[newElem->edgeNum++] = edge4;

	newElem->startNode = startNode;
	newElem->endNode = endNode;

	return newElem;
}

//对连接运算省略运算符处，加上‘+’
string add_join_symbol(string ori_str) {
	int len = ori_str.size();
	int rtn_len = 0;
	char* rtn_str = new char[2 * len + 2];
	char first, second;
	for (int i = 0; i < len - 1; i++) {
		first = ori_str.at(i);
		second = ori_str.at(i + 1);
		rtn_str[rtn_len++] = first;
		if (first != '(' && first != '|' && (second >= 'a' && second <= 'z' || second >= 'A' && second <= 'Z')) {
			rtn_str[rtn_len++] = '+';
		}
		if (second == '(' && first != '(' && first != '|') {
			rtn_str[rtn_len++] = '+';
		}
	}
	rtn_str[rtn_len++] = second;
	rtn_str[rtn_len] = '\0';
	string rtn(rtn_str);
	cout << "加' + '后的表达式:" << rtn << endl;
	return rtn;
}

infixToPostfix::infixToPostfix(const string& infix_expr) {
	infix = infix_expr;
	postfix = "";
	//这优先级这里还没太搞清
	isp = { {'+',3},{'|',5},{'*',7},{'(',1},{')',8}, {'#',0} };
	icp = { {'+', 2}, {'|', 4}, {'*', 6}, {'(', 8}, {')', 1}, {'#', 0} };
}

int infixToPostfix::is_letter(char check) {
	if (check >= 'a' && check <= 'z' || check >= 'A' && check <= 'Z')
		return true;
	else
		return false;
}

int infixToPostfix::ispFunc(char c) {
	int priority = isp.count(c) ? isp[c] : -1;
	if (priority == -1) {
		cout << "invalid symbol" << endl;
		exit(1);
	}
	return priority;
}

int infixToPostfix::icpFunc(char c) {
	int priority = icp.count(c) ? icp[c] : -1;
	if (priority == -1) {
		cout << "invalid symbol" << endl;
		exit(1);
	}
	return priority;
}

void infixToPostfix::inToPost() {
	infix = infix + '#';
	stack<char> opStack;
	int loc =0 ;
	while (!opStack.empty() || loc < infix.size()) {
		if (is_letter(infix[loc])) {
			postfix += infix[loc];
			loc++;
		}
		else {
			char in = (opStack.empty()) ? '#' : opStack.top();
			char out = infix[loc];
			if (ispFunc(in) < icpFunc(out)) {
				opStack.push(out);
				loc++;
			}
			else if (ispFunc(in) > icpFunc(out)) {
				opStack.pop();
				postfix += in;
			}
			else {
				if (in == '#' && out == '#') {
					break;
				}
				opStack.pop();
				loc++;
			}
		}
	}
}

string infixToPostfix::getResult() {
	postfix = "";
	inToPost();
	cout << "后缀表达式为："<< postfix <<endl ;
	return postfix;
}

//将后缀的正则表达式转化为NFA
elem*express_to_NFA(string expr) {
	char c;
	elem* newElem=new_elem();
	elem* first=new_elem();
	elem* second=new_elem();
	stack<elem*> elStack;
	for (int i = 0; i < expr.size(); i++) {
		c = expr[i];
		switch (c) {
		case '+': 
			first = elStack.top();
			elStack.pop();
			second = elStack.top();
			elStack.pop();
			newElem = act_join(first, second);
			elStack.push(newElem);
			break;
		case '|':
			first = elStack.top();
			elStack.pop();
			second = elStack.top();
			elStack.pop();
			newElem = act_unit(first, second);
			elStack.push(newElem);
			break;
		case '*':
			first = elStack.top();
			elStack.pop();
			newElem = act_star(first);
			elStack.push(newElem);
			break;
		default:
			newElem = act_elem(c); //aaaaaa
			elStack.push(newElem);
		}
	}
	newElem = elStack.top();
	elStack.pop();

	return newElem;
}
//生成NFAdot文件
void generate_dotFile(elem* nfa) {
	std::ofstream dotFile("nfa_graph.dot");

	if (dotFile.is_open()) {
		dotFile << "digraph NFA {\n";
		dotFile << "rankdir=LR;  // 横向布局\n\n";
		dotFile << "node [shape = circle];   // 状态节点\n\n";

		dotFile << nfa->endNode->nodeName << " [shape=doublecircle];\n";
		// 添加 NFA 状态
		dotFile << "  " << nfa->startNode->nodeName << " [label=\"Start State: " << nfa->startNode->nodeName << "\"];\n";
		dotFile << "  " << nfa->endNode->nodeName << " [label=\"End State: " << nfa->endNode->nodeName << "\"];\n";

		// 添加 NFA 转移
		for (int i = 0; i < nfa->edgeNum; i++) {
			const edge* currentEdge = nfa->edges[i];
			dotFile << "  " << currentEdge->startNode->nodeName << " -> " << currentEdge->endNode->nodeName << " [label=\"" << currentEdge->tranSymbol << "\"];\n";
		}

		dotFile << "}\n";

		dotFile.close();
		std::cout << "NFA DOT file generated successfully.\n";
	}
	else {
		std::cerr << "Unable to open NFA DOT file.\n";
	}
}

int main() {
	string expr;
	elem* NFA = new_elem();
	cout << "请输入中缀正则表达式:" << endl;
	getline(cin, expr);	//输入正则表达式
	if (expr.size() > 1) expr = add_join_symbol(expr);
	//将中缀正则转化为后缀正则
	infixToPostfix solution(expr);
	string postexpr = solution.getResult();
	NFA = express_to_NFA(postexpr);
	generate_dotFile(NFA);
	return 0;
}