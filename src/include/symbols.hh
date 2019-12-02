#pragma once

#include "common.hh"
#include <map>

namespace hasm {
    class symbols {
    public:
        symbols();

        void add(const std::string symbol, const uint16_t address);

        void push(const std::string symbol);

        bool contains(const std::string symbol);

        uint16_t get(const std::string symbol);

        bool contains_reverse(uint16_t address);

        const std::string get_reverse(uint16_t address);

    private:
        std::map<const std::string, uint16_t> symbol_map;
        std::map<uint16_t, const std::string> reverse_map;
        uint16_t stack_pos = 0xF;
    };
};