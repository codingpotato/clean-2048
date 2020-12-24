#include <doctest/doctest.h>

#include <string>

#include "BoardView.h"

TEST_CASE("Body of empty board") {
  console_ui::BoardView boardView;
  boardView.intiWithDimension(4, 4);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|       |       |       |       | 
---------------------------------
|       |       |       |       | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
}

TEST_CASE("Body of new actions") {
  console_ui::BoardView boardView;
  boardView.intiWithDimension(4, 4);
  common::model::Actions newActions{
      {},
      {
          {{1, 1}, 2},
          {{2, 2}, 4},
      },
      {},
  };
  boardView.presentActions(newActions);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|       |     2 |       |       | 
---------------------------------
|       |       |     4 |       | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
}

TEST_CASE("Bodies of move actions") {
  console_ui::BoardView boardView;
  boardView.intiWithDimension(4, 4);
  common::model::Actions newActions{
      {},
      {
          {{1, 1}, 2},
          {{2, 2}, 4},
      },
      {},
  };
  boardView.presentActions(newActions);
  REQUIRE_EQ(boardView.isInAnimation(), false);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|       |     2 |       |       | 
---------------------------------
|       |       |     4 |       | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
  common::model::Actions moveActions{
      {
          {{1, 1}, {1, 3}},
          {{2, 2}, {2, 3}},
      },
      {},
      {},
  };
  boardView.presentActions(moveActions);
  REQUIRE_EQ(boardView.isInAnimation(), true);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|       |       |     2 |       | 
---------------------------------
|       |       |       |     4 | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
  REQUIRE_EQ(boardView.isInAnimation(), true);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|       |       |       |     2 | 
---------------------------------
|       |       |       |     4 | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
}

TEST_CASE("Bodies of move, new and double actions") {
  console_ui::BoardView boardView;
  boardView.intiWithDimension(4, 4);
  common::model::Actions newActions{
      {},
      {
          {{1, 1}, 2},
          {{1, 3}, 2},
          {{2, 2}, 4},
      },
      {},
  };
  boardView.presentActions(newActions);
  REQUIRE_EQ(boardView.isInAnimation(), false);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|       |     2 |       |     2 | 
---------------------------------
|       |       |     4 |       | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
  common::model::Actions actions{
      {
          {{1, 1}, {1, 3}},
          {{2, 2}, {2, 3}},
      },
      {
          {{1, 0}, 4},
      },
      {
          {{1, 3}, 2, 4},
      },
  };
  boardView.presentActions(actions);
  REQUIRE_EQ(boardView.isInAnimation(), true);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|       |       |     2 |     2 | 
---------------------------------
|       |       |       |     4 | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
  REQUIRE_EQ(boardView.isInAnimation(), true);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|       |       |       |     2 | 
---------------------------------
|       |       |       |     4 | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
  REQUIRE_EQ(boardView.isInAnimation(), false);
  REQUIRE_EQ(boardView.body(), R"(---------------------------------
|       |       |       |       | 
---------------------------------
|     4 |       |       |     4 | 
---------------------------------
|       |       |       |     4 | 
---------------------------------
|       |       |       |       | 
---------------------------------

)");
}