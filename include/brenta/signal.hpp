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

//
// Signals
// -------
//
// A signal is a simple communication mechanism that uses the observer
// pattern. You can subscribe to an event / signal by registering a
// callback which will be called if that signal is emitted. Signals
// are identified by a name (string).
//
// When a signal is emitted, it does not immediately call all
// callbacks. Instead it stored a buffer of pending events which can
// be consimed with the `update()` function. This makes sure that the
// user of the API decides when to spend time processing the events.
//
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

  // A subscription keeps the connection alive using RAII
  static Subscription subscribe(const SignalId& id, Callback callback);
  static void         unsubscribe(const Subscription &sub);

  // Consumes all pending events
  static void update();

  // Resets everything
  static void clear();
  
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

//
// Subscription
// ------------
//
// When you subscribe to an event, you get a subscription object. This
// uses RAII to keep the connection alive and automatically clean
// its resources when it goes out of scope.
//
class SignalManager::Subscription
{
public:

  friend class SignalManager;
  
  Subscription()                              = delete;

  Subscription(Subscription&& other)
  {
    if (this == &other) return;
    
    if (this->connection_id != 0)
      SignalManager::unsubscribe(*this);
    
    this->signal_id     = tenno::move(other.signal_id);
    this->connection_id = other.connection_id;
    other.signal_id     = SignalId{};
    other.connection_id = ConnectionId{};
  }
  Subscription &operator=(Subscription&& other)
  {
    if (this == &other) return *this;
    
    if (this->connection_id != 0)
      SignalManager::unsubscribe(*this);
    
    this->signal_id     = tenno::move(other.signal_id);
    this->connection_id = other.connection_id;
    other.signal_id     = SignalId{};
    other.connection_id = ConnectionId{};
    return *this;
  }

  // Copy is not allowed
  Subscription(const Subscription&)            = delete;
  Subscription &operator=(const Subscription&) = delete;

  ~Subscription();
   
private:

  Subscription(SignalId signal_id, ConnectionId connection_id)
    : signal_id(signal_id), connection_id(connection_id) {}

  SignalId       signal_id;
  ConnectionId   connection_id;   // 0 = uninitialized
  
};
  
} // namespace brenta
