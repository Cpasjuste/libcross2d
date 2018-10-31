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

            Option(const std::string &name, const std::vector<std::string> &array, int id = 0);

            Option(const std::string &name, const std::vector<int> &array, int id = 0);

            Option(const std::string &name, const std::vector<float> &array, int id = 0);

            std::string getName() const;

            void setName(const std::string &name);

            ///
            /// \return the option value as string (doing float/int conversion if needed)
            std::string getString() const;

            std::vector<std::string> getStringArray() const;

            void setString(const std::string &value);

            void setStringArray(const std::vector<std::string> &array);

            int getInteger();

            void setInteger(int value);

            void setIntegerArray(const std::vector<int> &array);

            float getFloat();

            void setFloat(float value);

            void setFloatArray(const std::vector<float> &array);

            std::string getComment() const;

            void setComment(const std::string &info);

            int getId() const;

            void setId(int id);

            Type getType() const;

            void setType(Option::Type type);

            bool isSavable() const;

            void setSavable(bool savable);

            void *getUserData();

            void setUserData(void *data);

        private:

            std::string name;
            std::string info;
            Type type;
            int id = 0;

            std::vector<std::string> string_array;
            std::vector<int> integer_array;
            std::vector<float> float_array;

            bool savable = true;
            void *user_data;
        };
    }
}

#endif //C2D_CONFIG_OPTION_H
