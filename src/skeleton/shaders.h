//
// Created by cpasjuste on 27/01/17.
//

#ifndef PFBA_SHADERS_H
#define PFBA_SHADERS_H

#include <string>
#include <vector>

class Shaders {

public:

    struct Shader {
        std::string name;
        void *data = NULL;

        Shader(const std::string n, void *d) {
            name = n;
            data = d;
        }
    };

    Shaders(const std::string &shadersPath) {
        list.push_back(Shader("NONE", NULL));
    }

    virtual ~Shaders() {
        list.clear();
    }

    virtual void Add(std::string n, void *d) {
        list.push_back(Shader(n, d));
    }

    virtual Shader *Get() {
        if (current >= list.size()) {
            return NULL;
        }
        return &list[current];
    }

    virtual Shader *Get(int index) {
        if (index >= list.size()) {
            return NULL;
        }
        return &list[index];
    }

    virtual std::vector<std::string> GetNames() {
        std::vector<std::string> names;
        for (int i = 0; i < list.size(); i++) {
            names.push_back(list[i].name);
        }
        return names;
    }

    virtual int Count() {
        return list.size();
    }

    int current = 0;

private:
    std::vector<Shader> list;
};


#endif //PFBA_SHADERS_H
