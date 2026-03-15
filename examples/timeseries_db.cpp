// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/time_db.hpp>

#include <iostream>

using namespace brenta;

struct FpsRow
{
  TimeStamp timestamp;
  int fps;
};

int main()
{
  TimeDB db;
  db.create_table<FpsRow>();
  
  for (int i = 0; i < 10; i++)
  {
    db.insert<FpsRow>({
        .timestamp = i * 1000,
        .fps = 50 + i
      });
    }
  
  auto rows = db.range<FpsRow>(2000, 7000);

  for (auto r : rows)
    std::cout << r->fps << "\n";

  auto high = db.query<FpsRow>([](const FpsRow& r)
  {
    return r.fps > 55;
  });
  
  std::cout << "----\n";
    
  for (auto r : high)
    std::cout << r->fps << "\n";
}
