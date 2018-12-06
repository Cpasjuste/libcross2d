//
// Created by cpasjuste on 22/06/18.
//

#ifndef C2D_UTILITY_H
#define C2D_UTILITY_H

#include <string>

namespace c2d {

    class Utility {

    public:

        static std::string toLower(const std::string &str);

        static bool startWith(const std::string &str, const std::string &value, bool sensitive = true);

        static bool endsWith(const std::string &value, const std::string &ending, bool sensitive = true);

        static std::string removeLastSlash(const std::string &string);

        static std::string removeExt(const std::string &str);
    };

}

#endif //C2D_UTILITY_H
