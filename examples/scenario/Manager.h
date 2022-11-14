#ifndef MANAGER_H
#define MANAGER_H

#include "ColorT.h"
#include "GameObject.h"
#include "GeneratorBase.h"
#include <SDL.h>

const Uint32 BLUE_BITMASK =		0xFF0000;	//0b111111110000000000000000
const Uint32 GREEN_BITMASK =	0x00FF00;	//0b000000001111111100000000
const Uint32 RED_BITMASK =		0x0000FF;	//0b000000000000000011111111

class Manager: public GameObject {
 private:
  float accumulatedTime = 0;
  int sideSize=512;
  SDL_Texture *texture;
  bool isSimulating = false;

  float waterLevel = 1.0f;

  std::vector<ScenarioGeneratorBase*> generators;
  int generatorId=0;

  //Hex codes of color (2 hex digits = 1 byte, 8 binary digits)
  const uint32_t DEEPEST_COLOR = 0x000000;
  const uint32_t DEEPWATER_COLOR = 0x9E0000;
  const uint32_t WATER_COLOR = 0xFF5300;
  const uint32_t BEACH_COLOR = 0xA3E1E3;
  const uint32_t GRASS_COLOR = 0x116900;
  const uint32_t MOUNTAIN_COLOR = 0x4a4a4a;
  const uint32_t SNOW_COLOR = 0xFFFFFF;

  //Heights to start colors at
  const uint32_t LOWEST_HEIGHT = 0;
  const uint32_t DEEPWATER_MAX_HEIGHT = 15;
  const uint32_t WATER_MAX_HEIGHT = 50;
  const uint32_t BEACH_MAX_HEIGHT = 100;
  const uint32_t GRASS_MAX_HEIGHT = 150;
  const uint32_t MOUNTAIN_MAX_HEIGHT = 240;
  const uint32_t MAX_HEIGHT = 255;

  void step();
 public:
  ~Manager();
  explicit Manager(Engine* engine, int size);

  void Start() override;
  void OnGui(ImGuiContext *context) override;
  void OnDraw(SDL_Renderer* renderer) override;
  void Update(float deltaTime) override;

  void Clear();
  int GetSize() const;

  void SetPixels(std::vector<Color32> &pixels);

  Uint32 LerpColor(Uint32 startColor, Uint32 endColor, Uint32 startHeight, Uint32 endHeight, Uint32 currentHeight);
};

#endif  // MANAGER_H
