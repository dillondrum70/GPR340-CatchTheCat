#ifndef PERLIN_GENERATOR_H
#define PERLIN_GENERATOR_H

#include "../GeneratorBase.h"

class PerlinScenarioGenerator : public ScenarioGeneratorBase {
public:
	std::vector<Color32> Generate(int sideSize) override;
	std::string GetName() override;
};

#endif
