#include "Earley.h"

int main() {
  Algo algo;
  algo.GetGrammar();
  size_t word_cnt;
  std::string word;
  std::cin >> word_cnt;
  for (size_t i = 0; i < word_cnt; ++i) {
    std::cin >> word;
    algo.GetAnswer(word);
  }
}
