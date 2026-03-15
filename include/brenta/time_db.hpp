// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <typeindex>
#include <typeinfo>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <concepts>
#include <algorithm>

namespace brenta
{
  
using TypeId = const void*;

template<typename T>
inline constexpr TypeId type_id = &type_id<T>;

using TimeStamp = int64_t;

template<typename T>
concept TimeRow = requires(T r)
{
  requires std::same_as<
    std::remove_cvref_t<decltype(r.timestamp)>,
    TimeStamp
    >;
};
  
//
// TimeDB
// ------
//
// Very simple time-series database.
//
class TimeDB
{
public:
  
  struct ITable
  {
    virtual ~ITable() = default;
  };

  template<TimeRow Row>
  class Table : public ITable
  {
  public:

    void insert(Row row)
    {
      rows.push_back(std::move(row));
    }

    void remove(TimeStamp from, TimeStamp to)
    {
      rows.erase(std::remove_if(rows.begin(), rows.end(),
                                [&](const Row& r)
                                {
                                  return r.timestamp >= from && r.timestamp <= to;
                                }),
                 rows.end());
    }

    std::vector<const Row*> range(TimeStamp from, TimeStamp to) const
    {
      std::vector<const Row*> result;
    
      for (const auto& r : rows)
        {
          if (r.timestamp >= from && r.timestamp <= to)
            result.push_back(&r);
        }

      return result;
    }

    template<typename Predicate>
    std::vector<const Row*> query(Predicate pred) const
    {
      std::vector<const Row*> result;

      for (const auto& r : rows)
        {
          if (pred(r))
            result.push_back(&r);
        }

      return result;
    }
  
  private:

    std::vector<Row> rows;
  };
  
  template<TimeRow Row>
  void create_table()
  {
    TypeId id = type_id<Row>;
    
    if (tables.contains(id))
      return;

    tables[id] = std::make_unique<Table<Row>>();
  }

  template<TimeRow Row>
  void insert(Row row)
  {
    table<Row>().insert(std::move(row));
  }

  template<TimeRow Row>
  void remove(TimeStamp from, TimeStamp to)
  {
    table<Row>().remove(from, to);
  }

  template<TimeRow Row>
  std::vector<const Row*> range(TimeStamp from, TimeStamp to)
  {
    return table<Row>().range(from, to);
  }

  template<TimeRow Row, typename Predicate>
  std::vector<const Row*> query(Predicate pred)
  {
    return table<Row>().query(pred);
  }

private:

  template<TimeRow Row>
  Table<Row>& table()
  {
    TypeId id = type_id<Row>;

    auto it = tables.find(id);
    
    if (it == tables.end())
    {
      auto tbl = std::make_unique<Table<Row>>();
      auto* ptr = tbl.get();

      tables[id] = std::move(tbl);

      return *ptr;
    }

    return *static_cast<Table<Row>*>(it->second.get());
  }
  
private:

  std::unordered_map<TypeId, std::unique_ptr<ITable>> tables;
};

} // namespace brenta
