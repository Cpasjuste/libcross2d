//
// Created by cpasjuste on 22/06/18.
//

#ifndef C2D_UTILITY_H
#define C2D_UTILITY_H

namespace c2d {

    class Utility {

    public:

        static char *removeExt(const char *str, char sep);

        static bool startWith(const std::string &value, const std::string &str);

        static bool endsWith(const std::string &value, const std::string &ending);

        static std::string removeLastSlash(const std::string &string);
    };

}

#endif //C2D_UTILITY_H
