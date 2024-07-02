#ifndef PATTERN_MATCHER_H
#define PATTERN_MATCHER_H

#include <string>

bool contains_any(const std::string& text, const std::string& chars);
bool starts_with(const std::string& text, const std::string& prefix);
bool ends_with(const std::string& text, const std::string& suffix);
bool match_pattern_rec(const std::string& text, const std::string& pattern);
bool match_pattern(const std::string& text, const std::string& pattern);

#endif // PATTERN_MATCHER_H
