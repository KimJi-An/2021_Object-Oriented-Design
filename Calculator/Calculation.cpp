#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include "Operation.h"
#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Division.h"
#include "Context.h"
#define ERROR "Invalid Input"

using namespace std;

bool isOperator(char c) {
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')')
		return true;
	return false;
}

bool isOperand(char c) {
	if (c >= '0' && c <= '9' || c == '.')
		return true;
	else if ((c >= 'A' && c <= 'F') || c == 'X')
		return true;
	else if ((c >= 'a' && c <= 'f') || c == 'x')
		return true;
	else
		return false;
}

int priority(char c) {
	if (c == '*' || c == '/')
		return 3;
	else if (c == '+' || c == '-')
		return 2;
	else
		return 1;
}

bool parenthesis(string input) {
	int leftCount = 0, rightCount = 0;

	for (int i = 0; i < input.length(); i++) {
		char c = input.at(i);

		if (c == '(')
			leftCount++;
		else if (c == ')')
			rightCount++;
	}

	if (leftCount == rightCount)
		return true;
	else
		return false;
}

bool input_exception(string input) {
	if (!parenthesis(input))
		return true;

	else if (isOperator(input.at(0)) && input.at(0) != '(')
		return true;

	else {
		for (int i = 1; i < input.length(); i++) {
			char c = input.at(i);

			if (isOperand(c))
				continue;

			else {
				if ((isOperator(input.at(i - 1)) && input.at(i - 1) != ')')
					&& c == '-')
					return true;

				else if (input.at(i - 1) == '(' && c == ')')
					return true;

				else if (input.at(i - 1) == ')' && c == '(')
					return true;
			}
		}
	}

	return false;
}

string infixToPostfix(string input) {
	stack<char> st;

	string postfix = "";

	if (input_exception(input))
		postfix = ERROR;

	else {
		for (int i = 0; i < input.length(); i++) {
			char c = input.at(i);

			if (isOperand(c))
				postfix.push_back(c);

			else {
				if (c == '(')
					st.push(c);

				else if (c == ')') {
					while (st.top() != '(') {
						postfix.push_back(' ');
						postfix.push_back(st.top());
						st.pop();
					}
					st.pop();
				}
				else if (st.empty()) {
					postfix.push_back(' ');
					st.push(c);
				}
				else {
					postfix.push_back(' ');

					if (priority(c) > priority(st.top()))
						st.push(c);

					else {
						postfix.push_back(st.top());
						st.pop();
						st.push(c);
						postfix.push_back(' ');
					}
				}
			}
		}
	}
	if (postfix != ERROR) {
		while (!st.empty()) {
			postfix.push_back(' ');
			postfix.push_back(st.top());
			st.pop();
		}
	}

	return postfix;
}

double convert_value(string num) {
	double result = -1;

	if (num.length() > 2) {
		string st = num.substr(0, 2);

		if (st == "0X" || st == "0x")
			result = stoi(num, nullptr, 16);

		else if (st == "0B" || st == "0b") {
			string str = num.substr(2, num.length() - 2);
			result = stoi(str, nullptr, 2);
		}

		else {
			string type = "";
			for (int i = 0; i < num.length(); i++) {
				if (num.at(i) == '.') {
					type = "double";
					break;
				}
			}

			if (type == "double")
				result = stod(num);

			else
				result = stoi(num, nullptr, 10);
		}
	}
	else
		result = stoi(num, nullptr, 10);

	return result;
}

void calculation(string postfix) {
	string str;
	stringstream stream(postfix);

	stack<double> operand;

	double result;

	if (postfix == ERROR) {
		cout << " Invalid input. Try again." << endl;
		cout << endl;
	}
	else {
		while (stream >> str) {
			if (!isOperator(str.at(0))) {
				double num = convert_value(str);
				operand.push(num);
			}
			else {
				char oper = str.at(0);
				double calc;

				Context* context;

				double num1 = operand.top();
				operand.pop();

				double num2 = operand.top();
				operand.pop();

				switch (oper) {
				case '+':
					context = new Context(new Addition());
					calc = context->executeOperation(num1, num2);
					operand.push(calc);
					break;
				case '-':
					context = new Context(new Subtraction());
					calc = context->executeOperation(num1, num2);
					operand.push(calc);
					break;
				case '*':
					context = new Context(new Multiplication());
					calc = context->executeOperation(num1, num2);
					operand.push(calc);
					break;
				default:
					context = new Context(new Division());
					calc = context->executeOperation(num1, num2);
					operand.push(calc);
				}
			}
		}

		result = operand.top();
		operand.pop();

		cout << "  = " << result << endl;
		cout << endl;
	}
}

int main() {
	string input;
	string result;

	cout << " ---------- Calculator ----------" << endl;
	cout << endl;
	cout << " Enter \"q\" to quit." << endl;
	cout << endl;

	while (true) {
		cout << " >> ";
		getline(cin, input, '\n');

		if (input == "q") break;

		string postfix = infixToPostfix(input);

		calculation(postfix);
	}
}