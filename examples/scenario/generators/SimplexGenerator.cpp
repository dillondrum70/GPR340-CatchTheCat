#include "SimplexGenerator.h"
#include "Vector2.h"
#include <cmath>
#include <iostream>

const double F2 = 0.5 * (std::sqrt(3.0) - 1.0); //F in 2nd dimension
const double G2 = (3.0 - std::sqrt(3.0)) / 6.0; //G in 2nd dimension   

std::vector<Color32> SimplexGenerator::Generate(int sideSize) {
    //https://en.wikipedia.org/wiki/Simplex_noise
    //Coordinate Skew
        //sum = (x + y)
        //F = (sqrt(2 + 1) - 1) / 2 //dimension of 2, simplify to (sqrt(3) - 1) / 2
        //x' = x + sum * F
        //y' = y + sum * F
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
        //x = x' - sum * G
        //y = y' - sum * G
        //Then each vertex's summed kernel is determined using the equation
        //(max(0, r^2 - d^2))^4 * (<deltaX, xeltaY> dot <gradX, gradY>)
        //r^2 is either .5 (no discontinuities) or .6 (may increase visual quality, also used in perlin)

    std::vector<Color32> colors;;
    
    for (int i = 0; i < sideSize * sideSize; i++) {
        //colors.push_back(Color32::RandomColor());
        int x = i % sideSize;
        int y = i / sideSize;

        double val = SimplexNoise(x, y);
        
        colors.push_back(Color32(val, val, val, 1));
    }
    std::cout << "Done\n";
    return colors;
}

double SimplexGenerator::SimplexNoise(float xIn, float yIn)
{
    //Skew
    int sum = (xIn + yIn) * F2;
    int dx = std::floor(xIn + sum);
    int dy = std::floor(yIn + sum);

    //Simplicial Subdivision
    //Corner 0 = (0, 0)
    double corner1X = (dx > dy) ? 1 : 0;
    double corner1Y = (dx <= dy) ? 0 : 1;
    //Corner 2 = (1, 1)

    //Gradient Subsection
    int hashGradIndex0 = (int)noiseClass.samples[dx + noiseClass.samples[dy]] % 12;
    int hashGradIndex1 = (int)noiseClass.samples[dx + corner1X + noiseClass.samples[dy + corner1Y]] % 12;
    int hashGradIndex2 = (int)noiseClass.samples[dx + 1 + noiseClass.samples[dy + 1]] % 12;

    //Kernel Summation
    sum = dx + dy;
    double x0 = xIn - (dx - (sum * G2));
    double y0 = yIn - (dy - (sum * G2));

    double x1 = x0 - corner1X + G2;
    double y1 = x0 - corner1Y + G2;

    double x2 = x0 - 1.0 + ( G2);
    double y2 = x0 - 1.0 + ( G2);
    /*double x0 = xIn - (dx - (sum * G2));
    double y0 = yIn - (dy - (sum * G2));

    double x1 = xIn - (dx + corner1X - (sum * G2));
    double y1 = yIn - (dy + corner1Y - (sum * G2));

    double x2 = xIn - (dx + 1.0 - (sum * G2));
    double y2 = yIn - (dy + 1.0 - (sum * G2));*/

   //Final calculation, 0 for each vertex of the simplex if calculated value is less than 0
    double val0 = std::max(0.0, std::pow(std::max(0., .6 - Vector2(x0, y0).sqrMagnitude()), 4) * ((dx * grad[hashGradIndex0][0]) + (dy * grad[hashGradIndex0][1])));
    double val1 = std::max(0.0, std::pow(std::max(0., .6 - Vector2(x1, y1).sqrMagnitude()), 4) * ((dx * grad[hashGradIndex1][0]) + (dy * grad[hashGradIndex1][1])));
    double val2 = std::max(0.0, std::pow(std::max(0., .6 - Vector2(x2, y2).sqrMagnitude()), 4) * ((dx * grad[hashGradIndex2][0]) + (dy * grad[hashGradIndex2][1])));

    return (val0 + val1 + val2);

    /*//Skew
    double F = (std::sqrt(3) - 1) / 2;
    int sum = xIn + yIn;
    int dx = xIn + (sum * F);
    int dy = yIn + (sum * F);

    //Subdivision
    

    //Gradient Selection
    //float gradX = n.samples[dx];
    //float gradY = n.samples[dy];

    //Kernel Summation
    double G = (1 - (1 / std::sqrt(3))) / 2;
    sum = dx + dy;
    double modX = (dx - (sum * G));
    double modY = (dy - (sum * G));

    double x0 = xIn - modX;
    double y0 = yIn - modY;

    //We know (0, 0) and (1, 1) will be there, figure out if this point is in (1, 0) or (0, 1)
    int v1x = (x0 > y0) ? 1 : 0;
    int v1y = (x0 <= y0) ? 0 : 1;

    double x1 = x0 - v1x + G;
    double y1 = y0 - v1y + G;

    double x2 = x0 - 1.0 + (2.0 * G);
    double y2 = y0 - 1.0 + (2.0 * G);

    int maskX = v1x & 255; //bitmask with 11111111
    int maskY = v1y & 255;

    int hashGradIndex0 = (int)noiseClass.samples[maskX + noiseClass.samples[maskY]] % 12;
    int hashGradIndex1 = (int)noiseClass.samples[maskX + v1x + noiseClass.samples[maskY + v1y]] % 12;
    int hashGradIndex2 = (int)noiseClass.samples[maskX + 1 + noiseClass.samples[maskY + 1]] % 12;

    double val0, val1, val2;

    double t0 = .5 - ((x0 * x0) - (y0 * y0));
    if (t0 < 0) { val0 = 0; }
    else
    {
        val0 = std::pow(t0, 4) * ((grad[hashGradIndex0][0] * x0) + (grad[hashGradIndex0][1] * y0));
    }
   
    double t1 = .5 - ((x1 * x1) - (y1 * y1));
    if (t1 < 0) { val1 = 0; }
    else
    {
        val1 = std::pow(t1, 4) * ((grad[hashGradIndex1][0] * x1) + (grad[hashGradIndex1][1] * y1));
    }

    double t2 = .5 - ((x2 * x2) - (y2 * y2));
    if (t2 < 0) { val2 = 0; }
    else
    {
        val2 = std::pow(t2, 4) * ((grad[hashGradIndex2][0] * x2) + (grad[hashGradIndex2][1] * y2));
    }

    return 20.0 * (val0 + val1 + val2);*/
}

std::string SimplexGenerator::GetName() { return "Simplex"; }
