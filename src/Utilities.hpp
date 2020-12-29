#pragma once

#include <memory>

template <typename TDest, typename TSrc>
std::unique_ptr<TDest> dynamic_pointer_cast(std::unique_ptr<TSrc> &src)
{
  if (!src)
    return std::unique_ptr<TDest>(nullptr);
  TDest *dest_ptr = dynamic_cast<TDest *>(src.get());
  if (!dest_ptr)
    return std::unique_ptr<TDest>(nullptr);
  std::unique_ptr<TDest> dest_temp(dest_ptr);
  src.release();
  return dest_temp;
}
