#include "command_line_args_handler.h"

#include <fstream>
#include <iostream>
#include <thread>


namespace A {


command_line_args_handler::command_line_args_handler(std::string file_path,
                                                     std::string threads_num) {
  try {
    threads_num_ = std::stoi(threads_num);
  } catch (...) {
    std::cout << "Bad threads amount arg. Stop" << std::endl;
    std::exit(0);
  }

  if (threads_num_ < 1 || threads_num_ > std::thread::hardware_concurrency()) {
    std::cout << "Warning: Uncorrect threads amount value passed: " << threads_num_
              << ". App will be launched with 2 threads\n";
    threads_num_ = default_threads_num_;
  }

  try {
    read_file(file_path);
  } catch (const std::exception& e) {
    std::cout << "Error while file reading: " << e.what() << std::endl;
    std::exit(0);
  }
}


std::string&& command_line_args_handler::file_content() noexcept {
  return std::move(file_content_);
}


unsigned command_line_args_handler::threads_num() const noexcept {
  return threads_num_;
}


void command_line_args_handler::read_file(const std::string& path) {
  file_content_.clear();

  constexpr auto read_size = std::size_t{4096};
  auto stream = std::ifstream{path};
  stream.exceptions(std::ios_base::badbit);

  auto buf = std::string(read_size, '\0');
  while (stream.read(& buf[0], read_size)) {
    file_content_.append(buf, 0, stream.gcount());
  }
  file_content_.append(buf, 0, stream.gcount());
}

}
