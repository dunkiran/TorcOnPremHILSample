

#ifndef __Messages_HPP__
#define __Messages_HPP__

#include <iostream>
#include <limits>

enum Operation {
	ADD,
	SUB,
	MUL,
	DIV
};
std::string operationNames[] = {"ADD", "SUB", "MUL", "DIV"};

#pragma pack(push, r1, 1)
struct MessageTask {

	double firstOperand{std::numeric_limits<double>::quiet_NaN()};
	double secondOperand{std::numeric_limits<double>::quiet_NaN()};
	Operation operation{ADD};

	MessageTask(double fo, double so, Operation op) : firstOperand(fo), secondOperand(so), operation(op) {}
    MessageTask(const MessageTask& m) : firstOperand(m.firstOperand), secondOperand(m.secondOperand), operation(m.operation) {}
	MessageTask(){}
};

struct MessageTaskWithResult : MessageTask{

    double result{std::numeric_limits<double>::quiet_NaN()};

    MessageTaskWithResult(double fo, double so, Operation op) : MessageTask(fo, so, op) {}
	MessageTaskWithResult(double fo, double so, Operation op, double res) : MessageTask(fo, so, op), result(res) {}
	MessageTaskWithResult(const MessageTask& m) : MessageTask(m) {}
	MessageTaskWithResult(){}
};





#pragma pack(pop, r1)

std::ostream& operator<<(std::ostream &strm, const MessageTask &m) {
	switch(m.operation) {
		case ADD:  return strm << m.firstOperand << " + " << m.secondOperand;
		case SUB:  return strm << m.firstOperand << " - " << m.secondOperand;
		case MUL:  return strm << m.firstOperand << " * " << m.secondOperand;
		case DIV:  return strm << m.firstOperand << " / " << m.secondOperand;
		default: return strm << m.firstOperand << " *unknownOp* " << m.secondOperand;
    }
};

std::ostream& operator<<(std::ostream &strm, const MessageTaskWithResult &m) {
        switch(m.operation) {
            case ADD:  return strm << m.firstOperand << " + " << m.secondOperand << " = " << m.result;
            case SUB:  return strm << m.firstOperand << " - " << m.secondOperand << " = " << m.result;
            case MUL:  return strm << m.firstOperand << " * " << m.secondOperand << " = " << m.result;
            case DIV:  return strm << m.firstOperand << " / " << m.secondOperand << " = " << m.result;
            default: return strm << m.firstOperand << " *unknownOp* " << m.secondOperand << " = " << m.result;
    }
};

#endif
