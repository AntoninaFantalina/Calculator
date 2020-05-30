#include <iostream>
#include <vector>
#include "postprocessor.h"
#include "utils.h"

namespace
{

Node* dbgPrintReducedExpr(const char* function_name, Node* in, Node* out) {
	std::cout << "reduction in " << function_name << "(): ";
	printExpression(in);
	std::cout << " -> ";
	printExpression(out);
	std::cout << std::endl;
	// bool reduced_something=true;
	return out;
}

bool isEqual(Node* n1, Node* n2) {
	if (n1->type != n2->type) {
		return false;
	}

	switch (n1->type) {
	case NodeType::Name:
		return n1->name == n2->name;
	case NodeType::Number:
		return n1->number == n2->number;
	case NodeType::UnaryOp:
	case NodeType::BinaryOp:
		if (n1->op_type != n2->op_type) {
			return false;
		}
		if (isEqual(n1->op1, n2->op1) == false) {
			return false;
		}
		if (n1->op2 && isEqual(n1->op2, n2->op2) == false) {
			return false;
		}
		return true;
	default:
		assert(0);
	}
}

bool match(Node* node, Node* pat)
{
	switch (pat->type) {
	case NodeType::Name:
		return isEqual(node, pat);

	case NodeType::Number:
		return isEqual(node, pat);

	case NodeType::UnaryOp:
	case NodeType::BinaryOp:
		if (node->type != pat->type) {
			return false;
		}
		if (node->op_type != pat->op_type) {
			return false;
		}
		if (match(node->op1, pat->op1) == false) {
			return false;
		}
		if (pat->op2 && match(node->op2, pat->op2) == false) {
			return false;
		}
		return true;
	case NodeType::NodePtrToNode:
		*pat->binded_node = node;
		return true;
	case NodeType::NodePtrToNumber:
		if (node->type != NodeType::Number) {
			return false;
		}
		*pat->binded_number = node->number;
		return true;
	default:
		assert(0);
	}
}

bool matchComm(Node* node, OpType op_type, Node* pat1, Node* pat2) {
	if (match(node, createBinaryOpNode(op_type, pat1, pat2))) {
		return true;
	}
	if (match(node, createBinaryOpNode(op_type, pat2, pat1))) {
		return true;
	}
	return false;
}

Node* bindNode(Node** in) {
	Node* node = new Node();
	node->type = NodeType::NodePtrToNode;
	node->binded_node = in;
	return node;
}

Node* bindNumber(int64_t* number) {
	Node* node = new Node();
	node->type = NodeType::NodePtrToNumber;
	node->binded_number = number;
	return node;
}

Node* reduceMult(Node* node) {
	Node* X1;
	int64_t n, m;
	// (commutative) (X*n)*m -> X*(n*m)
	if (matchComm(node,
	              OpType::Mult,
				  createBinaryOpNode(OpType::Mult, bindNode(&X1), bindNumber(&n)),
				  bindNumber(&m))) {
		node = dbgPrintReducedExpr(__FUNCTION__, node, createBinaryOpNode(OpType::Mult, X1, createNumberNode(n * m)));
	}

	Node* X2;
	// (X*n)*(X*m) -> X*(n*m)
	if (match(node, createBinaryOpNode(OpType::Mult, 
					                   createBinaryOpNode(OpType::Mult, bindNode(&X1), bindNumber(&m)),
									   createBinaryOpNode(OpType::Mult, bindNode(&X2), bindNumber(&n))))
		&& isEqual(X1, X2)) {
		node = dbgPrintReducedExpr(__FUNCTION__, node, createBinaryOpNode(OpType::Mult, X1, createNumberNode(m * n)));
	}

	return node;
}

Node* reduceDiv(Node* node) {
	// (X/n)/m -> X/(n*m)
	Node* X;
	int64_t n, m;
	if (match(node, 
	            createBinaryOpNode(
					OpType::Div,
					createBinaryOpNode(OpType::Div, bindNode(&X), bindNumber(&n)),
					bindNumber(&m)))) {
		node = dbgPrintReducedExpr(__FUNCTION__, node, createBinaryOpNode(OpType::Div, X, createNumberNode(n * m)));
	}
	return node;
}

Node* reduceAdd(Node* node) {
	// X+X -> X*2
	if (node->type == NodeType::BinaryOp &&
	    node->op_type == OpType::Plus &&
		isEqual(node->op1, node->op2)) {
		node = dbgPrintReducedExpr(__FUNCTION__, node, createBinaryOpNode(OpType::Mult, node->op1, createNumberNode(2)));
	}

	// (commutative) (X+n)+m -> X+(n+m)
	Node* X1;
	int64_t n, m;
	if (matchComm(node,
	              OpType::Plus,
				  createBinaryOpNode(OpType::Plus, bindNode(&X1), bindNumber(&n)),
				  bindNumber(&m))) {
		node = dbgPrintReducedExpr(__FUNCTION__, node, createBinaryOpNode(OpType::Plus, X1, createNumberNode(n + m)));
	}

	Node* X2;
	// (commutative) (X*n)+X -> X*(n+1)
	if (matchComm(node, 
	              OpType::Plus,
				  createBinaryOpNode(OpType::Mult, bindNode(&X1), bindNumber(&n)),
				  bindNode(&X2)) &&
			isEqual(X1, X2)) {
		node = dbgPrintReducedExpr(__FUNCTION__, node, createBinaryOpNode(OpType::Mult, X1, createNumberNode(n + 1)));
	}

	// (X*n)+(X*m) -> X*(n+m)
	if (match(node, createBinaryOpNode(OpType::Plus, 
					                   createBinaryOpNode(OpType::Mult, bindNode(&X1), bindNumber(&n)),
									   createBinaryOpNode(OpType::Mult, bindNode(&X2), bindNumber(&m)))) &&
		isEqual(X1, X2)) {
		node = dbgPrintReducedExpr(__FUNCTION__, node, createBinaryOpNode(OpType::Mult, X1, createNumberNode(n + m)));
	}

	return node;
}

Node* reduceNeg(Node* node) {
	// (-(-X)) -> X
	Node* X;
	if (match(node, createUnaryOpNode(OpType::Negate, createUnaryOpNode(OpType::Negate, bindNode(&X))))) {
		node = dbgPrintReducedExpr(__FUNCTION__, node, X);
	}

	return node;
}

Node* reduceStep(Node* node) {
	// for child(-ren) of the current node:
	if (node->type == NodeType::UnaryOp || node->type == NodeType::BinaryOp) {
		node->op1 = reduceStep(node->op1);
		if (node->op2 != nullptr) {
			node->op2 = reduceStep(node->op2);
		}
	}

	// for the whole expression:
	node = reduceMult(node);
	node = reduceDiv(node);
	node = reduceAdd(node);
	node = reduceNeg(node);

	return node;
}

} // namespace

std::vector<Node*> reduce(std::vector<Node*> nodes) {
	std::vector<Node*> result;
	for (Node* node : nodes) {
		bool reduced_something = true;
		for (size_t step = 1; reduced_something; ++step) {
			reduced_something = false;
			std::cout << "step " << step << ". going to reduce ";
			printExpression(node);
			std::cout << std::endl;
			node = reduceStep(node);
		}
		result.push_back(node);
	}
	
	return result;
}
