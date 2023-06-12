#include "AstarStrategy.h"

#include "routing/astar.h"

AstarStrategy::AstarStrategy(Vector3 pos, Vector3 des,
                             const routing::IGraph* g) {
  std::vector<float> start_ = {pos[0], pos[1], pos[2]};
  std::vector<float> end_ = {des[0], des[1], des[2]};
  path = g->GetPath(start_, end_, AStar::Default());
  totalDistance = 0;
  for (int i = 0; i < path.size() - 1; i++) {
    std::vector<float> start = path.at(i);
    std::vector<float> end = path.at(i + 1);

    float xDiff = start.at(0) - end.at(0);
    float yDiff = start.at(1) - end.at(1);
    float zDiff = start.at(2) - end.at(2);
    totalDistance += sqrt((xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff));
  }
}
