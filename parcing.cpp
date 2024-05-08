#include "parcing.h"

// print tables
void print_line() {
    int size = 50;
    for (int i = 0; i < size; i++) std::cout << "-";
    std::cout << std::endl;
} // one line

void print_first_table() {
    print_line();

    std::cout << "| No |\tToken\t|  Length  |\t    Type   \t\t |\n";

    print_line();
} // table title

void print_first_table_line(int &n, const std::string &token,
                            const std::string &Type) {
    std::cout << "|" << std::setw(4) << n;
    std::cout << "|" << std::setw(10) << token;
    std::cout << "|" << std::setw(10) << token.size();
    std::cout << "|" << std::setw(21) << Type;
    std::cout << "|" << std::endl;
    n++;
} // info line of first table

void print_second_table(const int &label, const int &mnem, const int &count_mnem,
                        const int &op1, const int &count_op1, const int &op2, const int &count_op2) {
    print_line();

    std::cout << "| Label | Mnemocode |  Operand1 |    Operand2    |\n";

    print_line();

    std::cout << "|" << std::setw(7) << label;
    std::cout << "|" << std::setw(5) << mnem;
    std::cout << "|" << std::setw(5) << count_mnem;
    std::cout << "|" << std::setw(5) << op1;
    std::cout << "|" << std::setw(5) << count_op1;
    std::cout << "|" << std::setw(8) << op2;
    std::cout << "|" << std::setw(7) << count_op2;
    std::cout << "|" << std::endl;

    print_line();
} // full second table

// word parsing
// line structure: <label><mnem><op1>,<op2>
void label_word_parsing(std::string &word, int &count) {
    if (word[word.size() - 1] == ':') {
        word.resize(word.size() - 1);
        print_first_table_line(count, word, "user identifier");
        print_first_table_line(count, ":", "one char");
    } else {
        print_first_table_line(count, word, "user identifier");
    }
} // parsing <label> type

void mnem_word_parsing(std::string &word, int &count) {
    std::string type;
    std::transform(word.begin(), word.end(), word.begin(), tolower);

    if (word == ".model" || word == ".data" ||
        word == ".code" || word == "end") {
        type = "directive";
    } else if (word == "data" || word == "code") {
        type = "identifier";
    } else if (word == "cmpsb" || word == "inc" || word == "mov" || word == "dec" ||
               word == "jbe" || word == "bsr" || word == "and" ||
               word == "or" || word == "idiv" || word == "jmp") {
        type = "instruction";
    } else if (word == "db" || word == "dw" || word == "dd") {
        type = "type";
    } else if (word == "=") {
        type = "const assignment";
    }

    print_first_table_line(count, word, type);
} //parsing <mnem> type

void word_type(std::string &word, std::string &type, bool &is_status_changed) {
    if (word == "segment" || word == "ends") {
        type = "directive";
    } else if (word == ",") {
        type = "one char";
        is_status_changed = true;
    } else if (word == ":" || word == "[" || word == "]" ||
               word == "+" || word == "*" || word == "/" || word == "-") {
        type = "one char";
    } else if (word == "ss" || word == "ds" || word == "cs" ||
               word == "es" || word == "fs" || word == "gs") {
        type = "segment register";
    } else if (word == "ah" || word == "al" || word == "ch" || word == "cl" ||
               word == "dh" || word == "dl" || word == "bh" || word == "bl") {
        type = "8-bit register";
    } else if (word == "ax" || word == "bx" || word == "cx" || word == "si" ||
               word == "di" || word == "dx" || word == "sp" || word == "bp") {
        type = "16-bit register";
    } else if (word == "ebx" || word == "edi" || word == "eax" || word == "ecx" ||
               word == "edx" || word == "esp" || word == "ebp" || word == "esi") {
        type = "32-bit register";
    } else if (word[word.size() - 1] == 'h') {
        type = "hexadecimal";
    } else if (word[word.size() - 1] == 'b') {
        type = "binary";
    } else if (isdigit(word[0])) {
        type = "decimal";
    } else if (word == "dword") {
        type = "32-bits";
    } else if (word == "word") {
        type = "16-bits";
    } else if (word == "byte") {
        type = "8-bits";
    } else if (word == "ptr") {
        type = "pointer";
    } else if (word == "short") {
        type = "jump within [-128;127] bites";
    } else {
        type = "user identifier";
    }
} // determinate word<op> type

//valid recording forms:
/*
<label>:
<label> :
<label>/0 <mnem> <op1>
<mnem> <op1>, <op2>
<mnem> <op1> , <op2>
<mnem> <op: op [op +-/* op]>, <op2>/0
<mnem> <op : op [ op +-/* op ]>
 */

void op_word_parsing(std::string &word, int &count, bool &is_status_changed) {
    std::string type;
    std::transform(word.begin(), word.end(), word.begin(), tolower);

    bool is_coma = false;
    if (word[word.size() - 1] == ',') {
        word.resize(word.size() - 1);
        is_coma = true;
    } // we must reduce the length for next parsing, but info about ',' must be last

    if (word[0] == '[') {
        for (int i = 1; i < word.size(); i++) {
            word[i - 1] = word[i];
        }
        word.resize(word.size() - 1);
        print_first_table_line(count, "[", "one char");
    } // check symbol before word, reduce the length and print info about this symbol

    if (word[word.size() - 1] == ':') {
        word.resize(word.size() - 1);
        word_type(word, type, is_status_changed);
        print_first_table_line(count, word, type);
        print_first_table_line(count, ":", "one char");
    } else if (word[word.size() - 1] == ']') {
        word.resize(word.size() - 1);
        word_type(word, type, is_status_changed);
        print_first_table_line(count, word, type);
        print_first_table_line(count, "]", "one char");
    } else if (word[0] == '\'') {
        for (int i = 1; i < word.size(); i++) {
            word[i - 1] = word[i];
        }
        word.resize(word.size() - 2);
        print_first_table_line(count, "'", "one char");
        print_first_table_line(count, word, "symbols");
        print_first_table_line(count, "'", "one char");
    } else {
        word_type(word, type, is_status_changed);
        print_first_table_line(count, word, type);
    } // check symbol after word, reduce the length, print info about the word and symbol

    if (is_coma) {
        print_first_table_line(count, ",", "one char");
        is_status_changed = true;
    } // print info about the coma
} // parsing <op> type

// line parsing
std::string check_first_word(std::string &word) {
    std::transform(word.begin(), word.end(), word.begin(), tolower);
    std::vector<std::string> mnem = {".model", ".data", ".code", "end", "dec", "cmpsb",
                                     "mov", "inc", "jbe", "dec", "bsr", "and", "or", "jmp",
                                     "data", "code", "idiv"}; // all <mnem> codes

    for (auto &i: mnem) {
        if (word == i) return "<mnem>";
    }
    return "<label>";
}

void line_parsing(const std::string &data) {
    if (!data.empty()) {
        std::cout << data << std::endl;
        print_first_table();

        std::stringstream line;
        line << data; // use line us cin

        // info for second table
        int label = 0; // label type
        int code = 0; // first mnem position
        int count_code = 0; // number of mnem elements
        int op1 = 0; // first op1 position
        int count_op1 = 0; // number of op1 elements
        int op2 = 0; // first op2 position
        int count_op2 = 0; // number of op2 elements

        bool is_first_mnem = true; // check first mnem position
        bool is_first_op1 = true; // check first op1 position
        bool is_first_op2 = true; // check first op2 position
        bool is_status_changed = false; // check is we need change <op1> -> <op2>

        std::string word;
        std::string status;

        // check first word
        std::stringstream line_next;
        line_next << data;
        std::string next_word;
        line_next >> next_word;
        status = check_first_word(next_word); // <mnem> or <label>

        int token_count = 1;
        while (line >> word) {
            if (status == "<label>") {
                label_word_parsing(word, token_count);
                label = 1;
                status = "<mnem>"; // <label> has only one symbol
            } else if (status == "<mnem>") {
                if (is_first_mnem) {
                    code = token_count;
                    is_first_mnem = false;
                }
                mnem_word_parsing(word, token_count);
                count_code++;
                status = "<op1>"; // <mnem> has only one symbol
            } else if (status == "<op1>") {
                if (is_first_op1) {
                    op1 = token_count;
                    is_first_op1 = false;
                }
                op_word_parsing(word, token_count, is_status_changed);
                count_op1++;
                if (is_status_changed) status = "<op2>"; // is we have ',' after <op1>
            } else if (status == "<op2>") {
                if (is_first_op2) {
                    op2 = token_count;
                    is_first_op2 = false;
                }
                op_word_parsing(word, token_count, is_status_changed);
                count_op2++;
            }
        }

        print_line();
        print_second_table(label, code, count_code, op1, count_op1, op2, count_op2);
    }
}

// file parsing
void file_parsing(const std::string &name) {
    std::fstream file(name);

    if (!file.is_open()) {
        std::cout << "Open error" << std::endl;
    } else {
        std::string line;

        while (getline(file, line)) { // read line from file
            line_parsing(line);
        }
    }
}
