#include "DAG.h"

// AST Nodes
NumberExpression::NumberExpression(int value) : value(value) {}
DAGNode *NumberExpression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
string NumberExpression::toString() const { return to_string(value); }
int NumberExpression::get_value() { return value; }

VariableExpression::VariableExpression(const string &name) : name(name) {}
DAGNode *VariableExpression::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}
string VariableExpression::toString() const { return name; }
string VariableExpression::get_name() { return name; }

AdditionExpression::AdditionExpression(Expression *left, Expression *right)
    : left(left), right(right) {}
AdditionExpression::~AdditionExpression() {
  delete left;
  delete right;
}
DAGNode *AdditionExpression::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}
string AdditionExpression::toString() const {
  return "(+ " + left->toString() + " " + right->toString() + ")";
}
Expression *AdditionExpression::get_left() { return left; }
Expression *AdditionExpression::get_right() { return right; }

LessExpression::LessExpression(Expression *left, Expression *right)
    : left(left), right(right) {}
LessExpression::~LessExpression() {
  delete left;
  delete right;
}
DAGNode *LessExpression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
string LessExpression::toString() const {
  return "(< " + left->toString() + " " + right->toString() + ")";
}
Expression *LessExpression::get_left() { return left; }
Expression *LessExpression::get_right() { return right; }

LetExpression::LetExpression(const string &variable, Expression *value,
                             Expression *body)
    : variable(variable), value(value), body(body) {}
LetExpression::~LetExpression() {
  delete value;
  delete body;
}
DAGNode *LetExpression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
string LetExpression::toString() const {
  return "(let ((" + variable + " " + value->toString() + ")) " +
         body->toString() + ")";
}
string LetExpression::get_variable() { return variable; }
Expression *LetExpression::get_value() { return value; }
Expression *LetExpression::get_body() { return body; }

SetExpression::SetExpression(const string &variable, Expression *value)
    : variable(variable), value(value) {}
SetExpression::~SetExpression() { delete value; }
DAGNode *SetExpression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
string SetExpression::toString() const {
  return "(set " + variable + " " + value->toString() + ")";
}
Expression *SetExpression::get_value() { return value; }
string SetExpression::get_variable() { return variable; }

IfExpression::IfExpression(Expression *cnd, Expression *thn, Expression *els)
    : cnd(cnd), thn(thn), els(els) {}
IfExpression::~IfExpression() {
  delete cnd;
  delete thn;
  delete els;
}
DAGNode *IfExpression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
string IfExpression::toString() const {
  return "(if " + cnd->toString() + " " + thn->toString() + " " +
         els->toString() + ")";
}
Expression *IfExpression::get_cnd() { return cnd; }
Expression *IfExpression::get_thn() { return thn; }
Expression *IfExpression::get_els() { return els; }

WhileExpression::WhileExpression(Expression *cnd, Expression *body)
    : cnd(cnd), body(body) {}
WhileExpression::~WhileExpression() {
  delete cnd;
  delete body;
}
DAGNode *WhileExpression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
string WhileExpression::toString() const {
  return "(while " + cnd->toString() + " " + body->toString() + ")";
}
Expression *WhileExpression::get_cnd() { return cnd; }
Expression *WhileExpression::get_body() { return body; }

BeginExpression::BeginExpression(vector<Expression *> *expressions)
    : expressions(expressions) {}
BeginExpression::BeginExpression() : expressions(new vector<Expression *>{}) {}
DAGNode *BeginExpression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
BeginExpression::~BeginExpression() {
  for (Expression *expr : *expressions) {
    delete expr;
  }
  delete expressions;
}
void BeginExpression::push_exp(Expression *exp) { expressions->push_back(exp); }
string BeginExpression::toString() const {
  string result = "(begin";
  for (const auto &expr : *expressions) {
    result += " " + expr->toString();
  }
  result += ")";
  return result;
}
// DAG
VarNode::VarNode(string var) : var(var) {}
void VarNode::accept(DAGVisitor &visitor) { visitor.visit(*this); }

string VarNode::get_var() const { return var; }

NumNode::NumNode(int n) : n(n) {}
NumNode::~NumNode() {}
void NumNode::accept(DAGVisitor &visitor) { visitor.visit(*this); }

int NumNode::get_num() const { return n; }
SetNode::SetNode(DAGNode *var, DAGNode *exp) : var(var), exp(exp) {}
SetNode::~SetNode() {
  delete var;
  delete exp;
}

void SetNode::accept(DAGVisitor &visitor) { visitor.visit(*this); }

DAGNode *SetNode::get_var() const { return var; }

DAGNode *SetNode::get_exp() const { return exp; };

int main() {
  int n = 3;
  NumNode n1(n);

  cout << n1.get_num() << endl;
}
