//
// Created by cpasjuste on 27/01/2022.
//

#ifndef C2D_PS4_SHADER_COMPILER_H
#define C2D_PS4_SHADER_COMPILER_H

#if defined(__PS4__) && defined(PS4_DUMP_SHADERS)

namespace c2d {
    class PS4ShaderCompiler {
    public:
        PS4ShaderCompiler(const std::string &dumpPath);

        ~PS4ShaderCompiler();

        void dump(int id, const std::string &name, const std::string &type);

    private:
        std::string dump_path;
        std::string header_path;
        const std::string header_name = "ps4_shaders.h";
    };
}

#endif //
#endif //C2D_PS4_SHADER_COMPILER_H
