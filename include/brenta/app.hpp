// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

namespace brenta
{

/**
 * @brief App runner
 *
 * The user needs to define setup(), update() and cleanup(). Define
 * BRENTA_MAIN before including this header to define a main function.
 */
class App
{
public:

  App()  = delete;
  ~App() = delete;
  
  static bool setup();
  static bool update(float delta_time);
  static void cleanup();
  
};

} // namespace brenta


#ifdef BRENTA_MAIN

int main()
{
  if (!brenta::App::setup())     // user implemented
    return 1;
    
  while(!brenta::Window::should_close())
  {
    auto delta_time = brenta::Window::get_time().get_delta();
    
    if (!brenta::App::update(delta_time))  // user implemented
      break;
    
    brenta::Window::poll_events();
    brenta::Window::swap_buffers();
  }
  
  brenta::App::cleanup();   // user implemented
  return 0;
}

#endif
