#include <iostream>
#include <vector>
#include "postprocessor.h"
#include "utils.h"

namespace
{

void logReduce(const char* function_name, const Node& in, const Node& out) {
	std::cout << "reduction in " << function_name << "(): ";
	printExpression(in);
	std::cout << " -> ";
	printExpression(out);
	std::cout << std::endl;
}

bool isEqual(const Node& n1, const Node& n2) {
	if (n1.type_ != n2.type_) {
		return false;
	}

	switch (n1.type_) {
	case NodeType::Name:
		return n1.name_ == n2.name_;
	case NodeType::Number:
		return n1.number_ == n2.number_;
	case NodeType::UnaryOp:
	case NodeType::BinaryOp:
		if (n1.op_type_ != n2.op_type_) {
			return false;
		}
		if (isEqual(*n1.op1_, *n2.op1_) == false) {
			return false;
		}
		if (n1.op2_ && isEqual(*n1.op2_, *n2.op2_) == false) {
			return false;
		}
		return true;
	default:
		assert(0);
	}
}

bool match(const Node& node, const Node& pat) {
	switch (pat.type_) {
	case NodeType::NodePtrToNode: {
		*(pat.binded_node_) = &node;
		return true;
	}
	case NodeType::NodePtrToNumber:
		if (node.type_ != NodeType::Number) {
			return false;
		}
		*pat.binded_number_ = node.number_;
		return true;
	case NodeType::UnaryOp:
	case NodeType::BinaryOp:
		if (node.type_ != pat.type_) {
			return false;
		}
		if (node.op_type_ != pat.op_type_) {
			return false;
		}
		if (match(*node.op1_, *pat.op1_) == false) {
			return false;
		}
		if (pat.op2_ && match(*node.op2_, *pat.op2_) == false) {
			return false;
		}
		return true;
	default:
		return isEqual(node, pat);
	}
}

bool matchComm(const Node& node, OpType op_type, Node& pat1, Node& pat2) {
	Node pat(op_type, &pat1, &pat2);
	if (match(node, pat)) {
		return true;
	}
	pat = Node(op_type, &pat1, &pat2);
	if (match(node, pat)) {
		return true;
	}
	return false;
}

Node bindNode(const Node** in) {
	Node node (NodeType::NodePtrToNode);
	node.binded_node_ = in;
	return node;
}

Node bindNumber(int64_t* number) {
	Node node (NodeType::NodePtrToNumber);
	node.binded_number_ = number;
	return node;
}

} // namespace

std::vector<const Node*> PostProcessor::reduce(std::vector<const Node*> nodes) {
	std::vector<const Node*> result;
	for (const Node* node : nodes) {
		Node cur_node = *node;
		reduced_something_ = true;
		for (size_t step = 1; reduced_something_; ++step) {
			std::cout << "step " << step << ". going to reduce ";
			printExpression(cur_node);
			std::cout << std::endl;
			reduced_something_ = false;
			cur_node = reduceStep(cur_node);
		}
		result.push_back(new Node(cur_node));
	}

	return result;
}

Node PostProcessor::reduceStep(const Node& node) {
	Node result = node;

	// for child of the current node:
	if (result.type_ == NodeType::UnaryOp || node.type_ == NodeType::BinaryOp) {
		result.op1_ = new Node(reduceStep(*result.op1_));
		if (result.op2_ != nullptr) {
			result.op2_ = new Node(reduceStep(*result.op2_));
		}
	}

	// for the whole expression:
	result = reduceMult(result);
	result = reduceDiv(result);
	result = reduceAdd(result);
	result = reduceNeg(result);

	return result;
}

Node PostProcessor::reduceMult(const Node& node) {
	// (commutative) (X*n)*m -> X*(n*m)
	const Node* X1 = nullptr;
	int64_t n, m;
	Node x1_node = bindNode(&X1);
	Node n_node = bindNumber(&n);
	Node x1_mul_n = Node(OpType::Mult, &x1_node, &n_node);
	Node m_node = bindNumber(&m);
	if (matchComm(node, OpType::Mult, x1_mul_n, m_node)) {
		Node result = Node(OpType::Mult, X1, new Node(createNumberNode(n * m)));
		logReduce(__FUNCTION__, node, result);
		reduced_something_ = true;
		return result;
	}

	return node;
}

Node PostProcessor::reduceDiv(const Node& node) {
	// (X/n)/m -> X/(n*m)
	const Node* X = nullptr;
	int64_t n, m;
	Node x_node = bindNode(&X);
	Node n_node = bindNumber(&n);
	Node x_div_n = Node(OpType::Div, &x_node, &n_node);
	Node m_node = bindNumber(&m);
	Node div_node = Node(OpType::Div, &x_div_n, &m_node);
	if (match(node, div_node)) {
		Node result = Node(OpType::Div, X, new Node(createNumberNode(n * m)));
		logReduce(__FUNCTION__, node, result);
		reduced_something_ = true;
		return result;
	}
	return node;
}

Node PostProcessor::reduceAdd(const Node& node) {
	// X+X -> X*2
	if (node.type_ == NodeType::BinaryOp &&
	    node.op_type_ == OpType::Plus &&
		isEqual(*node.op1_, *node.op2_)) {
		Node result = Node(OpType::Mult, node.op1_, new Node(createNumberNode(2)));
		logReduce(__FUNCTION__, node, result);
		return result;
	}

	// (commutative) (X+n)+m -> X+(n+m)
	const Node* X1 = nullptr;
	int64_t n, m;
	Node x1_node = bindNode(&X1);
	Node n_node = bindNumber(&n);
	Node x1_plus_n = Node(OpType::Plus, &x1_node, &n_node);
	Node m_node = bindNumber(&m);
	if (matchComm(node, OpType::Plus, x1_plus_n, m_node)) {
		Node result = Node(OpType::Plus, X1, new Node(createNumberNode(n + m)));
		logReduce(__FUNCTION__, node, result);
		reduced_something_ = true;
		return result;
	}

	// (commutative) (X*n)+X -> X*(n+1)
	Node x1_mul_n = Node(OpType::Mult, &x1_node, &n_node);
	const Node* X2 = nullptr;
	Node x2_node = bindNode(&X2);
	if (matchComm(node, OpType::Plus, x1_mul_n, x2_node) && isEqual(*X1, *X2)) {
		Node result = Node(OpType::Mult, X1, new Node(createNumberNode(n + 1)));
		logReduce(__FUNCTION__, node, result);
		reduced_something_ = true;
		return result;
	}

	// (X*n)+(X*m) -> X*(n+m)
	Node x2_mul_m = Node(OpType::Mult, &x2_node, &m_node);
	Node summ = Node(OpType::Plus, &x1_mul_n, &x2_mul_m);
	if (match(node, summ) && isEqual(*X1, *X2)) {
		Node result = Node(OpType::Mult, X1, new Node(createNumberNode(n + m)));
		logReduce(__FUNCTION__, node, result);
		reduced_something_ = true;
		return result;
	}

	return node;
}

Node PostProcessor::reduceNeg(const Node& node) {
	// (-(-X)) -> X
	const Node* X = nullptr;
	Node x_node = bindNode(&X);
	Node neg_node = Node(OpType::Negate, &x_node);
	Node neg_neg_node = Node(OpType::Negate, &neg_node);
	if (match(node, neg_neg_node)) {
		Node result = *X;
		logReduce(__FUNCTION__, node, result);
		reduced_something_ = true;
		return result;
	}

	return node;
}
