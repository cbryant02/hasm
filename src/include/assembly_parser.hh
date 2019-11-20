#pragma once
#include "include/instruction.hh"
#include <fstream>
#include <vector>
#include <map>

namespace hasm {
    class assembly_parser {
    public:
        std::map<std::string, unsigned> source_to_ln;
        std::map<std::string, std::string> stripped_to_source;
        std::vector<std::string> stripped;
        std::vector<hasm::instruction*> instructions;

        assembly_parser(std::ifstream& source);
    
    private:
        hasm::instruction* parse(std::string instruction);
    };
};