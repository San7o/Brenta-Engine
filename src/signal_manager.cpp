// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/signal_manager.hpp>

using namespace brenta;

//
// Static storage
//

tenno::vector<SignalManager::Event> SignalManager::pending_events;
std::unordered_map<SignalManager::SignalId,
                   tenno::vector<std::pair<SignalManager::ConnectionId,
                                           SignalManager::Callback>>>
SignalManager::registry;

//
// Member functions
//

void SignalManager::emit(const SignalManager::Event &event)
{
  SignalManager::pending_events.push_back(event);
}

SignalManager::Subscription
SignalManager::subscribe(const SignalManager::SignalId &id,
                         SignalManager::Callback callback)
{
  static ConnectionId connection_id = 0;
  connection_id++;
  
  SignalManager::registry[id].push_back(std::make_tuple(connection_id, callback));
  return Subscription(id, connection_id);
}

void SignalManager::unsubscribe(const SignalManager::Subscription &sub)
{
  auto& entries = SignalManager::registry[sub.signal_id];
  for (auto it = entries.begin(); it != entries.end(); ++it)
  {
    if (std::get<0>(*it) == sub.connection_id)
    {
      entries.erase(it);
      break;
    }
  }
}

void SignalManager::update()
{
  for (auto& event : SignalManager::pending_events)
  {
    if (!SignalManager::registry.contains(event.id)) continue;

    for (auto& [_, callback] : SignalManager::registry[event.id])
    {
      callback(event.message);
    }
  }
}
  
SignalManager::Subscription::~Subscription()
{
  if (this->connection_id == 0) return;
  SignalManager::unsubscribe(*this);
}
