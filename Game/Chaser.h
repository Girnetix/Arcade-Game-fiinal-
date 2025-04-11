#pragma once
#include <Engine/Entity.h>
#include <Engine/World.h>
#include "EntitiesTypes.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

struct Coord {
    int x, y;

    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Coord& other) const {
        return !(*this == other);
    }

    // Перегрузка оператора <
    bool operator<(const Coord& other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
};


// Хеш-функция для Coord, чтобы использовать его в unordered_map
struct CoordHash {
    std::size_t operator()(const Coord& coord) const {
        return std::hash<int>()(coord.x) ^ std::hash<int>()(coord.y);
    }
};

class Chaser : public Entity {
public:
    enum State { PATROLLING, CHASING };

    Chaser(int x, int y, short color, double speed, const std::wstring& name, const std::vector<Coord>& patrolPath);
    void Update() override;
    void SetTarget(Entity* player);

private:
    State state;
    Entity* targetPlayer;
    std::vector<Coord> patrolPath;
    int currentPatrolIndex;
    std::vector<std::pair<int, int>> directions;
    Coord previousPlayerPosition;
    std::vector<Coord> path;

    std::vector<Coord> FindPath(int startX, int startY, int goalX, int goalY);
    void Patrol();
    void ChasePlayer();
    bool CanSeePlayer();
    int Heuristic(int startX, int startY, int goalX, int goalY);
};
