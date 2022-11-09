#include "PerlinGenerator.h"

std::vector<Color32> PerlinScenarioGenerator::Generate(int sideSize, float displacement) {
    std::vector<Color32> colors;;
    for (int i = 0; i < sideSize * sideSize; i++) {
        colors.push_back(Color32::RandomColor());
    }
    return colors;
}
std::string PerlinScenarioGenerator::GetName() { return "Perlin"; }