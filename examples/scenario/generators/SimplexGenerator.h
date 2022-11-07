#ifndef SIMPLEX_GENERATOR_H
#define SIMPLEX_GENERATOR_H

#include "../GeneratorBase.h"

class SimplexScenarioGenerator : public ScenarioGeneratorBase {
public:
	std::vector<Color32> Generate(int sideSize) override;
	std::string GetName() override;
};

#endif
