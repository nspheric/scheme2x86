#include "parser.h"
#include "DAG.h"

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
vector<Expression *> *BeginExpression::get_expressions() { return expressions; }

vector<string> Parser::tokenize(const string &program) {
  vector<string> tokens;
  string currentToken;
  for (char ch : program) {
    switch (ch) {
    case '(':
    case ')':
    case ' ':
      if (!currentToken.empty()) {
        tokens.push_back(currentToken);
        currentToken.clear();
      }
      if (ch != ' ')
        tokens.push_back(string(1, ch));
      break;
    default:
      currentToken += ch;
      break;
    }
  }
  if (!currentToken.empty()) {
    tokens.push_back(currentToken);
  }
  return tokens;
}

Expression *Parser::parse(const string &program) {
  vector<string> tokens = tokenize(program);
  size_t index = 0;
  return parseExpression(tokens, index);
}

Expression *Parser::parseExpression(const vector<string> &tokens,
                                    size_t &index) {
  const string &token = tokens[index];

  if (token == "(") {
    ++index;
    const string &nextToken = tokens[index];

    if (nextToken == "+") {
      ++index;
      Expression *left = parseExpression(tokens, index);
      ++index;
      Expression *right = parseExpression(tokens, index);
      ++index; // Skip closing ')'
      return new AdditionExpression(left, right);
    } else if (nextToken == "<") {
      ++index;
      Expression *left = parseExpression(tokens, index);
      ++index;
      Expression *right = parseExpression(tokens, index);
      ++index; // Skip closing ')'
      return new LessExpression(left, right);
    } else if (nextToken == "if") {
      ++index; // Skip "if"
      Expression *cnd = parseExpression(tokens, index);
      ++index;
      Expression *thn = parseExpression(tokens, index);
      ++index;
      Expression *els = parseExpression(tokens, index);
      ++index; // Skip closing ')'
      return new IfExpression(cnd, thn, els);
    } else if (nextToken == "begin") {
      ++index;
      BeginExpression *bgn = new BeginExpression();
      while (tokens[index] != ")") {
        Expression *exp = parseExpression(tokens, index);
        bgn->push_exp(exp);
        ++index;
      }

      return bgn;
    } else if (nextToken == "set") {
      ++index;
      string variable = tokens[index++];
      Expression *value = parseExpression(tokens, index);
      ++index;
      return new SetExpression(variable, value);
    } else if (nextToken == "let") {
      ++index;
      ++index;
      ++index;
      string variable = tokens[index++];
      Expression *value = parseExpression(tokens, index);
      ++index;
      ++index;
      ++index;
      Expression *body = parseExpression(tokens, index);
      ++index;
      return new LetExpression(variable, value, body);
    } else if (nextToken == "while") {
      ++index;
      Expression *cnd = parseExpression(tokens, index);
      ++index;
      Expression *body = parseExpression(tokens, index);
      ++index;
      return new WhileExpression(cnd, body);
    }
  } else if (isNumber(token)) {
    return new NumberExpression(stoi(token));
  } else {
    return new VariableExpression(token);
  }

  return nullptr;
}

bool Parser::isNumber(const string &token) {
  return !token.empty() && all_of(token.begin(), token.end(), ::isdigit);
}
