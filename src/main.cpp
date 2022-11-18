#include <fstream>
#include <iostream>

#include "async_task_runner.h"
#include "command_line_args_handler.h"


int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Please check the argument's number (required 2: input file and threads amount). Stop" << std::endl;
    return 0;
  }

  A::command_line_args_handler args_handler(argv[1], argv[2]);

  A::async_task_runner task_runner(args_handler.file_content());
//  std::cout << "After moving: " << args_handler.file_content().size() << std::endl;
  task_runner.run_tasks(args_handler.threads_num());

  const std::string filename{"sorted" + std::to_string(args_handler.threads_num()) +  ".txt"};
  auto out = std::ofstream{filename};
  out << task_runner.merge_results();

  return 0;
}
