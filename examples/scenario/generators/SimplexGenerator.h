#ifndef SIMPLEX_GENERATOR_H
#define SIMPLEX_GENERATOR_H

#include "../GeneratorBase.h"
#include "../Noise.h"

class SimplexGenerator : public ScenarioGeneratorBase {
private:
	Noise noiseClass = Noise(time(nullptr));
	int grad[12][3] = { {1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0},
					{1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1},
					{0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1} };
public:
	std::vector<Color32> Generate(int sideSize) override;
	double SimplexNoise(float xIn, float yIn);
	std::string GetName() override;
};

#endif
