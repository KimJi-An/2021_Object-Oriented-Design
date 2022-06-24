#include "Context.h"
#include "Operation.h"

Context::Context(Operation* operation) {
	this->operation = operation;
}

void Context::setOperation(Operation* operation) {
	this->operation = operation;
}

double Context::executeOperation(double num1, double num2) {
	return operation->doOperation(num1, num2);
}
