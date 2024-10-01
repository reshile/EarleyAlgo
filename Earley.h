#ifndef EARLEY_H
#define EARLEY_H

#include "Grammar.h"

struct Situation {
  Rule rule;
  size_t dot_pos;
  size_t index;

  Situation() = default;

  Situation(const Rule& rule, size_t dot_pos, size_t index) : 
    rule(rule),
    dot_pos(dot_pos),
    index(index) {}

  bool operator<(const Situation& other);

  ~Situation() {}
};

class Algo {
 public:
  Grammar g;
  std::vector<std::vector<std::vector<Situation>>> states_;  // second vector will contain situations with specified char after dot
  std::vector<std::vector<std::vector<std::vector<bool>>>> used_situations_; // check if situation was used in Dj

  Algo() = default;

  void GetGrammar() {
    std::cin >> g;
    g.NumerateAlphabet();
  }

  void Reset() {
    states_.clear();
    used_situations_.clear();
  }

  //Algo(Grammar g) : g(g) {}

  void SetUpStates(size_t word_size);
 public:
  void GetAnswer(std::string& word);

  // TODO:
  bool WasUsedBefore(const Situation& situation, size_t set_index);
  void MarkAsUsed(const Situation& situation, size_t set_index);

  size_t GetIndexFromChar(char symbol);
  char GetCharFromIndex(size_t index);

  size_t Predict(size_t set_index);
  void Scan(size_t set_index, char symbol);
  size_t Complete(size_t set_index);

  bool WordCorrect(const std::string& word);
  ~Algo() {}
};

#endif
