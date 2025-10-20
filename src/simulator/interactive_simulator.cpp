#include "interactive_simulator.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace simulator {
InteractiveSimulator::InteractiveSimulator(std::size_t memory_size) : simulator_(memory_size) {}

void InteractiveSimulator::start() {
  std::cout << "Simulator started. Type 'help' for commands.\n";

  std::string line;
  while (running_) {
    std::cout << "> ";
    std::getline(std::cin, line);

    if (line == "sr" || line == "set_register") {
      int reg;
      int value;
      std::cin >> reg >> value;
      std::cin.ignore(); 
      simulator_.get_cpu().set_register(reg, value);
      std::cout << "R" << reg << " = " << value << "\n";
    }
    else if (line == "run_cycle") {
      simulator_.get_cpu().pipeline_cycle();
      std::cout << "Cycle executed. PC = " << simulator_.get_cpu().get_pc() << "\n";
    }
    else if (line == "run_program") {
      simulator_.get_cpu().run_program();
      std::cout << "Program executed.\n";
    }
    else if (line == "print_reg") {
      simulator_.get_cpu().print_registers();
    }
    else if (line == "load") {
      std::string filename;
      std::cin >> filename;
      std::cin.ignore(); 
      load_program(filename);
    }
    else if (line == "reset") {
      for (int i = 0; i < 32; i++) {
        simulator_.get_cpu().set_register(i, 0);
      }
      simulator_.get_cpu().set_pc(0);
      std::cout << "All registers and PC reset to 0\n";
    }
    else if (line == "set_pc" || line == "sp") {
      int pc_value;
      std::cin >> pc_value;
      std::cin.ignore();
      simulator_.get_cpu().set_pc(pc_value);
      std::cout << "PC = " << pc_value << "\n";
    }
    else if (line == "help") {
      std::cout << "Commands:\n";
      std::cout << "set_register(sr) - set register (then enter reg number and value)\n";
      std::cout << "set_pc(sp) - set program counter\n";
      std::cout << "run_cycle - execute one cycle\n";
      std::cout << "run_program - run program to completion\n";
      std::cout << "print_reg - show registers\n";
      std::cout << "load - load program from file\n";
      std::cout << "reset - reset all registers and PC to 0\n";
      std::cout << "exit - quit\n";
    }
    else if (line == "exit") {
      running_ = false;
    }
    else {
      std::cout << "Unknown command. Type 'help'.\n";
    }
  }
}

void InteractiveSimulator::load_program(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cout << "Cannot open file: " << filename << "\n";
    return;
  }

  std::vector<uint8_t> program(
    (std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>()
  );

  simulator_.load_program(program);
  std::cout << "Program loaded: " << program.size() << " bytes\n";
} 
} // namespace simulator
