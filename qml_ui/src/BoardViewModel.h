#ifndef CLEAN_2048_QML_UI_BOARDVIEWMODEL_H
#define CLEAN_2048_QML_UI_BOARDVIEWMODEL_H

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include <QKeyEvent>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QString>
#include <unordered_map>
#include <vector>

#include "common/ModelHelper.h"
#include "presenter/BoardPresenter.h"
#include "presenter/Controller.h"

class BoardViewModel : public QObject, presenter::BoardPresenterDelegate {
  Q_OBJECT

  Q_PROPERTY(int rows MEMBER rows NOTIFY rowsChanged)
  Q_PROPERTY(int columns MEMBER columns NOTIFY columnChanged)
  Q_PROPERTY(QQuickItem* board MEMBER board)

 public:
  explicit BoardViewModel(QObject* parent = nullptr);

  void intiWithDimension(int row, int col) override {
    spdlog::info("row: {}, col: {}", row, col);
    cells = std::vector<std::vector<QQuickItem*>>(
        row, std::vector<QQuickItem*>(col, nullptr));
  }

  void present(common::Actions actions) override {
    spdlog::info("actions: {}", actions);
    for (const auto& [from, to] : actions.moveActions) {
      moveCell(from, to);
    }
    for (const auto& [pos, value] : actions.newActions) {
      createCell(pos, value);
    }
    for (const auto& [pos, from, to] : actions.changeActions) {
      changeCell(pos, to);
    }
  }

  Q_INVOKABLE void restart() { controller.newGame(); }

  Q_INVOKABLE void swipe(int key) { controller.swipe(directionMap.at(key)); }

 signals:
  void rowsChanged();
  void columnChanged();

 private:
  void createCell(common::Position pos, common::Value value) {
    QQmlEngine engine;
    QQmlComponent component(&engine, "qrc:/qml/Cell.qml");
    const auto cell = qobject_cast<QQuickItem*>(component.create());
    cell->setParentItem(board);
    cell->setSize({board->width() / 4, board->height() / 4});
    cell->setX(cell->width() * pos.col);
    cell->setY(cell->height() * pos.row);
    cell->setProperty("value", QString::number(value));
    cells[pos.row][pos.col] = cell;
  }

  void moveCell(common::Position from, common::Position to) {
    if (from == to) return;
    const auto cell = cells[from.row][from.col];
    cell->setX(cell->width() * to.col);
    cell->setY(cell->height() * to.row);
    cell->setProperty("animMoveEnable", true);
    if (cells[to.row][to.col] != nullptr) {
      delete cells[to.row][to.col];
    }
    cells[to.row][to.col] = cells[from.row][from.col];
    cells[from.row][from.col] = nullptr;
  }

  void changeCell(common::Position pos, common::Value value) {
    delete cells[pos.row][pos.col];
    createCell(pos, value);
  }

  int rows = 4;
  int columns = 4;

  QQuickItem* board;
  std::vector<std::vector<QQuickItem*>> cells;

  presenter::Controller controller;
  presenter::BoardPresenter boardPresenter;

  inline static const std::unordered_map<int, common::Direction> directionMap{
      {Qt::Key_Left, common::Direction::left},
      {Qt::Key_Right, common::Direction::right},
      {Qt::Key_Up, common::Direction::up},
      {Qt::Key_Down, common::Direction::down},
  };
};

#endif
