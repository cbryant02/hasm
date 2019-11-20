#include "include/instruction.hh"
#include <regex>
#include <map>
using hasm::instruction;

namespace hasm {
    class cinstruction : public instruction {
    private:
        static std::vector<std::string> split(const std::string& str, const std::string& token) {
            std::vector<std::string> parts;
            size_t start, end = 0;
            while (end < str.size()) {
                start = end;
                while (start < str.size() && (token.find(str[start]) != std::string::npos)) {
                    start++;  // skip initial whitespace
                }
                end = start;
                
                while (end < str.size() && (token.find(str[end]) == std::string::npos)) {
                    end++; // skip to end of word
                }

                if (end-start != 0) {  // just ignore zero-length strings.
                    parts.push_back(std::string(str, start, end-start));
                }
            }
            return parts;
        }

        static std::vector<std::string> decompose(std::string istring) {
            std::vector<std::string> components;
            bool has_eq = (istring.find('=') != std::string::npos);
            bool has_semi = (istring.find(';') != std::string::npos);
            
            if(has_eq) {
                components = split(istring, "=");
                if(has_semi) {
                    components.push_back(split(components.at(1), ";").at(1));
                } else {
                    components.push_back("");
                }
            } else if(has_semi) {
                components = split(istring, ";");
                components.insert(components.begin(), "");
            }
            auto test = components.at(0);
            return components;
        }

        uint16_t lookup_error_handling(std::map<std::string, uint16_t> m, std::string key) {
            uint16_t ret;
            try {
                ret = m.at(key);
            } catch (const std::out_of_range& e) {
                std::string exp;
                if (m == comp_table) exp = comp;
                else exp = key;

                throw_assemble_error("\"" + exp + "\" is not a valid C-instruction mnemonic", this->istring.find(exp));
            }
            return ret;
        }
    
    public:
        cinstruction(std::string istring, unsigned line) : instruction(istring, line) {
            std::vector<std::string> components = decompose(istring);
            this->dest = components[0];
            this->comp = components[1];
            this->jump = components[2];
        }

        uint16_t assemble() override {
            uint16_t a_flag = 0b111'0'000000'000'000;
            if(comp.find("M") != std::string::npos) a_flag = 0b111'1'000000'000'000;
            std::string comp_f = std::regex_replace(comp, std::regex("A|M"), "X");

            uint16_t res = lookup_error_handling(dest_table, dest) |
                    lookup_error_handling(comp_table, comp_f) |
                    lookup_error_handling(jump_table, jump) |
                    a_flag;
            return res;
        }

    private:
        std::string dest;
        std::string comp;
        std::string jump;

        std::map<std::string, uint16_t> dest_table = {
            {"",    0b111'0'000000'000'000},
            {"M",   0b111'0'000000'001'000},
            {"D",   0b111'0'000000'010'000},
            {"MD",  0b111'0'000000'011'000},
            {"A",   0b111'0'000000'100'000},
            {"AM",  0b111'0'000000'101'000},
            {"AD",  0b111'0'000000'110'000},
            {"AMD", 0b111'0'000000'111'000}
        };

        std::map<std::string, uint16_t> comp_table = {
            {"0",   0b111'0'101010'000'000},
            {"1",   0b111'0'111111'000'000},
            {"-1",  0b111'0'111010'000'000},
            {"D",   0b111'0'001100'000'000},
            {"X",   0b111'0'110000'000'000},
            {"!D",  0b111'0'001101'000'000},
            {"!X",  0b111'0'110001'000'000},
            {"-D",  0b111'0'001111'000'000},
            {"-X",  0b111'0'110011'000'000},
            {"D+1", 0b111'0'011111'000'000},
            {"X+1", 0b111'0'110111'000'000},
            {"D-1", 0b111'0'001110'000'000},
            {"X-1", 0b111'0'110010'000'000},
            {"D+X", 0b111'0'000010'000'000},
            {"D-X", 0b111'0'010011'000'000},
            {"X-D", 0b111'0'000111'000'000},
            {"D&X", 0b111'0'000000'000'000},
            {"D|X", 0b111'0'010101'000'000}
        };

        std::map<std::string, uint16_t> jump_table = {
            {"",    0b111'0'000000'000'000},
            {"JGT", 0b111'0'000000'000'001},
            {"JEQ", 0b111'0'000000'000'010},
            {"JGE", 0b111'0'000000'000'011},
            {"JLT", 0b111'0'000000'000'100},
            {"JNE", 0b111'0'000000'000'101},
            {"JLE", 0b111'0'000000'000'110},
            {"JMP", 0b111'0'000000'000'111}
        };
    };
};