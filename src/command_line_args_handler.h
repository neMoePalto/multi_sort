#pragma once

#include <string>


namespace A {


class command_line_args_handler {
public:
  command_line_args_handler(std::string file_path, std::string threads_num);
  std::string&& file_content() noexcept;
  unsigned threads_num() const noexcept;

private:
  void read_file(const std::string& path);

private:
  constexpr static unsigned default_threads_num_ = 2;
  std::string               file_content_;
  unsigned                  threads_num_;
};

}
