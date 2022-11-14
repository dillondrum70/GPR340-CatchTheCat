#ifndef MANAGER_H
#define MANAGER_H

#include "ColorT.h"
#include "GameObject.h"
#include "GeneratorBase.h"
#include <SDL.h>

class Manager: public GameObject {
 private:
  float accumulatedTime = 0;
  int sideSize=512;
  SDL_Texture *texture;
  bool isSimulating = false;

  float waterLevel = 1.0f;

  std::vector<ScenarioGeneratorBase*> generators;
  int generatorId=0;

  const uint32_t DEEPWATER_COLOR = 10354688;
  const uint32_t WATER_COLOR = 16732928;
  const uint32_t BEACH_COLOR = 10740195;
  const uint32_t GRASS_COLOR = 1140992;
  const uint32_t MOUNTAIN_COLOR = 7566195;
  const uint32_t SNOW_COLOR = 16777215;

  const uint32_t DEEPWATER_MAX_HEIGHT = 15;
  const uint32_t WATER_MAX_HEIGHT = 50;
  const uint32_t BEACH_MAX_HEIGHT = 100;
  const uint32_t GRASS_MAX_HEIGHT = 150;
  const uint32_t MOUNTAIN_MAX_HEIGHT = 225;
  //snow above 225

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
};

#endif  // MANAGER_H
