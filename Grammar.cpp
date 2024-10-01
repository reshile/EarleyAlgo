#include "Grammar.h"

#include <iostream>

std::istream& operator>>(std::istream& in, Grammar& grammar) {
  size_t non_term_cnt;
  size_t char_cnt;
  size_t rules_cnt;
  std::string non_terminals;
  std::string alphabet_chars;
  std::string rule;
  std::string start_non_term;
  grammar.max_rule_size_ = 1;

  std::cin >> non_term_cnt >> char_cnt >> rules_cnt;
  std::cin >> non_terminals;
  std::cin >> alphabet_chars;

  Rule current_rule;
  grammar.rules_.resize(rules_cnt + 1);

  for (size_t i = 1; i <= rules_cnt; ++i) {
    std::getline(std::cin, rule);
    while (rule.length() == 0) {
      getline(std::cin, rule);
    }
    current_rule = grammar.ParseRule(rule, i);
    if (current_rule == WrongRuleFormat) {
      throw std::exception();
    }
    grammar.rules_[i] = current_rule;
    grammar.max_rule_size_ = std::max(grammar.max_rule_size_, current_rule.rhs.size());
  }

  std::cin >> start_non_term;
  grammar.rules_[0] = (Rule("#", start_non_term, 0)); // Add rule S' -> S
  return in;
}

void Grammar::NumerateAlphabet() {
  size_t index = 0;
  for (const auto& rule : rules_) {
    auto result = alphabet_.insert({rule.lhs[0], index});
    if (result.second) {
      chars_by_index.push_back(rule.lhs[0]);
      ++index;
    }
    for (const auto& symb : rule.rhs) {
      if (!std::isupper(symb)) {
        auto result = alphabet_.insert({symb, index});
        if (result.second) {
          chars_by_index.push_back(symb);
          ++index;
        }
      }
    }
  }
  alphabet_.insert({'\0', index});
  alphabet_size_ = alphabet_.size();
}

bool Grammar::IsRuleCorrect(const std::string& rule) {
  if ((rule.find("->") == std::string::npos) ||
      !(std::isupper(rule[0]))) {
    return false;
  }
  return true;
}

Rule Grammar::ParseRule(const std::string& rule, size_t rule_index) {
  if (!IsRuleCorrect(rule)) {
    return WrongRuleFormat;
  }
  Rule add_rule;
  size_t arrow_pos = rule.find("->");
  size_t first_non_space = rule.find_first_not_of(' ', arrow_pos + 2);
  add_rule.lhs = rule[0];
  add_rule.rule_index = rule_index;
  if (first_non_space == std::string::npos) {
    add_rule.rhs = "";
    return add_rule;
  }
  add_rule.rhs = rule.substr(first_non_space);
  return add_rule;
}

char Grammar::GetCharFromIndex(size_t index) {
  return chars_by_index[index];
}

size_t Grammar::GetIndexFromChar(char symbol) {
  auto result = alphabet_.find(symbol);
  if (result == alphabet_.end()) {
    return out_of_range;
  }
  return result->second;
}
