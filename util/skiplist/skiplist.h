#include <memory>
#include <random>
#include <vector>

constexpr int MAX_LEVEL = 32;      // 最大层级
constexpr double P_FACTOR = 0.25;  // 概率因子

struct SkiplistNode {
  using Ptr = std::shared_ptr<SkiplistNode>;
  SkiplistNode(int _val, int _max_leve = MAX_LEVEL)
      : val(_val), forward(_max_leve, nullptr) {}

  int val;
  std::vector<SkiplistNode::Ptr> forward;
};

class SkipList {
 public:
  SkipList() : head_(new SkiplistNode(-1)), level_(0), dis(0, 1) {}
  bool Search(int target) {
    SkiplistNode::Ptr curr = this->head_;
    for (int i = level_ - 1; i >= 0; i--) {
      while (curr->forward[i] && curr->forward[i]->val < target) {
        curr = curr->forward[i];
      }
    }
    curr = curr->forward[0];
    if (curr && curr->val == target) {
      return true;
    }
    return false;
  }
  void Add(int num) {
    std::vector<SkiplistNode::Ptr> update(MAX_LEVEL, head_);
    SkiplistNode::Ptr curr = this->head_;
    for (int i = level_ - 1; i >= 0; i--) {
      while (curr->forward[i] && curr->forward[i]->val < num) {
        curr = curr->forward[i];
      }
      update[i] = curr;
    }
    int lv = RandomLevel();
    level_ = std::max(level_, lv);

    SkiplistNode::Ptr newNode = std::make_shared<SkiplistNode>(num, lv);
    for (int i = 0; i < lv; i++) {
      newNode->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = newNode;
    }
  }
  bool Erase(int num) {
    std::vector<SkiplistNode::Ptr> update(MAX_LEVEL, head_);
    SkiplistNode::Ptr curr = this->head_;
    for (int i = level_ - 1; i >= 0; i--) {
      while (curr->forward[i] && curr->forward[i]->val < num) {
        curr = curr->forward[i];
      }
      update[i] = curr;
    }
    curr = curr->forward[0];
    if (!curr || curr->val != num) {
      return false;
    }
    for (int i = 0; i < level_; i++) {
      if (update[i]->forward[i] != curr) {
        break;
      }
      update[i]->forward[i] = curr->forward[i];
    }
    curr.reset();
    while (level_ > 0 && head_->forward[level_ - 1] == nullptr) {
      level_--;
    }
    return true;
  }
  int RandomLevel() {
    int lv = 1;
    while (dis(gen) < P_FACTOR && lv < MAX_LEVEL) {
      lv++;
    }
    return lv;
  }

 private:
  SkiplistNode::Ptr head_;
  int level_;
  std::mt19937 gen{std::random_device{}()};
  std::uniform_real_distribution<double> dis;
};