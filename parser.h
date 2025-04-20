#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Parser {
public:
  static Expression *parse(const string &program);

private:
  static vector<string> tokenize(const string &program);
  static Expression *parseExpression(const vector<string> &tokens,
                                     size_t &index);
  static bool isNumber(const string &token);
};

#endif // EXPRESSIONS_H
