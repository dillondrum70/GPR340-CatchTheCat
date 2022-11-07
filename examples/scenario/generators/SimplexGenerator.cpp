#include "SimplexGenerator.h"
std::vector<Color32> SimplexScenarioGenerator::Generate(int sideSize) {
    //https://en.wikipedia.org/wiki/Simplex_noise
    //Coordinate Skew
        //sum = (x + y)
        //F = (sqrt(2 + 1) - 1) / 2 //dimension of 2, simplify to (sqrt(3) - 1) / 2
        //x' = x + total
        //y' = y + total
    //Simplicial Subdivision
        //End up with 3 coordinates
        //Start at (0, 0)
        //Add 1 to position with highest value, i.e. (.5, .3) -> (1, 0)
        //Add 1 to next highest position (only one left) so (.5, .3) -> (1, 1)
        //Point lies in (0,0) - (1, 0) - (1, 1) simplex
    //Gradient Selection
        //Add each vertex of simplex back to skewed base coordinate
        //then hashed into a pseudo-random gradient direction - permutation table or bit manipulation scheme, can use Noise::samples or things like random bit manipulation
    //Kernel Summation
        //total = x' + y'
        //G = (1 - (1 / sqrt(2 + 1))) / 2
        //x = x' - total * G
        //y = y' - total * G
        //Then each vertex's summed kernel is determined using the equation
        //(max(0, r^2 - d^2))^4 * (<deltaX, xeltaY> dot <gradX, gradY>)
        //r^2 is either .5 (no discontinuities) or .6 (may increase visual quality, also used in perlin)

    std::vector<Color32> colors;;
    for (int i = 0; i < sideSize * sideSize; i++) {
        colors.push_back(Color32::RandomColor());
    }
    return colors;
}
std::string SimplexScenarioGenerator::GetName() { return "Simplex"; }
