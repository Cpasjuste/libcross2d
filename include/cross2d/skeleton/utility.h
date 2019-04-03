//
// Created by cpasjuste on 22/06/18.
//

#ifndef C2D_UTILITY_H
#define C2D_UTILITY_H

#include <string>
#include <vector>

namespace c2d {

    class Utility {

    public:

        static std::string toLower(const std::string &str);

        static std::string toUpper(const std::string &str);

        static std::vector<std::string> split(const std::string &str, const std::string &token);

        static bool startWith(const std::string &str, const std::string &value, bool sensitive = true);

        static bool endsWith(const std::string &value, const std::string &ending, bool sensitive = true);

        static std::string removeLastSlash(const std::string &string);

        static std::string removeExt(const std::string &str);
    };

}

#endif //C2D_UTILITY_H
