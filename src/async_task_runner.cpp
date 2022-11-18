#include "async_task_runner.h"

#include <iostream>
#include <thread>


namespace A {


void print_substring(std::string::iterator from, unsigned len) noexcept {
  for (auto it = from; it != from + len; ++it) {
    std::cout << *it;
  }
  std::cout << '\n';
}


async_task_runner::range async_task_runner::sort(range substr) noexcept {
//  print_substring(substr.begin(), 30);

  // Using std::sort. Other sort algorithm may be used instead
  std::sort(substr.begin(), substr.end());
//  print_substring(substr.begin(), 30);

  return {substr.begin(), substr.end()};
}


async_task_runner::async_task_runner(std::string&& file_content)
  : file_content_(std::move(file_content)) {
//  std::cout << file_content_.size() << " and " << file_content.size() << std::endl;
}


void async_task_runner::run_tasks(std::size_t used_threads) {
  futures_.clear();

  std::size_t chunk_size = file_content_.size() / used_threads;
  for (unsigned i = 0; i < used_threads; ++i) {
    std::string::iterator it_begin = file_content_.begin() + i * chunk_size;
    std::string::iterator it_end = (i == used_threads - 1) ? file_content_.end() : it_begin + chunk_size;

    auto fut = std::async(std::launch::async,
                          &async_task_runner::sort,
                          this,
                          range{it_begin, it_end});
    futures_.emplace_back(std::move(fut));
  }
}


std::string async_task_runner::merge_results() noexcept {
  chunk_pairs chunks;

  for (auto& f : futures_) {
    range sorted = f.get();
    if (chunks.empty() || !chunks.back().second.empty()) {
      chunks.push_back({{sorted.begin(), sorted.end()}, std::string{}});
    } else {
      chunks.back().second = {sorted.begin(), sorted.end()};
    }
  }

  return pairwise_merge(chunks);
}


std::string async_task_runner::pairwise_merge(const chunk_pairs& ch_pairs) noexcept {
  for (const auto& ch : ch_pairs) {
    std::cout << ch.first.size() << " and " << ch.second.size() << std::endl;
  }
  std::cout << "------------------\n";

  if (ch_pairs.size() == 1 && ch_pairs.back().second.empty()) {
    return ch_pairs.back().first;
  }

  chunk_pairs pairs_after_merge;
  for (const auto& p : ch_pairs) {
    std::string merged_str;
    if (p.second.empty()) {
      merged_str = std::move(p.first); // Нет пары для проведения мерджа, берем последовательность как есть
    } else {
      merged_str.reserve(p.first.size() + p.second.size());
      auto it2 = p.second.cbegin();
      for (auto it = p.first.cbegin(); it != p.first.cend() || it2 != p.second.cend();) {
        if (it == p.first.cend() ||
            (it2 != p.second.cend() && *it > *it2)) {
          merged_str.push_back(*it2);
          ++it2;
        } else {
          merged_str.push_back(*it);
          ++it;
        }
      }
    }

    if (pairs_after_merge.empty() || !pairs_after_merge.back().second.empty()) {
      pairs_after_merge.push_back({std::move(merged_str), std::string{}});
    } else {
      pairs_after_merge.back().second = std::move(merged_str);
    }
  }

  return pairwise_merge(pairs_after_merge);
}

}
