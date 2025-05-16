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

		for (int i = 0; i < regions.size(); ++i) {
			Region* region = regions[i];

			// Handle conflicting status
			if (region->isFlooded && region->isInDrought) {
				close(canals, region->name);
			}
			// If the region is flooded, release water from it
			else if (region->isFlooded) {
				release(canals, region->name);
			}
			// If the region is in drought, open canals TO it
			else if (region->isInDrought) {
				for (int j = 0; j < canals.size(); ++j) {
					if (canals[j]->destinationRegion->name == region->name) {
						canals[j]->toggleOpen(true);
						canals[j]->setFlowRate(1.0);
					}
				}
			}
		}

		manager.nexthour();
	}
}



	