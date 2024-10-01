#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

enum OutOfRange {
  out_of_range = 1000000000
};

struct Rule {
  std::string lhs;
  std::string rhs;
  size_t rule_index;

  Rule() = default;

  Rule(const std::string& lhs, const std::string& rhs, size_t rule_index) :
    lhs(lhs),
    rhs(rhs),
    rule_index(rule_index) {}

  bool operator<(const Rule& other) const {
    return (lhs < other.lhs ||
           (lhs == other.lhs && rhs < other.rhs));
  }

  bool operator==(const Rule& other) const {
    return !(*this < other || other < *this);
  }

  ~Rule() {}
};

class Grammar {
 public:
  std::vector<Rule> rules_;
  // std::vector<char> alphabet_; // numerate alphabet
  //std::vector<char> non_terms_;
  std::vector<char> chars_by_index;
  std::unordered_map<char, int> alphabet_; // set biection between chars and ints
  std::string eps = "e";
  size_t alphabet_size_;
  size_t max_rule_size_;

  Grammar() = default;

  // for tests only
  char GetCharFromIndex(size_t index);
  size_t GetIndexFromChar(char symbol);

  void NumerateAlphabet();
  friend std::istream& operator>>(std::istream& in, Grammar& grammar);

  bool IsRuleCorrect(const std::string& rule);
  bool IsLeftSideCorrect(const char lhs);
  Rule ParseRule(const std::string& rule, size_t rule_index);
  ~Grammar() {}
};

const Rule WrongRuleFormat = Rule("wrong", "format", -1);

#endif
