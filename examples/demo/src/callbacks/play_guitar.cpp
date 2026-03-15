// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/asset.hpp>
#include <brenta/sound.hpp>
#include <brenta/input.hpp>
#include <brenta/logger.hpp>

#include <demo/callbacks.hpp>

using namespace brenta;

void init_play_guitar_callback()
{
  auto guitar_asset_builder = SoundAsset::Builder()
    .path("examples/assets/audio/guitar.wav");
  auto guitar_asset =
    AssetManager::new_asset<SoundAsset>("guitar",
                                        guitar_asset_builder);
  if (!guitar_asset)
  {
    ERROR("Demo: error loading guitar sound");
    return;
  }
  
  auto play_guitar = [guitar_asset]() {
    static Sound guitar_sound = Sound(*guitar_asset);
    guitar_sound.play();
  };
  Input::add_keyboard_callback(Key::P, play_guitar);
}
