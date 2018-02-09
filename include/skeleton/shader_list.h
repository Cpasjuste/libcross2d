//
// Created by cpasjuste on 27/01/17.
//

#ifndef CROSS2D_SHADERS_H
#define CROSS2D_SHADERS_H

#include <string>
#include <vector>

namespace c2d {

    class ShaderList {

    public:

        struct Shader {

            std::string name;
            void *data = NULL;

            Shader(const std::string n, void *d) {
                name = n;
                data = d;
            }
        };

        ShaderList(const std::string &shadersPath) {
            list.push_back(Shader("NONE", NULL));
        }

        virtual ~ShaderList() {
            list.clear();
        }

        virtual void add(std::string n, void *d) {
            list.push_back(Shader(n, d));
        }

        virtual Shader *get(int index) {
            if ((size_t) index >= list.size()) {
                return NULL;
            }
            return &list[index];
        }

        virtual Shader *get(const std::string &name) {
            for (size_t i = 0; i < list.size(); i++) {
                if (list[i].name == name) {
                    return &list[i];
                }
            }
            return NULL;
        }

        virtual std::vector<std::string> getNames() {
            std::vector<std::string> names;
            for (size_t i = 0; i < list.size(); i++) {
                names.push_back(list[i].name);
            }
            return names;
        }

        virtual int getCount() {
            return list.size();
        }

    private:
        std::vector<Shader> list;
    };
}

#endif //CROSS2D_SHADERS_H
