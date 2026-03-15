// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

namespace brenta
{

class RenderPipeline;
  
//
// App runner
// ----------
//
// This class provides a simple main, and a structure to organize the
// overall execution of an application in distinct phases. This is
// just for convenience since the main loop of many applications look
// the same.
//
// The user needs to define setup(), set_pipeline(), update(), draw()
// and cleanup(). Define BRENTA_MAIN before including this header to
// include a main function.
//
class App
{
public:

  App()  = delete;
  ~App() = delete;
  
  static bool setup();
  static tenno::shared_ptr<RenderPipeline> set_pipeline();
  static bool update(float delta_time);
  static bool draw(tenno::shared_ptr<RenderPipeline> pipeline);
  static void cleanup();
  
};

} // namespace brenta


#ifdef BRENTA_MAIN

int main()
{
  if (!brenta::App::setup())     // user implemented
    return 1;

  {
    auto pipeline = brenta::App::set_pipeline();  // user implemented
  
    while(!brenta::Window::should_close())
    {
      auto delta_time = brenta::Window::get_time().delta;
    
      if (!brenta::App::update(delta_time))  // user implemented
        break;

      if (!brenta::App::draw(pipeline))      // user implemented
        break;
    
      brenta::Window::poll_events();
      brenta::Window::swap_buffers();
    }

  }
  
  brenta::App::cleanup();   // user implemented
  return 0;
}

#endif
