#ifndef INTERACTIVE_SIMULATOR_HPP_
#define INTERACTIVE_SIMULATOR_HPP_

#include "simulator.hpp"
#include <string>

namespace simulator {

class InteractiveSimulator {
 private:
    Simulator simulator_;
    bool running_ = true;

 public:
    InteractiveSimulator(std::size_t memory_size);

    void start();

 private:
    void load_program(const std::string& filename);
};

} // namespace simulator

#endif // INTERACTIVE_SIMULATOR_HPP_

