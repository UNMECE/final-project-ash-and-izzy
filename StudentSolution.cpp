#include "acequia_manager.h"
#include <iostream>
#include <vector>
#include <string>

// Find index of the first canal from a source region
int findCanal(const std::vector<Canal*>& canals, const std::string& sourceRegion) {
	for (int i = 0; i < canals.size(); ++i) {
		if (canals[i]->sourceRegion->name == sourceRegion) {
			return i;
		}
	}
	return -1; // Not found
}

// Open a canal from a region to release water
void release(const std::vector<Canal*>& canals, const std::string& region) {
	int index = findCanal(canals, region);
	if (index != -1) {
		canals[index]->toggleOpen(true);
		canals[index]->setFlowRate(1.0);
	}
}

// Close all canals from a region to stop water flow
void close(const std::vector<Canal*>& canals, const std::string& region) {
	int index = findCanal(canals, region);
	if (index != -1) {
		canals[index]->toggleOpen(false);
	}
}

// Main solution logic, at least the best i could come up with
void solveProblems(AcequiaManager& manager) {
	auto canals = manager.getCanals();
	auto regions = manager.getRegions();

	while (!manager.isSolved && manager.hour != manager.SimulationMax) {
		for (auto canal : canals) {
			Region* src = canal->sourceRegion;
			Region* dst = canal->destinationRegion;

			// Determine if the source has extra water
			bool sourceHasExtra = src->waterLevel > src->waterNeed;

			// Determine if destination is in drought or still below its need
			bool destNeedsMore = dst->waterLevel < dst->waterNeed;

			// Extra condition: avoid sending water if destination is already near capacity
			bool destNotNearCapacity = dst->waterLevel < (dst->waterCapacity - 10);

			if (sourceHasExtra && destNeedsMore && destNotNearCapacity) {
				canal->toggleOpen(true);

				// Dynamically adjust flow rate based on how much the destination needs
				double deficit = dst->waterNeed - dst->waterLevel;
				double flowRate = std::min(1.0, deficit / 20.0); // Cap flow rate
				canal->setFlowRate(flowRate);
			} else {
				canal->toggleOpen(false);
			}
		}

		manager.nexthour();
	}
}
