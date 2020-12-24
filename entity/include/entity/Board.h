#ifndef CLEAN2048_ENTITY_BOARD_H_
#define CLEAN2048_ENTITY_BOARD_H_

#include <memory>

#include "common/Model.h"

namespace entity {

using Index = common::model::Index;
using Value = common::model::Value;
using Direction = common::model::Direction;
using Position = common::model::Position;
using Positions = common::model::Positions;
using MoveAction = common::model::MoveAction;
using NewAction = common::model::NewAction;
using ChangeAction = common::model::ChangeAction;

struct SwipeAction {
  std::vector<MoveAction> moveActions;
  std::vector<ChangeAction> changeActions;

  friend bool operator==(const SwipeAction& lhs, const SwipeAction& rhs) {
    return lhs.moveActions == rhs.moveActions &&
           lhs.changeActions == rhs.changeActions;
  }
};

class Board {
 public:
  Board();
  ~Board();

  Positions emptyPositions() const;
  NewAction addCell(Position pos, Value value);
  SwipeAction swipe(Direction direction);

 private:
  class Impl;
  std::unique_ptr<Impl> impl;
};

}  // namespace entity

#endif