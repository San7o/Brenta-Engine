// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/signal_manager.hpp>
#include <brenta/logger.hpp>
#include <valfuzz/test.hpp>

using namespace brenta;

BEFORE()
{
  SignalManager::clear();
}

AFTER()
{
  SignalManager::clear();
}

TEST(signal_basic_emit, "Emit and consume a single signal")
{
  bool called = false;
  std::string received_msg = "";

  {
    auto sub = SignalManager::subscribe("test_signal",
                                        [&](SignalManager::Message msg)
    {
      called = true;
      received_msg = msg;
    });

    SignalManager::emit({"test_signal", "hello_world"});

    // Signal should NOT be called yet (deferred)
    ASSERT(called == false);

    SignalManager::update();

    ASSERT(called == true);
    ASSERT(received_msg == "hello_world");
  }
}

TEST(signal_multiple_subscribers, "Multiple subscribers for the same signal")
{
  int count_a = 0;
  int count_b = 0;

  auto sub_a = SignalManager::subscribe("multi", [&](auto) { count_a++; });
  auto sub_b = SignalManager::subscribe("multi", [&](auto) { count_b++; });

  SignalManager::emit({"multi", "event_1"});
  SignalManager::emit({"multi", "event_2"});

  SignalManager::update();

  ASSERT(count_a == 2);
  ASSERT(count_b == 2);
}

TEST(signal_raii_unsubscribe, "Subscription RAII automatically unsubscribes")
{
  int call_count = 0;

  {
    auto sub = SignalManager::subscribe("raii_test", [&](auto) {
      call_count++;
    });
    
    SignalManager::emit({"raii_test", "should_fire"});
    SignalManager::update();
    ASSERT(call_count == 1);
  } // sub goes out of scope here

  SignalManager::emit({"raii_test", "should_not_fire"});
  SignalManager::update();

  // Count should still be 1
  ASSERT(call_count == 1);
}

TEST(signal_move_semantics, "Moving a subscription transfers ownership")
{
  int call_count = 0;
  
  auto callback = [&](auto) { call_count++; };
  
  {
    SignalManager::Subscription outer_sub =
      SignalManager::subscribe("move_test", callback);
    
    {
      SignalManager::Subscription inner_sub =
        SignalManager::subscribe("move_test", callback);
      outer_sub = std::move(inner_sub); 
      // inner_sub is now "empty", outer_sub owns the inner's connection
    } 

    SignalManager::emit({"move_test", "msg"});
    SignalManager::update();
    
    // We expect 1 call because the original outer_sub was overwritten 
    // and the inner_sub was moved to outer.
    ASSERT(call_count == 1);
  }

  SignalManager::emit({"move_test", "msg"});
  SignalManager::update();

  ASSERT(call_count == 1); // No new calls after outer_sub dies
}

TEST(signal_buffer_clearing, "Update consumes all pending events")
{
  int call_count = 0;
  auto sub = SignalManager::subscribe("buffer_test",
                                      [&](auto) { call_count++; });

  SignalManager::emit({"buffer_test", "1"});
  SignalManager::emit({"buffer_test", "2"});
  
  SignalManager::update();
  ASSERT(call_count == 2);

  // Calling update again without new emits should do nothing
  SignalManager::update();
  ASSERT(call_count == 2);
}
