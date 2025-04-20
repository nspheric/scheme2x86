#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class ASTVisitor;
class DAGVisitor;

class DAGNode {
public:
  virtual void accept(DAGVisitor &visitor) = 0;
};
// AST Nodes
class Expression {
public:
  virtual ~Expression() = default;
  virtual string toString() const = 0;
  virtual DAGNode *accept(ASTVisitor &visitor) = 0;
};

class NumberExpression : public Expression {
  int value;

public:
  NumberExpression(int value);
  ~NumberExpression() override = default;
  DAGNode *accept(ASTVisitor &visitor) override;
  string toString() const override;
  int get_value();
};

class VariableExpression : public Expression {
  string name;

public:
  VariableExpression(const string &name);
  ~VariableExpression() override = default;
  DAGNode *accept(ASTVisitor &visitor) override;
  string toString() const override;
  string get_name();
};

class AdditionExpression : public Expression {
  Expression *left;
  Expression *right;

public:
  AdditionExpression(Expression *left, Expression *right);
  ~AdditionExpression() override;
  DAGNode *accept(ASTVisitor &visitor) override;
  string toString() const override;
  Expression *get_left();
  Expression *get_right();
};

class LessExpression : public Expression {
  Expression *left;
  Expression *right;

public:
  LessExpression(Expression *left, Expression *right);
  ~LessExpression() override;
  DAGNode *accept(ASTVisitor &visitor) override;
  string toString() const override;
  Expression *get_left();
  Expression *get_right();
};

class LetExpression : public Expression {
  string variable;
  Expression *value;
  Expression *body;

public:
  LetExpression(const string &variable, Expression *value, Expression *body);
  ~LetExpression() override;
  DAGNode *accept(ASTVisitor &visitor) override;
  string toString() const override;
  string get_variable();
  Expression *get_value();
  Expression *get_body();
};

class SetExpression : public Expression {
  string variable;
  Expression *value;

public:
  SetExpression(const string &variable, Expression *value);
  ~SetExpression() override;
  DAGNode *accept(ASTVisitor &visitor) override;
  string toString() const override;
  Expression *get_value();
  string get_variable();
};

class IfExpression : public Expression {
  Expression *cnd;
  Expression *thn;
  Expression *els;

public:
  IfExpression(Expression *cnd, Expression *thn, Expression *els);
  ~IfExpression() override;
  DAGNode *accept(ASTVisitor &visitor) override;
  string toString() const override;
  Expression *get_cnd();
  Expression *get_thn();
  Expression *get_els();
};

class WhileExpression : public Expression {
  Expression *cnd;
  Expression *body;

public:
  WhileExpression(Expression *cnd, Expression *body);
  ~WhileExpression() override;
  DAGNode *accept(ASTVisitor &visitor) override;
  string toString() const override;
  Expression *get_cnd();
  Expression *get_body();
};

class BeginExpression : public Expression {
  vector<Expression *> *expressions;

public:
  BeginExpression(vector<Expression *> *expressions);
  BeginExpression();
  ~BeginExpression() override;
  DAGNode *accept(ASTVisitor &visitor) override;
  void push_exp(Expression *exp);
  string toString() const override;
  vector<Expression *> *get_expressions();
};

// DAG

class VarNode : public DAGNode {
private:
  string var;

public:
  VarNode(string var);
  ~VarNode() = default;
  void accept(DAGVisitor &visitor) override;
  string get_var() const;
};

class NumNode : public DAGNode {
private:
  int n;

public:
  NumNode(int n);
  ~NumNode();
  void accept(DAGVisitor &visitor) override;
  int get_num() const;
};

class SetNode : public DAGNode {
private:
  DAGNode *var;
  DAGNode *exp;

public:
  SetNode(DAGNode *var, DAGNode *exp);
  ~SetNode();
  void accept(DAGVisitor &visitor) override;
  DAGNode *get_var() const;
  DAGNode *get_exp() const;
};

class DAGVisitor {
public:
  virtual ~DAGVisitor() = default;
  virtual void visit(VarNode &var_exp) = 0;
  virtual void visit(NumNode &num_exp) = 0;
  virtual void visit(SetNode &set_exp) = 0;
  // virtual void visit(IfNode& if_exp) = 0;
  // virtual void visit(BeginNode& begin_exp) = 0;
};

class ASTVisitor {
public:
  virtual ~ASTVisitor() = default;
  virtual DAGNode *visit(VariableExpression &var) = 0;
  virtual DAGNode *visit(NumberExpression &num) = 0;
  virtual DAGNode *visit(SetExpression &set) = 0;
  virtual DAGNode *visit(LetExpression &let) = 0;
  virtual DAGNode *visit(LessExpression &less) = 0;
  virtual DAGNode *visit(AdditionExpression &add) = 0;
  virtual DAGNode *visit(IfExpression &if_exp) = 0;
  virtual DAGNode *visit(WhileExpression &while_exp) = 0;
  virtual DAGNode *visit(BeginExpression &begin) = 0;
};

class ToDAGVisitor : public ASTVisitor {
private:
  unordered_map<Expression *, DAGNode *> nodes;

public:
  DAGNode *visit(NumberExpression &n) override {
    if (nodes.find(&n) == nodes.end()) {
      int val = n.get_value();
      DAGNode *n_node = new NumNode(val);
      nodes[&n] = n_node;
      return n_node;
    }
    return nodes.at(&n);
  }

  DAGNode *visit(VariableExpression &var) override {
    if (nodes.find(&var) == nodes.end()) {
      string v = var.get_name();
      DAGNode *var_node = new VarNode(v);
      nodes[&var] = var_node;
      return var_node;
    }
    return nodes.at(&var);
  }

  DAGNode *visit(SetExpression &set) override {
    if (nodes.find(&set) == nodes.end()) {
      DAGNode *value_node = set.get_value()->accept(*this);
      string var = set.get_variable();
      DAGNode *var_node = new VarNode(var);
      DAGNode *set_node = new SetNode(var_node, value_node);
      nodes[&set] = set_node;
      return set_node;
    }
    return nodes.at(&set);
  }
};
