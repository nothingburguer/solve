#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <thread>
#include <sstream>
#include <vector>
#include <mutex>

std::mutex mcout;

enum class TokenType {
    Identifier,
    Number,
    String,
    Char,
    Operator,
    Punct,
    NewLine,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
};

static bool is_ident_start(char c) {
    return std::isalpha((unsigned char)c) || c == '_';
}

static bool is_ident(char c) {
    return std::isalnum((unsigned char)c) || c == '_';
}

static bool is_operator(char c) {
    std::string ops = "+-*/=%!<>|&^";
    return ops.find(c) != std::string::npos;
}

static bool is_punct(char c) {
    std::string p = "(){},.;[]";
    return p.find(c) != std::string::npos;
}

std::vector<Token> lex(const std::string& src) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < src.size()) {
        char c = src[i];

        if (c == '\n') {
            tokens.push_back({TokenType::NewLine, "\\n"});
            i++;
            continue;
        }

        if (std::isspace((unsigned char)c)) {
            i++;
            continue;
        }

        if (is_ident_start(c)) {
            size_t start = i++;
            while (i < src.size() && is_ident(src[i])) i++;
            tokens.push_back({TokenType::Identifier, src.substr(start, i-start)});
            continue;
        }

        if (std::isdigit((unsigned char)c)) {
            size_t start = i++;
            while (i < src.size() && (std::isdigit(src[i]) || src[i]=='.')) i++;
            tokens.push_back({TokenType::Number, src.substr(start, i-start)});
            continue;
        }

        if (c == '"') {
            size_t start = ++i;
            while (i < src.size() && src[i] != '"') {
                if (src[i] == '\\' && i+1 < src.size()) i++;
                i++;
            }
            tokens.push_back({TokenType::String, src.substr(start, i-start)});
            i++; 
            continue;
        }

        if (c == '\'') {
            size_t start = ++i;
            if (i < src.size() && src[i] == '\\') i+=2;
            else i++;
            tokens.push_back({TokenType::Char, src.substr(start, i-start)});
            if (i < src.size() && src[i]=='\'') i++;
            continue;
        }

        if (is_operator(c)) {
            std::string op(1,c);
            if (i+1 < src.size() && is_operator(src[i+1]))
                op += src[++i];
            tokens.push_back({TokenType::Operator, op});
            i++;
            continue;
        }

        if (is_punct(c)) {
            tokens.push_back({TokenType::Punct, std::string(1,c)});
            i++;
            continue;
        }

        tokens.push_back({TokenType::Unknown, std::string(1,c)});
        i++;
    }

    return tokens;
}

void fdata_output(const std::string& filename) {
    std::ifstream f(filename);
    if (!f) {
        std::lock_guard<std::mutex> lock(mcout);
        std::cerr << "ERROR -> Cannot open \"" << filename << "\"\n";
        return;
    }

    std::stringstream buffer;
    buffer << f.rdbuf();
    std::string str = buffer.str();

    auto tokens = lex(str);

    std::lock_guard<std::mutex> lock(mcout);

    std::cout << "solve: (" << filename << ") >>>\n";

    for (auto& t : tokens) {
        std::string type;
        switch(t.type) {
            case TokenType::Identifier: type="IDENT"; break;
            case TokenType::Number:     type="NUMBER"; break;
            case TokenType::String:     type="STRING"; break;
            case TokenType::Char:       type="CHAR"; break;
            case TokenType::Operator:   type="OP"; break;
            case TokenType::Punct:      type="PUNCT"; break;
            case TokenType::NewLine:    type="NEWLINE"; break;
            default: type="UNKNOWN";
        }

        std::cout << "|\t[" << type << "] " << t.value << "\n";
    }

    std::cout << "<<<\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "ERROR -> No file passed\n";
        return 1;
    }

    std::vector<std::string> fv;
    std::vector<std::thread> threads;

    for (int i = 1; i < argc; i++) {
        fv.push_back(argv[i]);
    }

    for (const auto& f : fv) {
        threads.emplace_back(fdata_output, f);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}

