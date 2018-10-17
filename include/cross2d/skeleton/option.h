//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_OPTION_H
#define C2D_OPTION_H

#include <string>
#include <vector>

namespace c2d {

    class Option {

    public:

        Option(const std::string &name,
               const std::vector<std::string> &values,
               int index = 0, int id = 0);

        std::string getName();

        void setName(const std::string &name);

        std::string getInfo();

        void setInfo(const std::string &info);

        std::string getValue();

        std::vector<std::string> getValues();

        void setValues(const std::vector<std::string> &values);

        int getIndex();

        void setIndex(int index);

        int getId();

        void setId(int id);

    private:

        std::string name;
        std::string info;
        std::vector<std::string> values;
        int index = 0;
        int id = 0;
    };
}

#endif //C2D_OPTION_H
