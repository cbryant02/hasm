#include "include/symbols.hh"

using hasm::symbols;

symbols::symbols() {
    // Emplace builtin symbols
    symbol_map.emplace("SP", 0x0);
    symbol_map.emplace("LCL", 0x1);
    symbol_map.emplace("ARG", 0x2);
    symbol_map.emplace("THIS", 0x3);
    symbol_map.emplace("THAT", 0x4);
    for (int i = 0; i <= 0xF; i++) {
        symbol_map.emplace("R" + std::to_string(i), i);
    }
    symbol_map.emplace("SCREEN", 0x4000);
    symbol_map.emplace("KBD", 0x6000);
}

void symbols::add(std::string symbol, uint16_t addr) {
    symbol_map.emplace(symbol, addr);
    reverse_map.emplace(addr, symbol);
}

void symbols::push(std::string symbol) {
    auto next = ++stack_pos;
    symbol_map.emplace(symbol, next);
    reverse_map.emplace(next, symbol);
}

bool symbols::contains(std::string symbol) {
    return symbol_map.find(symbol) != symbol_map.end();
}

uint16_t symbols::get(const std::string symbol) {
    return symbol_map.at(symbol);
}

bool symbols::contains_reverse(uint16_t addr) {
    return reverse_map.find(addr) != reverse_map.end();
}

const std::string symbols::get_reverse(uint16_t addr) {
    return reverse_map.at(addr);
}