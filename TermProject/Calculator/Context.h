#include "Operation.h"

class Context {
private:
	Operation* operation;

public:
	Context(Operation* operation);

	void setOperation(Operation* operation);

	double executeOperation(double num1, double num2);
};
