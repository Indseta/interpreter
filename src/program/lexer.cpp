#include <program/lexer.h>

const std::vector<std::string> Lexer::keywords = {
    "let",
    "var",
    "const",
    "function",
    "return",
    "true",
    "false",
    "if",
    "else",
    "for",
    "while",
    "break",
    "continue",
};

const std::vector<std::string> Lexer::operators = {
    "=",
    "!",
    "+",
    "-",
    "*",
    "/",
    "%",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "==",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
};

const std::vector<std::string> Lexer::punctuators = {
    ";",
    ".",
    ",",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]",
};

Lexer::Lexer(const Source &source) {
    lex(source.get());
}

void Lexer::lex(const std::string &raw) {
    Token ct;

    for (int i = 0; i < raw.length(); ++i) {
        const char c = raw[i];

        // Skip whitespace
        if (std::isspace(c)) {
            continue;
        }

        // Reset token value
        ct.category = UNKNOWN;
        ct.value = "";

        // Keyword, identifier & boolean literal
        if (std::isalpha(c)) {
            ct.value += c;
            while (i + 1 < raw.length() && (std::isalpha(raw[i + 1]) || std::isdigit(raw[i + 1]))) {
                ct.value += raw[++i];
            }
            if (ct.value == "false" || ct.value == "true") {
                ct.category = BOOLEAN_LITERAL;
            } else {
                ct.category = is_keyword(ct.value) ? KEYWORD : IDENTIFIER;
            }
            tokens.push_back(ct);
            continue;
        }

        // Integer & float literal
        if (std::isdigit(c)) {
            ct.value += c;
            while (i + 1 < raw.length() && std::isdigit(raw[i + 1])) {
                ct.value += raw[++i];
            }
            ct.category = INTEGER_LITERAL;
            if (raw[i + 1] == '.' && std::isdigit(raw[i + 2])) {
                ct.value += raw[++i];
                while (i + 1 < raw.length() && std::isdigit(raw[i + 1])) {
                    ct.value += raw[++i];
                }
                ct.category = FLOAT_LITERAL;
            }
            tokens.push_back(ct);
            continue;
        }

        // Operator
        if (is_operator(std::string(1, c))) {
            ct.value += c;
            while (i + 1 < raw.length() && is_operator(ct.value + raw[i + 1])) {
                ct.value += raw[++i];
            }
            ct.category = OPERATOR;
            tokens.push_back(ct);
            continue;
        }

        // Punctuator
        if (is_punctuator(std::string(1, c))) {
            ct.value += c;
            while (i + 1 < raw.length() && is_punctuator(ct.value + raw[i + 1])) {
                ct.value += raw[++i];
            }
            ct.category = PUNCTUATOR;
            tokens.push_back(ct);
            continue;
        }

        // String literal
        if (c == '"') {
            while (i + 1 < raw.length() && raw[i + 1] != '"') {
                ct.value += raw[++i];
            }
            ct.category = STRING_LITERAL;
            tokens.push_back(ct);
            ++i;
            continue;
        }

        // Unkown token
        std::string msg = "Unkown token: ";
        msg += ct.value;
        throw std::runtime_error(msg);
    }
}

const bool Lexer::is_keyword(const std::string &value) const {
    for (const auto &e : keywords) {
        if (value == e) {
            return true;
        }
    }
    return false;
}

const bool Lexer::is_operator(const std::string &value) const {
    for (const auto& e : operators) {
        if (value == e) {
            return true;
        }
    }
    return false;
}

const bool Lexer::is_punctuator(const std::string &value) const {
    for (const auto& e : punctuators) {
        if (value == e) {
            return true;
        }
    }
    return false;
}

const std::vector<Lexer::Token>& Lexer::get() const {
    return tokens;
}

void Lexer::Token::log() const {
    std::cout << "(";
    switch (category) {
        case PUNCTUATOR: std::cout << "punctuator"; break;
        case KEYWORD: std::cout << "keyword"; break;
        case IDENTIFIER: std::cout << "identifier"; break;
        case OPERATOR: std::cout << "operator"; break;
        case INTEGER_LITERAL: std::cout << "integer_literal"; break;
        case FLOAT_LITERAL: std::cout << "float_literal"; break;
        case BOOLEAN_LITERAL: std::cout << "boolean_literal"; break;
        case STRING_LITERAL: std::cout << "string_literal"; break;
        case LINE_COMMENT: std::cout << "line_comment"; break;
        case BLOCK_COMMENT: std::cout << "block_comment"; break;
        default: std::cout << "unknown"; break;
    }
    std::cout << "): '" << value << "'";
}