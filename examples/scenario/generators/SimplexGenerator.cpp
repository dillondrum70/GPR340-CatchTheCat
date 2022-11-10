#include "SimplexGenerator.h"
#include "Vector2.h"
#include <cmath>
#include <iostream>  

std::vector<Color32> SimplexGenerator::Generate(int sideSize, float displacement) {
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

        float val = SimplexNoise(x, y);
        
        colors.push_back(Color32(val, val, val, 1));
    }
    std::cout << "Done\n";
    return colors;
}

double dot(int g[], double x, double y) {
    return g[0] * x + g[1] * y;
}
double dot(int g[], double x, double y, double z) {
    return g[0] * x + g[1] * y + g[2] * z;
}
double dot(int g[], double x, double y, double z, double w) {
    return g[0] * x + g[1] * y + g[2] * z + g[3] * w;
}

int fastfloor(double x) {
    return x > 0 ? (int)x : (int)x - 1;
}

double SimplexGenerator::SimplexNoise(double xIn, double yIn)
{
   // const float F2 = 0.5 * (std::sqrt(3.0) - 1.0); //F in 2nd dimension
   // const float G2 = (3.0 - std::sqrt(3.0)) / 6.0; //G in 2nd dimension 

   // //Skew
   // int sum = (xIn + yIn) * F2;
   // int dx = (int)std::floor(xIn + sum);
   // int dy = (int)std::floor(yIn + sum);

   // sum = dx + dy;
   // float x0 = xIn - (dx - (sum * G2));
   // float y0 = yIn - (dy - (sum * G2));
   // 
   // float corner1X = 0;
   // float corner1Y = 1;
   // if (x0 > y0) { corner1X = 1; corner1Y = 0; }

   // int dx2 = dx & 255;
   // int dy2 = dy & 255;
   // int hashGradIndex0 = permutation[(int)dx2 + permutation[(int)dy2]] % 12;
   // int hashGradIndex1 = permutation[(int)dx2 + corner1X + permutation[(int)dy2 + corner1Y]] % 12;
   // int hashGradIndex2 = permutation[(int)dx2 + 1 + permutation[(int)dy2 + 1]] % 12;
   // //std::cout << hashGradIndex0 << " " << hashGradIndex1 << " " << hashGradIndex2 << std::endl;

   // float x1 = x0 - corner1X + G2;
   // float y1 = y0 - corner1Y + G2;

   // float x2 = x0 - 1.0 + (2.0 * G2);
   // float y2 = y0 - 1.0 + (2.0 * G2);
   // /*double x0 = xIn - (dx - (sum * G2));
   // double y0 = yIn - (dy - (sum * G2));

   // double x1 = xIn - (dx + corner1X - (sum * G2));
   // double y1 = yIn - (dy + corner1Y - (sum * G2));

   // double x2 = xIn - (dx + 1.0 - (sum * G2));
   // double y2 = yIn - (dy + 1.0 - (sum * G2));*/

   ////Final calculation, 0 for each vertex of the simplex if calculated value is less than 0
   // float val0 = std::max(0.0, std::pow(std::max(0., .6 - Vector2(x0, y0).sqrMagnitude()), 4) * ((x0 * (float)grad[hashGradIndex0][0]) + (y0 * (float)grad[hashGradIndex0][1])));
   // float val1 = std::max(0.0, std::pow(std::max(0., .6 - Vector2(x1, y1).sqrMagnitude()), 4) * ((x1 * (float)grad[hashGradIndex1][0]) + (y1 * (float)grad[hashGradIndex1][1])));
   // float val2 = std::max(0.0, std::pow(std::max(0., .6 - Vector2(x2, y2).sqrMagnitude()), 4) * ((x2 * (float)grad[hashGradIndex2][0]) + (y2 * (float)grad[hashGradIndex2][1])));

   // return 1000.0 * (val0 + val1 + val2);

    double n0, n1, n2; // Noise contributions from the three corners
 // Skew the input space to determine which simplex cell we're in
    double F2 = 0.5 * (std::sqrt(3.0) - 1.0);
    double s = (xIn + yIn) * F2; // Hairy factor for 2D
    int i = fastfloor(xIn + s);
    int j = fastfloor(yIn + s);
    double G2 = (3.0 - std::sqrt(3.0)) / 6.0;
    double t = (i + j) * G2;
    double X0 = i - t; // Unskew the cell origin back to (x,y) space
    double Y0 = j - t;
    double x0 = xIn - X0; // The x,y distances from the cell origin
    double y0 = yIn - Y0;
    //std::cout << "(" << xIn << ", " << yIn << ")" << std::endl;
    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if (x0 > y0) { i1 = 1; j1 = 0; } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    else { i1 = 0; j1 = 1; } // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 - 1.0 + 2.0 * G2;
    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = permutation[ii + permutation[jj]] % 12;
    int gi1 = permutation[ii + i1 + permutation[jj + j1]] % 12;
    int gi2 = permutation[ii + 1 + permutation[jj + 1]] % 12;
    // Calculate the contribution from the three corners
    double t0 = 0.5 - x0 * x0 - y0 * y0;
    if (t0 < 0) n0 = 0.0;
    else {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
    }
    double t1 = 0.5 - x1 * x1 - y1 * y1;
    if (t1 < 0) n1 = 0.0;
    else {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad[gi1], x1, y1);
    }
    double t2 = 0.5 - x2 * x2 - y2 * y2;
    if (t2 < 0) n2 = 0.0;
    else {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad[gi2], x2, y2);
    }
    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 700.0 * (n0 + n1 + n2);

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
