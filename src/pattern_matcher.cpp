#include "pattern_matcher.h"

bool contains_any(const std::string& text, const std::string& chars) {
    return text.find_first_of(chars) != std::string::npos;
}

bool starts_with(const std::string& text, const std::string& prefix) {
    return text.find(prefix) == 0;
}

bool ends_with(const std::string& text, const std::string& suffix) {
    return text.rfind(suffix) == text.size() - suffix.size();
}

bool match_alternation(const std::string& text, const std::string& pattern) {
    auto pos = pattern.find('|');
    if (pos != std::string::npos) {
        return match_pattern_rec(text, pattern.substr(0, pos)) ||
               match_pattern_rec(text, pattern.substr(pos + 1));
    }
    return match_pattern_rec(text, pattern);
}

bool match_pattern_rec(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return true;
    if (text.empty()) return false;

    if (pattern.substr(0, 2) == "\\d") {
        if (isdigit(text[0])) {
            return match_pattern_rec(text.substr(1), pattern.substr(2));
        } else {
            return match_pattern_rec(text.substr(1), pattern);
        }
    }

    if (pattern.substr(0, 2) == "\\w") {
        if (isalnum(text[0])) {
            return match_pattern_rec(text.substr(1), pattern.substr(2));
        } else {
            return match_pattern_rec(text.substr(1), pattern);
        }
    }

    if (pattern[0] == '[') {
        auto closing_bracket = pattern.find(']');
        bool is_negated = pattern[1] == '^';
        if (is_negated) {
            if (!contains_any(text, pattern.substr(2, closing_bracket - 2))) {
                return match_pattern_rec(text.substr(1), pattern.substr(closing_bracket + 1));
            } else {
                return false;
            }
        }
        if (contains_any(text, pattern.substr(1, closing_bracket - 1))) {
            return match_pattern_rec(text.substr(1), pattern.substr(closing_bracket + 1));
        } else {
            return false;
        }
    }

    if (pattern.size() > 1 && pattern[1] == '+') {
        if (pattern[0] == text[0]) {
            size_t i = 1;
            while (i < text.size() && text[i] == pattern[0]) {
                ++i;
            }
            return match_pattern_rec(text.substr(i), pattern.substr(2));
        } else {
            return false;
        }
    }

    if (pattern.size() > 1 && pattern[1] == '?') {
        if (pattern[0] == text[0]) {
            return match_pattern_rec(text.substr(1), pattern.substr(2)) ||
                   match_pattern_rec(text, pattern.substr(2));
        } else {
            return match_pattern_rec(text, pattern.substr(2));
        }
    }


    if (pattern[0] == '.') {
        return match_pattern_rec(text.substr(1), pattern.substr(1));
    }


    if (pattern[0] == '(') {
        auto closing_paren = pattern.find(')');
        return match_alternation(text, pattern.substr(1, closing_paren - 1)) &&
               match_pattern_rec(text.substr(1), pattern.substr(closing_paren + 1));
    }

    if (pattern[0] == text[0]) {
        return match_pattern_rec(text.substr(1), pattern.substr(1));
    } else {
        return match_pattern_rec(text.substr(1), pattern);
    }
}

bool match_pattern(const std::string& text, const std::string& pattern) {
    if (pattern[0] == '^') {
        return starts_with(text, pattern.substr(1));
    } else if (pattern[pattern.size() - 1] == '$') {
        return ends_with(text, pattern.substr(0, pattern.size() - 1));
    }

    std::string remaining_text = text;
    do {
        if (match_pattern_rec(remaining_text, pattern)) {
            return true;
        }
        remaining_text = remaining_text.substr(1);
    } while (!remaining_text.empty());
    return false;
}
