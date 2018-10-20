//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_CONFIG_OPTION_H
#define C2D_CONFIG_OPTION_H

#include <string>
#include <vector>

namespace c2d {

    namespace config {

        class Option {

        public:

            enum class Type {
                Integer,
                String,
                Float
            };

            Option(const std::string &name, const std::string &value, int id = 0);

            Option(const std::string &name, int value, int id = 0);

            Option(const std::string &name, float value, int id = 0);

            std::string getName() const;

            void setName(const std::string &name);

            ///
            /// \return the option value as string (doing float/int to string if needed)
            std::string getString() const;

            void setString(const std::string &value);

            int getInteger();

            void setInteger(int value);

            float getFloat();

            void setFloat(float value);

            std::string getComment() const;

            void setComment(const std::string &info);

            int getId() const;

            void setId(int id);

            Type getType() const;

            void setType(Option::Type type);

        private:

            std::string name;
            std::string info;
            std::string value_string;
            int value_integer;
            float value_float;
            Type type;
            int id = 0;
        };
    }
}

#endif //C2D_CONFIG_OPTION_H
