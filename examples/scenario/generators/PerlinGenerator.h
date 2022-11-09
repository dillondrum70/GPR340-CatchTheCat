#ifndef PERLIN_GENERATOR_H
#define PERLIN_GENERATOR_H

#include "../GeneratorBase.h"

class PerlinScenarioGenerator : public ScenarioGeneratorBase {
public:
	std::vector<Color32> Generate(int sideSize, float displacement = 0) override;
	std::string GetName() override;
};

#endif
