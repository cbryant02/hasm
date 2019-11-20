#pragma once
#include "include/common.hh"

namespace hasm {
    class instruction {
    public:
        instruction(std::string istring, unsigned line);
        virtual uint16_t assemble();

        static instruction* make_instruction(std::string source, std::string istring, unsigned line_number);
    protected:
        std::string istring;
        unsigned line;

        void throw_assemble_error(std::string message, unsigned pos);
    };
};