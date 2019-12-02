#include "include/common.hh"
#include "include/instruction.hh"
#include <memory>
#include <stdexcept>

using hasm::instruction;

namespace hasm {
    class ainstruction : public instruction {
    public:
        ainstruction(std::string istring, unsigned line) : instruction(istring, line) {
            this->istring = istring;
        }

        uint16_t assemble() override {
            uint16_t address;
            try {
                int temp = std::stoi(istring.substr(1));
                if (temp < 0 || temp > 65535) throw std::out_of_range("");
                address = std::stoi(istring.substr(1));
            } catch (const std::invalid_argument& e) {
                throw_assemble_error("Could not convert " + istring.substr(1) + " to an integer", 1);
            } catch (const std::out_of_range& e) {
                throw_assemble_error("Address is out of range -- must be within 0 to 65535", 1);
            }

            return 0x7FFF & address;
        }

    private:
        std::string istring;
    };
};