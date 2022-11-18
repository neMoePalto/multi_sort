#pragma once

#include <future>
#include <string>
#include <vector>

#include <boost/range.hpp>


namespace A {


class async_task_runner {
  using range = boost::iterator_range<std::string::iterator>;
  using chunk_pairs = std::vector<std::pair<std::string, std::string>>;

private:
  range sort(range substr) noexcept;

public:
  async_task_runner(std::string&& file_content);
  void run_tasks(std::size_t threads_count);
  std::string merge_results() noexcept;

private:
  std::string pairwise_merge(const chunk_pairs& ch_pairs) noexcept;

private:
  std::vector<std::future<range>> futures_;
  std::string                     file_content_;
};

}
