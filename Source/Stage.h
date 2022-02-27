#pragma once

#include "Material.h"
#include <vector>

namespace game_framework {
	class Stage {
	public:
		Stage();
		vector<vector<int>> getStageMaterial(int stage);
		vector<vector<int>> getStageEntity(int stage);
	protected:
		std::vector<vector<int>> map;
	};
}
