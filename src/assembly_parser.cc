#include "include/assembly_parser.hh"
#include "include/instruction.hh"
#include "include/symbols.hh"
#include <functional>
#include <algorithm>
#include <memory>

using hasm::assembly_parser;

static std::string strip_whitespace(std::string s) {
    char whitespace[] = {'\n', '\r', '\t', ' '};
    std::string ret = "";
    for (auto ch : s) {
        if (std::find(std::begin(whitespace), std::end(whitespace), ch) != std::end(whitespace)) continue;
        ret = ret + ch;
    }
    return ret;
}

assembly_parser::assembly_parser(std::ifstream& source) {
    hasm::symbols symbols;

    // Pass 1: remove comments and symbols
    std::string line;
    unsigned line_number = 0;
    unsigned line_number_raw = 0;
    while (std::getline(source, line)) {
        line = strip_whitespace(line);

        source_to_ln.emplace(line, line_number_raw);
        line_number_raw++;

        if (line.size() == 0 || line.at(0) == '/') continue;

        std::string fs;
        for (auto c : line) {
            // Skip comments
            if (c == '/') break;

            // Skip symbols and add them to the symbol table
            if (c == '(') {
                std::string symbol_name = line.substr(1, line.find(')') - 1);
                symbols.add(symbol_name, line_number);
                break;
            }

            fs.push_back(c);
        }

        if (fs.empty()) continue;
        stripped_to_source.emplace(fs, line);
        stripped.push_back(fs);
        line_number++;
    }

    // Pass 2: replace symbol references
    for (int i = 0; i < stripped.size(); i++) {
        auto istring = stripped.at(i);
        if (istring.at(0) == '@') {
            auto symbol_name = istring.substr(1);
            if (symbols.contains(symbol_name)) {
                auto new_inst = '@' + std::to_string(symbols.get(symbol_name));
                stripped.at(i) = new_inst;

                if (stripped_to_source.find(istring) != stripped_to_source.end()) stripped_to_source.erase(istring);
                stripped_to_source.emplace(new_inst, istring);
            } else {
                if (!symbol_name.empty() && !std::all_of(symbol_name.begin(), symbol_name.end(), ::isdigit)) {
                    symbols.push(symbol_name);
                    auto new_inst = '@' + std::to_string(symbols.get(symbol_name));
                    stripped.at(i) = new_inst;

                    if (stripped_to_source.find(istring) != stripped_to_source.end()) stripped_to_source.erase(istring);
                    stripped_to_source.emplace(new_inst, istring);
                }
            }
        }
    }

    // Pass 3: construct
    unsigned prev = 0;
    for (int i = 0; i < stripped.size(); i++) {
        auto istring = stripped.at(i);
        auto source = stripped_to_source.at(istring);

        if (istring.at(0) == '@' && symbols.contains_reverse(std::stoi(istring.substr(1)))) {
            source = '@' + symbols.get_reverse(std::stoi(istring.substr(1)));
        }

        auto line = source_to_ln.at(source);
        instructions.push_back(hasm::instruction::make_instruction(source, istring, line + 1));
    }
}