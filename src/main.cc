#include "include/common.hh"
#include "include/assembly_parser.hh"
#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>

#ifdef _WIN32
static const std::string sh_warn;
static const std::string eh_warn;
#else
static const std::string sh_warn = "\033[93;1m";
static const std::string eh_warn = "\033[0m";
#endif

int except(std::exception* ex) {
    std::cout << ex->what() << std::endl;
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return except(new std::invalid_argument("Please provide a filename."));
    }

    // Open input file
    std::string fin_name(argv[1]);
    std::ifstream fin(fin_name);
    // Check input file state
    if (!fin.is_open() || fin.fail()) {
        return except(new std::invalid_argument(fin_name + " is inaccessible or invalid."));
    }
    // Warn if input file is not an .asm/.s file
    if (fin_name.find(".asm") == std::string::npos && fin_name.find(".s") == std::string::npos) {
        std::cout << sh_warn << "warning: " << eh_warn << fin_name << " might not an assembly file. (.asm/.s missing)"
                  << std::endl;
    }

    // Open output file and check state
    auto extension_index = fin_name.find_last_of(".");
    std::string fout_name =
            fin_name.substr(0, (extension_index == std::string::npos) ? fin_name.length() : extension_index) + ".hack";
    std::ofstream fout(fout_name, std::ios::binary);
    if (!fout.is_open() || fout.fail()) {
        return except(new std::invalid_argument("Could not open " + fout_name + "."));
    }

    // Create parser
    hasm::assembly_parser fasm(fin);

    unsigned int processed_count = 1;
    std::cout << fin_name << " -> " << fout_name << std::endl;
    for (auto inst : fasm.instructions) {
        std::cout << processed_count << " of " << fasm.stripped.size() << "\r";
        std::string iasm;
        try {
            iasm = std::bitset<16>(inst->assemble()).to_string();
        } catch (std::exception& e) {
            return except(&e);
        }

        fout << iasm << "\n";
        ++processed_count;
    }
    fout << std::flush;
    std::cout << std::endl << "Done!";

    return 0;
}