//
// Created by cpasjuste on 27/01/17.
//

#ifndef C2D_SHADERLIST_H
#define C2D_SHADERLIST_H

#include <string>
#include <vector>

namespace c2d {

    class ShaderList {

    public:

        struct Shader {

            std::string name;
            void *data = nullptr;

            Shader(const std::string &n, void *d) {
                name = n;
                data = d;
            }
        };

        ShaderList() {
            list.emplace_back("NONE", nullptr);
        }

        virtual ~ShaderList() {
            list.clear();
        }

        virtual void add(const std::string &n, void *d) {
            list.emplace_back(n, d);
        }

        virtual Shader *get(int index) {
            if ((size_t) index >= list.size()) {
                return nullptr;
            }
            return &list[index];
        }

        virtual Shader *get(const std::string &name) {
            for (auto &i: list) {
                if (i.name == name) {
                    return &i;
                }
            }
            return nullptr;
        }

        virtual std::vector<std::string> getNames() {
            std::vector<std::string> names;
            for (auto &i: list) {
                names.push_back(i.name);
            }
            return names;
        }

        virtual int getCount() {
            return (int) list.size();
        }

    private:
        std::vector<Shader> list;
    };
}

#endif //C2D_SHADERLIST_H
