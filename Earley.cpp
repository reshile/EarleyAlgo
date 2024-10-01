#include "Earley.h"

void Algo::SetUpStates(size_t word_size) {
  states_.resize(word_size + 1);
  for (auto& vec : states_) {
    vec.resize(g.alphabet_size_);
  }

  // TODO:
  /*
    vec<vec<vec<vec>>>> used_situations;

    first vector <-> states (0, 1, ... , word_size + 1)
    second vector <-> rules
    third vector  <-> index (0, 1, ..., word_size + 1)
    fourth vector <-> dot position

  */
  used_situations_.resize(word_size + 1);
  for (auto& rule_vec : used_situations_) {
    rule_vec.resize(g.rules_.size());
    for (auto& index_vec : rule_vec) {
      index_vec.resize(word_size + 1);
      for (auto& dot_vec : index_vec) {
        dot_vec.resize(g.max_rule_size_, false);
      }
    }
  }
}

void Algo::GetAnswer(std::string& word) {
  if (!WordCorrect(word)) {
    std::cout << "No" << '\n';
    return;
  };
  SetUpStates(word.size());
  states_[0][GetIndexFromChar('S')].push_back(Situation(g.rules_[0], 0, 0));
  MarkAsUsed(Situation(g.rules_[0], 0, 0), 0);

  size_t predict_flag;
  size_t complete_flag;
  do {
    predict_flag = Predict(0);
    complete_flag = Complete(0);
  } while (predict_flag || complete_flag);

  for (size_t i = 1; i <= word.size(); ++i) {
    Scan(i - 1, word[i - 1]);
    do {
      predict_flag = Predict(i);
      complete_flag = Complete(i);
    } while (predict_flag || complete_flag);
  }


  // Get Answer Here --> ...
  if (WasUsedBefore(Situation(g.rules_[0], 1, 0), word.size())) {
    std::cout << "Yes" << '\n';
  } else {
    std::cout << "No" << '\n';
  }
  Reset();
}

size_t Algo::Predict(size_t set_index) {
  size_t cnt_of_changes = 0;
  for (const auto& rule : g.rules_) {
    if (states_[set_index][GetIndexFromChar(rule.lhs[0])].empty()) { continue; }
    Situation new_sit(rule, 0, set_index);
    if (!WasUsedBefore(new_sit, set_index)) {
      size_t symbol_index = GetIndexFromChar(rule.rhs[0]);
      states_[set_index][symbol_index].push_back(new_sit);
      ++cnt_of_changes;
    }
  }
  return cnt_of_changes;
}

void Algo::Scan(size_t set_index, char symbol) {
  size_t symbol_index = GetIndexFromChar(symbol);
  for (auto& situation : states_[set_index][symbol_index]) {
    Situation new_sit(situation.rule, situation.dot_pos + 1, situation.index);
    MarkAsUsed(new_sit, set_index + 1);
    size_t next_symb_index = GetIndexFromChar(situation.rule.rhs[situation.dot_pos + 1]);
    states_[set_index + 1][next_symb_index].push_back(new_sit);
  }
}

size_t Algo::Complete(size_t set_index) {
  size_t cnt_of_changes = 0;
  for (auto& vec : states_[set_index]) {
    for (auto& situation : vec) {
      if (situation.dot_pos != situation.rule.rhs.size()) { continue; }
      size_t sit_set_index = situation.index;
      size_t symbol_index = GetIndexFromChar(situation.rule.lhs[0]);
      for (auto& main_situation : states_[sit_set_index][symbol_index]) {
        Situation new_situation(main_situation.rule, main_situation.dot_pos + 1,
                                main_situation.index);
        size_t next_symbol_index =
                GetIndexFromChar(main_situation.rule.rhs[main_situation.dot_pos + 1]);
        if (!WasUsedBefore(new_situation, set_index)) {
          states_[set_index][next_symbol_index].push_back(new_situation);
          ++cnt_of_changes;
        }
      }
    }
  }
  return cnt_of_changes;
}

// Possibly don't need
bool Situation::operator<(const Situation& other) {
  if (rule == other.rule) {
    if (dot_pos == other.dot_pos) {
      return index < other.index;
    }
    return dot_pos < other.dot_pos;
  }
  return rule < other.rule;
}

char Algo::GetCharFromIndex(size_t index) {
  return g.chars_by_index[index];
}

size_t Algo::GetIndexFromChar(char symbol) {
  auto result = g.alphabet_.find(symbol);
  if (result == g.alphabet_.end()) {
    return out_of_range;
  }
  return result->second;
}

bool Algo::WasUsedBefore(const Situation& situation, size_t set_index) {
  bool was_used = used_situations_[set_index][situation.rule.rule_index]
                         [situation.index][situation.dot_pos];
  if (was_used) {
    return true;
  }
  MarkAsUsed(situation, set_index);
  return false;
}

void Algo::MarkAsUsed(const Situation& situation, size_t set_index) {
  used_situations_[set_index][situation.rule.rule_index]
                  [situation.index][situation.dot_pos] = true;
}

bool Algo::WordCorrect(const std::string& word) {
  for (const auto& symb : word) {
    if (GetIndexFromChar(symb) == out_of_range) {
      return false;
    }
  }
  return true;
}
