#ifndef CLEAN2048_PRESENTER_CONTROLLER_H_
#define CLEAN2048_PRESENTER_CONTROLLER_H_

#include <memory>

#include "common/Model.h"

namespace presenter {

class Controller {
 public:
  Controller();
  ~Controller();

  void newGame();
  void swipe(common::Direction d);

 private:
  class Impl;
  std::unique_ptr<Impl> impl;
};

}  // namespace presenter

#endif
