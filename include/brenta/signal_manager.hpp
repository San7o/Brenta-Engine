// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include <tenno/vector.hpp>

namespace brenta
{

class SignalManager
{
public:
  
  using SignalId     = std::string;
  using Message      = std::string;
  using ConnectionId = int;
  using Callback     = std::function<void(Message)>;

  struct Event;
  class  Subscription;

  SignalManager()  = delete;
  ~SignalManager() = delete;

  static void emit(const Event& event);
  
  static Subscription subscribe(const SignalId& id, Callback callback);
  static void unsubscribe(const Subscription &sub);

  // Process all pending events
  static void update();
  
private:

  static tenno::vector<Event> pending_events;
  static std::unordered_map<SignalId,
                            tenno::vector<std::pair<ConnectionId,
                                                    Callback>>> registry;
  
};

struct SignalManager::Event
{
  SignalId id;
  Message  message;
};
  
class SignalManager::Subscription
{
public:

  friend class SignalManager;
  
  Subscription() = delete;
  ~Subscription();
   
private:

  Subscription(SignalId signal_id, ConnectionId connection_id)
    : signal_id(signal_id), connection_id(connection_id) {}

  SignalId       signal_id;
  ConnectionId   connection_id;
};

  
} // namespace brenta
