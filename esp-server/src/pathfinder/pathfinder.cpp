#include "pathfinder.h"
#include <micropather.h>

using namespace micropather;

namespace Pathfinder {

    namespace {
        MicroPather* pather = nullptr;

        int xyToIndex(int x, int y) { return y * MAP_WIDTH + x; }
        
        void indexToXy(int index, int* x, int* y) {
            *x = index % MAP_WIDTH;
            *y = index / MAP_WIDTH;
        }

        class NavGraph : public Graph {
        public:
            NavGraph() {}
            virtual ~NavGraph() {}

            virtual float LeastCostEstimate(void* stateStart, void* stateEnd) {
                int startX, startY, endX, endY;
                indexToXy((int)(uintptr_t)stateStart, &startX, &startY);
                indexToXy((int)(uintptr_t)stateEnd, &endX, &endY);

                return (float)(abs(startX - endX) + abs(startY - endY));
            }

            virtual void AdjacentCost(void* state, MPVector<StateCost>* adjacent) {
                int x, y;
                indexToXy((int)(uintptr_t)state, &x, &y);

                const int dx[4] = { 1, -1, 0,  0 };
                const int dy[4] = { 0,  0, 1, -1 };

                for (int i = 0; i < 4; ++i) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];

                    if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT) {
                        int nIndex = xyToIndex(nx, ny);
                        if (OBSTACLES_MAP[nIndex] == 0) {
                            StateCost nodeCost = { (void*)(uintptr_t)nIndex, 1.0f };
                            adjacent->push_back(nodeCost);
                        }
                    }
                }
            }

            virtual void PrintStateInfo(void* state) {
                int x, y;
                indexToXy((int)(uintptr_t)state, &x, &y);
                Serial.printf("[%d,%d]", x, y);
            }
        };

        NavGraph* navGraph = nullptr;
    }

    void setup() {
        navGraph = new NavGraph();
        pather = new MicroPather(navGraph, 100); 
        Serial.println("Pathfinder initialized.");
    }

    void routeToDestination(uint8_t destId) {
        if (!pather) return;

        MPVector<void*> bestPath;
        float bestCost = 999999.0f;
        int bestDestX = -1;
        int bestDestY = -1;

        void* startState = (void*)(uintptr_t)xyToIndex(CURRENT_X, CURRENT_Y);
        unsigned long startTime = micros();

        for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i) {
            if (DESTINATIONS_MAP[i] == destId) {
                MPVector<void*> currentPath;
                float currentCost = 0;
                void* endState = (void*)(uintptr_t)i;

                int result = pather->Solve(startState, endState, &currentPath, &currentCost);

                if (result == MicroPather::SOLVED && currentCost < bestCost) {
                    bestCost = currentCost;
                    bestPath = currentPath;
                    indexToXy(i, &bestDestX, &bestDestY);
                }
            }
        }

        unsigned long endTime = micros();

        Serial.printf("\n--- Routing to Department ID: %d ---\n", destId);
        if (bestDestX != -1) {
            Serial.printf("Optimal parking spot found at [%d,%d]. Route calculated in %lu us.\n", bestDestX, bestDestY, (endTime - startTime));
            Serial.printf("Total steps: %.0f\n", bestCost);
            Serial.print("Route: ");
            for (unsigned i = 0; i < bestPath.size(); ++i) {
                navGraph->PrintStateInfo(bestPath[i]);
                if (i < bestPath.size() - 1) Serial.print(" -> ");
            }
            Serial.println("\n----------------------------------");
        } else {
            Serial.println("Error: No reachable parking spots found for this department!");
        }
    }
}
