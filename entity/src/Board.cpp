#include "entity/Board.h"

#include <cassert>
#include <vector>

namespace entity {

class Board::Impl {
 public:
  Impl(common::Index rows, common::Index cols,
       const common::NewActions& newActions)
      : rows{rows}, cols{cols}, values(rows * cols, empty) {
    for (const auto& [pos, value] : newActions) {
      assert(pos.row >= 0 && pos.row < rows && pos.col >= 0 && pos.col < cols);
      values[indexOf(pos)] = value;
    }
  }

  common::Index getRows() const { return rows; }
  common::Index getCols() const { return cols; }

  common::Positions emptyPositions() const {
    common::Positions positions;
    for (auto index = 0; index < totalSizeOfMatrix(); ++index) {
      if (values[index] == empty) {
        positions.emplace_back(positionOf(index));
      }
    }
    return positions;
  }

  bool isGameOver() const {
    for (auto index = 0; index < rows * cols; ++index) {
      if (values[index] == empty || isSameAsRight(index) ||
          isSameAsBottom(index)) {
        return false;
      }
    }
    return true;
  }

  common::NewAction addCell(common::Position pos, common::Value value) {
    assert(pos.row >= 0 && pos.row < rows);
    assert(pos.col >= 0 && pos.col < cols);
    assert(values[indexOf(pos)] == empty);
    values[indexOf(pos)] = value;
    return {pos, value};
  }

  common::SwipeAction swipe(common::Direction direction) {
    common::SwipeAction action;
    const auto lastLine = (direction == common::Direction::left ||
                           direction == common::Direction::right)
                              ? rows
                              : cols;
    for (auto line = 0; line < lastLine; ++line) {
      switch (direction) {
        case common::Direction::left:
          moveLine(action, line * cols, 1, [&](const auto index) {
            return index < line * cols + cols;
          });
          break;
        case common::Direction::right:
          moveLine(action, line * cols + cols - 1, -1,
                   [&](const auto index) { return index > line * cols - 1; });
          break;
        case common::Direction::up:
          moveLine(action, line, cols,
                   [&](const auto index) { return index < rows * cols; });
          break;
        case common::Direction::down:
          moveLine(action, rows * cols - line - 1, -cols,
                   [&](const auto index) { return index >= 0; });
          break;
      }
    }
    return action;
  }

  common::NewActions restoreActions() const {
    common::NewActions newActions;
    for (auto index = 0; index < totalSizeOfMatrix(); ++index) {
      if (values[index] != empty) {
        newActions.emplace_back(positionOf(index), values[index]);
      }
    }
    return newActions;
  }

 private:
  common::Index totalSizeOfMatrix() const { return rows * cols; }
  common::Index indexOf(common::Position pos) const {
    return pos.row * cols + pos.col;
  }
  common::Position positionOf(common::Index index) const {
    return {index / cols, index % cols};
  }

  bool isSameAsRight(common::Index index) const {
    return positionOf(index).col % cols < cols - 1 &&
           values[index] == values[index + 1];
  }
  bool isSameAsBottom(common::Index index) const {
    return index < rows * cols - cols && values[index] == values[index + cols];
  }

  template <typename IsInLineFunc>
  void moveLine(common::SwipeAction& action, common::Index begin,
                common::Index offset, IsInLineFunc isInLine) {
    common::Index dest = begin;
    common::Index src = dest + offset;
    while (isInLine(dest) && isInLine(src)) {
      while (isInLine(src) && values[src] == empty) {
        src += offset;
      }
      if (isInLine(src)) {
        if (values[dest] == empty) {
          std::swap(values[dest], values[src]);
          action.moveActions.emplace_back(positionOf(src), positionOf(dest));
        } else if (values[dest] != values[src]) {
          dest += offset;
          if (dest != src) {
            std::swap(values[dest], values[src]);
            action.moveActions.emplace_back(positionOf(src), positionOf(dest));
          }
        } else {
          const auto from = values[dest];
          values[dest] = from * 2;
          values[src] = empty;
          action.moveActions.emplace_back(positionOf(src), positionOf(dest));
          action.mergeActions.emplace_back(positionOf(dest), values[dest]);
          dest += offset;
        }
        src += offset;
      }
    }
  }

  constexpr static common::Value empty = 0;

  common::Index rows;
  common::Index cols;
  common::Values values;
};

constexpr static common::Index defaultRows = 4;
constexpr static common::Index defaultCols = 4;

Board::Board() : Board{defaultRows, defaultCols, {}} {}

Board::Board(common::Index rows, common::Index cols,
             const common::NewActions& newActions)
    : impl{std::make_unique<Impl>(rows, cols, newActions)} {}

Board::~Board() = default;

Board::Board(Board&&) = default;
Board& Board::operator=(Board&&) = default;

common::Index Board::getRows() const { return impl->getRows(); }
common::Index Board::getCols() const { return impl->getCols(); }

common::Positions Board::emptyPositions() const {
  return impl->emptyPositions();
}

bool Board::isGameOver() const { return impl->isGameOver(); }

common::NewAction Board::addCell(common::Position pos, common::Value value) {
  return impl->addCell(pos, value);
}

common::SwipeAction Board::swipe(common::Direction direction) {
  return impl->swipe(direction);
}

common::NewActions Board::restoreActions() const {
  return impl->restoreActions();
}

}  // namespace entity
