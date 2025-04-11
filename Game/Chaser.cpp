#include "Chaser.h"

Chaser::Chaser(int x, int y, short color, double speed, const std::wstring& name, const std::vector<Coord>& patrolPath)
    : Entity(x, y, color, Direction::Up, speed, name), patrolPath(patrolPath), currentPatrolIndex(0), state(PATROLLING) {
    directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} }; // Вверх, вниз, влево, вправо
    symbol = L'&';
    eEntityType = EntityType::Chaser;
    targetPlayer = nullptr;
    previousPlayerPosition = { -1,-1 };
};

void Chaser::Update() {
    if (state == PATROLLING) {
        Patrol();
        if (CanSeePlayer()) {
            state = CHASING;
        }
    }
    else if (state == CHASING) {
        if (!CanSeePlayer()) {
            state = PATROLLING;
        }
        else {
            ChasePlayer();
        }
    }
}

void Chaser::SetTarget(Entity* player) {
    this->targetPlayer = player;
}

void Chaser::Patrol() {
    x = patrolPath[currentPatrolIndex].x;
    y = patrolPath[currentPatrolIndex].y;
    currentPatrolIndex = (currentPatrolIndex + 1) % patrolPath.size();
}

void Chaser::ChasePlayer() {
    if (!targetPlayer) return;


    Coord currentPlayerPosition = { targetPlayer->GetX(), targetPlayer->GetY() };
   

    if (currentPlayerPosition != previousPlayerPosition)
    {
        CTimerValue tp1, tp2;
        tp1 = pTimer->GetHighPrecisionTime();
        path = FindPath(x, y, targetPlayer->GetX(), targetPlayer->GetY());
        tp2 = pTimer->GetHighPrecisionTime();

        pConsole->CPrintF(L"Chaser::FindPath() completed in: %3.3f ms", (tp2 - tp1).GetMilliseconds());

        if (!path.empty())
            path.erase(path.begin());
        previousPlayerPosition = currentPlayerPosition;
    }

    if (!path.empty()) {
        x = path[0].x; 
        y = path[0].y;
        path.erase(path.begin());
    }
}

bool Chaser::CanSeePlayer() {

    if (!targetPlayer)
        targetPlayer = pWorld->FindEntity(EntityType::Player);

    if (!targetPlayer) return false;

    int playerX = targetPlayer->GetX();
    int playerY = targetPlayer->GetY();

    if (std::abs(x - playerX) <= 1000 && std::abs(y - playerY) <= 1000) {
        return true;
    }

    targetPlayer = nullptr;
    return false;
}

int Chaser::Heuristic(int startX, int startY, int goalX, int goalY) {
    return abs(startX - goalX) + abs(startY - goalY);  // Манхэттенское расстояние
}

std::vector<Coord> Chaser::FindPath(int startX, int startY, int goalX, int goalY) {
    std::priority_queue<std::pair<int, Coord>, std::vector<std::pair<int, Coord>>, std::greater<>> openList;
    std::unordered_set<Coord, CoordHash> closedList;
    std::unordered_map<Coord, Coord, CoordHash> cameFrom;
    std::unordered_map<Coord, int, CoordHash> gScore;

    Coord start = { startX, startY };
    Coord goal = { goalX, goalY };
    gScore[start] = 0;
    openList.emplace(Heuristic(startX, startY, goalX, goalY), start);

    while (!openList.empty()) {
        Coord current = openList.top().second;
        openList.pop();

        if (current == goal) {
            std::vector<Coord> path;
            while (current != start) {
                path.push_back(current);
                current = cameFrom[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedList.insert(current);

        for (const auto& dir : directions) {
            Coord neighbor = { current.x + dir.first, current.y + dir.second };

            if (neighbor.x < 0 || neighbor.x >= pWindow->GetScrWidth() || neighbor.y < 0 || neighbor.y >= pWindow->GetScrHeight()) {
                continue;  // Вне границ поля
            }

            Entity* entity = pWorld->GetEntity(neighbor.x, neighbor.y);
            if (entity != nullptr && !entity->IsMovable()) {
                continue;  // Непроходимый объект
            }

            int tentativeGScore = gScore[current] + 1;
            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                openList.emplace(tentativeGScore + Heuristic(neighbor.x, neighbor.y, goalX, goalY), neighbor);
            }
        }
    }

    return {};  // Путь не найден
}
