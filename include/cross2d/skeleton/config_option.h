//
// Created by cpasjuste on 17/10/18.
//

#ifndef C2D_CONFIG_OPTION_H
#define C2D_CONFIG_OPTION_H

#include <string>
#include <vector>

#include "cross2d/skeleton/sfml/Vector2.hpp"
#include "cross2d/skeleton/sfml/Rect.hpp"
#include "cross2d/skeleton/sfml/Color.hpp"

namespace c2d {

    namespace config {

        class Option {

        public:

            enum class Type {
                Integer,
                String,
                Float,
                Vector2f,
                FloatRect,
                Color,
                Choice
            };

            /// Constructors

            Option(const std::string &name, const std::string &value, int id = 0);

            Option(const std::string &name, int value, int id = 0);

            Option(const std::string &name, float value, int id = 0);

            Option(const std::string &name, const Vector2f &value, int id = 0);

            Option(const std::string &name, const FloatRect &value, int id = 0);

            Option(const std::string &name, const Color &value, int id = 0);

            Option(const std::string &name, const std::vector<std::string> &values, int index, int id = 0);

            /// Type::String

            std::string getString() const;

            void setString(const std::string &value);

            /// Type::Integer

            int getInteger();

            void setInteger(int value);

            /// Type::Float

            float getFloat();

            void setFloat(float value);

            /// Type::Vector2f

            Vector2f getVector2f();

            void setVector2f(const Vector2f &value);

            /// Type::FloatRect

            FloatRect getFloatRect();

            void setFloatRect(const FloatRect &value);

            /// Type::Color

            Color getColor();

            void setColor(const Color &color);

            /// Type::Choice

            int getChoiceIndex();

            void setChoicesIndex(int index);

            std::vector<std::string> getChoices();

            void setChoices(const std::vector<std::string> &values, int index);

            /// ...

            std::string getName() const;

            void setName(const std::string &name);

            int getId() const;

            void setId(int id);

            std::string getComment() const;

            void setComment(const std::string &info);

            Type getType() const;

            void setType(Option::Type type);

            bool isSavable() const;

            void setSavable(bool savable);

            void *getUserData();

            void setUserData(void *data);

        private:

            std::string string;
            int integer;
            FloatRect floatRect;
            std::vector<std::string> choices;
            int choices_index;

            std::string name;
            std::string info;
            Type type;
            int id = 0;
            bool savable = true;
            void *user_data = nullptr;
        };
    }
}

#endif //C2D_CONFIG_OPTION_H
