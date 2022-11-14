#include "Manager.h"
#include "Point2D.h"
#include "generators/RandomGenerator.h"
#include <chrono>
#include <iostream>
#include "generators/SimplexGenerator.h"
Manager::Manager(Engine* engine, int size)
    : GameObject(engine) {
    generators.push_back(new RandomScenarioGenerator());
    generators.push_back(new SimplexGenerator());
}

void Manager::SetPixels(std::vector<Color32> &input) {
  Uint32* output = nullptr;
  int pitch = 0;
  uint32_t format;

  double sqrt2 = std::sqrt(2);

  // Get the size of the texture.
  int w, h;
  SDL_QueryTexture(texture, &format, nullptr, &w, &h);

  // Now let's make our "pixels" pointer point to the texture data.
  if (SDL_LockTexture(texture, nullptr, (void**)&output, &pitch))
  {
    auto error = SDL_GetError();
    SDL_Log(error);
    return;
    // If the locking fails, you might want to handle it somehow. SDL_GetError(); or something here.
  }

  SDL_PixelFormat pixelFormat;
  pixelFormat.format = format;
  for(uint64_t line=0; line<h; line++){
    for(uint64_t column=0; column<w; column++) {
      // Now you want to format the color to a correct format that SDL can use.
      // Basically we convert our RGB color to a hex-like BGR color.
      auto color = input[line * w + column].GetPacked();
      // Before setting the color, we need to know where we have to place it.
      Uint32 pixelPosition = (line * w + column);

      Uint32 height = color % 256;
      
      double nx = (2 * ((double)column / (double)w)) - 1;
      double ny = (2 * ((double)line / (double)h)) - 1;
      //double nx = ((double)column / (double)w) * 255.0;
      //double ny = ((double)line / (double)h) * 255.0;
      double d = std::min(1.0, ((nx * nx) + (ny * ny)) / sqrt2);
      //double d = ((nx * nx) + (ny * ny)) / sqrt2;
      //color = std::clamp((int)d * 255, 0, 255);
      //color = (1 - d) * 255;
      height = std::clamp((height + ((waterLevel - d) * 255)) / 2, 0.0, 255.0);

      Uint32 b = 0;
      Uint32 g = 0;
      Uint32 r = 0;
      if (height < DEEPWATER_MAX_HEIGHT)
      {
          //color = DEEPWATER_COLOR;
          color = LerpColor(DEEPEST_COLOR, DEEPWATER_COLOR, LOWEST_HEIGHT, DEEPWATER_MAX_HEIGHT, height);
      }
      else if (height < WATER_MAX_HEIGHT)
      {
          //color = WATER_COLOR;
          color = LerpColor(DEEPWATER_COLOR, WATER_COLOR, DEEPWATER_MAX_HEIGHT, WATER_MAX_HEIGHT, height);
      }
      else if (height < BEACH_MAX_HEIGHT)
      {
          //color = BEACH_COLOR;
          color = LerpColor(WATER_COLOR, BEACH_COLOR, WATER_MAX_HEIGHT, BEACH_MAX_HEIGHT, height);
      }
      else if (height < GRASS_MAX_HEIGHT)
      {
          //color = GRASS_COLOR;
          color = LerpColor(BEACH_COLOR, GRASS_COLOR, BEACH_MAX_HEIGHT, GRASS_MAX_HEIGHT, height);
      }
      else if (height < MOUNTAIN_MAX_HEIGHT)
      {
          //color = MOUNTAIN_COLOR;
          color = LerpColor(GRASS_COLOR, MOUNTAIN_COLOR, GRASS_MAX_HEIGHT, MOUNTAIN_MAX_HEIGHT, height);
      }
      else 
      {
          //color = SNOW_COLOR;
          color = LerpColor(MOUNTAIN_COLOR, SNOW_COLOR, MOUNTAIN_MAX_HEIGHT, MAX_HEIGHT, height);
      }

      //color = (b * 255 * 255 + g * 255 + r);

      // Now we can set the pixel(s) we want.
      output[pixelPosition] = color;
    }
  }
  // Also don't forget to unlock your texture once you're done.
  SDL_UnlockTexture(texture);
}

void Manager::OnDraw(SDL_Renderer* renderer) {
  auto windowSize = engine->window->size();
  auto center = Point2D(windowSize.x/2, windowSize.y/2);
  int minDimension = std::min(windowSize.x, windowSize.y);
  const SDL_Rect r = {center.x-minDimension/2,center.y-minDimension/2, minDimension,minDimension};

  SDL_RenderCopy(renderer, texture, NULL, &r);
}
Manager::~Manager() {
  SDL_DestroyTexture(texture);
  texture=nullptr;
}
void Manager::Start() {
  texture = SDL_CreateTexture(engine->window->sdlRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 512, 512);
  std::vector<Color32> colors;
  colors.resize(sideSize*sideSize);
  for(int i=0;i<sideSize*sideSize;i++)
    colors[i]=Color::Cyan;
  SetPixels(colors);
}
void Manager::OnGui(ImGuiContext* context) {
  ImGui::SetCurrentContext(context);
  float deltaTime = ImGui::GetIO().DeltaTime;

  ImGui::SetCurrentContext(context);
  ImGui::Begin("Settings", nullptr);
  ImGui::Text("%.1fms %.0fFPS | AVG: %.2fms %.1fFPS",
              ImGui::GetIO().DeltaTime * 1000,
              1.0f / ImGui::GetIO().DeltaTime,
              1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
  static auto newSize = sideSize;

  if(ImGui::SliderInt("Side Size", &newSize, 5, 2048)) {
    //newSize = (newSize/4)*4 + 1;
    if(newSize!=sideSize) {
      sideSize = newSize;
      Clear();
    }
  }

  ImGui::Text("Generator: %s", generators[generatorId]->GetName().c_str());
  if (ImGui::BeginCombo("##combo", generators[generatorId]->GetName().c_str())) // The second parameter is the label previewed before opening the combo.
  {
    for (int n = 0; n < generators.size(); n++) {
      bool is_selected =
          (generators[generatorId]->GetName() ==
           generators[n]
               ->GetName());  // You can store your selection however you want, outside or inside your objects
      if (ImGui::Selectable(generators[n]->GetName().c_str(), is_selected)) {
        generatorId = n;
        Clear();
      }
      if (is_selected)
        ImGui::SetItemDefaultFocus();  // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
    }
    ImGui::EndCombo();
  }

  if(ImGui::Button("Generate")) {
    step();
  }

  static float newWaterLevel = waterLevel;

  if (ImGui::SliderFloat("Water Level", &newWaterLevel, .1f, 2.0f)) {
      //newSize = (newSize/4)*4 + 1;
      if (newWaterLevel != waterLevel) {
          waterLevel = newWaterLevel;
          Clear();
      }
  }

  ImGui::Text("Simulation");
  if(ImGui::Button("Step")) {
    isSimulating = false;
    step();
  }
  ImGui::SameLine();
  if(ImGui::Button("Start")) {
    isSimulating = true;
  }
  ImGui::SameLine();
  if(ImGui::Button("Pause")) {
    isSimulating = false;
  }
  ImGui::End();
}
void Manager::Update(float deltaTime) {
  if(isSimulating) {
    accumulatedTime += deltaTime;
    step();
  }
}
void Manager::Clear() {
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture(engine->window->sdlRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, sideSize, sideSize);

}
int Manager::GetSize() const {
  return sideSize;
}
void Manager::step() {
  auto start = std::chrono::high_resolution_clock::now();
  auto pixels = generators[generatorId]->Generate(sideSize, accumulatedTime);
  auto step = std::chrono::high_resolution_clock::now();
  SetPixels(pixels);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout <<  std::chrono::duration_cast<std::chrono::microseconds>(step - start).count() << " " << std::chrono::duration_cast<std::chrono::microseconds>(end - step).count() << std::endl;
}

Uint32 Manager::LerpColor(Uint32 startColor, Uint32 endColor, Uint32 startHeight, Uint32 endHeight, Uint32 currentHeight)
{
    //bit shift color value to get 8 bits each of blue, green, and red
    //Keeping these values separate prevents us from accidentally going into the bits of the other colors
    Uint32 b = (endColor & BLUE_BITMASK) >> 16;
    Uint32 g = (endColor & GREEN_BITMASK) >> 8;
    Uint32 r = endColor & RED_BITMASK;

    Uint32 lastB = (startColor & BLUE_BITMASK) >> 16;
    Uint32 lastG = (startColor & GREEN_BITMASK) >> 8;
    Uint32 lastR = startColor & RED_BITMASK;

    //Calculate what percentatage current height is between the start and end (linearly interpolate)
    double factor = ((double)currentHeight - (double)startHeight) / ((double)endHeight - (double)startHeight);

    //Multiply the next color by the factor and the last color by the inverse to get gradient value between the two
    //Bitshift blue and green back to original positions
    return ((int)((b * factor) + (lastB * (1 - factor))) << 16) + ((int)((g * factor) + (lastG * (1 - factor))) << 8) + ((r * factor) + (lastR * (1 - factor)));
}
