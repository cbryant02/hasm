#include "include/instruction.hh"
#include "ainstruction.cc"
#include "cinstruction.cc"

using hasm::instruction;

#ifdef _WIN32
static const std::string sh_error;
static const std::string eh_error;
#else
static const std::string sh_error = "\033[31;1m";
static const std::string eh_error = "\033[0m";
#endif

instruction::instruction(std::string istring, unsigned line) {
    this->istring = istring;
    this->line = line;
}

instruction* instruction::make_instruction(std::string source, std::string istring, unsigned line) {
    if (istring.at(0) == '@') return dynamic_cast<instruction*>(new hasm::ainstruction(istring, line));
    return dynamic_cast<instruction*>(new hasm::cinstruction(istring, line));
}

uint16_t instruction::assemble() {
    return -1;
}

void instruction::throw_assemble_error(std::string message, unsigned pos) {
    std::ostringstream s;
    s << "line " << this->line << ": " << sh_error << "error: " << eh_error << message << "\n\t" <<
      istring.insert(pos, sh_error) << "\n\t" <<
      std::string(pos, ' ') << sh_error << '^' << eh_error << std::flush;
    throw std::invalid_argument(s.str());
}