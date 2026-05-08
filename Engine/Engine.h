#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "SFML/Graphics.hpp"

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace MyEngine {
class ENGINE_API Engine {
   public:
    Engine(const Engine& app) = delete;
    Engine& operator=(const Engine&) = delete;

    static Engine* Instance();

    void Run();

    // New virtual method – called on every frame after LateUpdate
    virtual void OnFrameEnd(){}

    Engine();
    virtual ~Engine() = default;

   private:
    void setupLogger();
};
}  // namespace MyEngine
#endif  // ENGINE_H