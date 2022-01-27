//
// Created by cpasjuste on 27/01/2022.
//

#if defined(__PS4__) && defined(PS4_DUMP_SHADERS)

#include <cstdio>
#include <string>
#include <cstdarg>
#include <orbis/Pigletv2VSH.h>

#include "ps4_shader_compiler.h"
#include "ps4_sys.h"

using namespace c2d;

extern "C" void glPigletGetShaderBinarySCE(
        GLuint program, GLsizei bufSize, GLsizei *length,
        GLenum *binaryFormat, void *binary);

PS4ShaderCompiler::PS4ShaderCompiler(const std::string &dumpPath) {
    dump_path = dumpPath;
    header_path = dump_path + "/" + header_name;

    FILE *fp = fopen(header_path.c_str(), "w");
    fprintf(fp, "#ifndef C2D_PS4_SHADERS_H\n#define C2D_PS4_SHADERS_H\n\n");
    fclose(fp);
}

PS4ShaderCompiler::~PS4ShaderCompiler() {
    FILE *fp = fopen(header_path.c_str(), "a");
    fprintf(fp, "\n#endif //C2D_PS4_SHADERS_H\n");
    fclose(fp);
}

void PS4ShaderCompiler::dump(int id, const std::string &name, const std::string &type) {
    int size;
    GLenum format = 0;
    int need_comma = 0, file_size;
    std::string path = dump_path + "/" + name + "_" + type + ".cpp";
    char *buf = (char *) malloc(0x5000);
    memset(buf, 0, 0x5000);

    // dump shader
    glPigletGetShaderBinarySCE((GLuint) id, 0x5000, &size, &format, buf);

    if (size > 0) {
        // add shader to cpp file
        PS4Sys::print("PS4ShaderCompiler::dumpShaderBinary: "
                      "dumping shader to: %s, (size: %i)\n", path.c_str(), size);

        file_size = size;
        std::string array_name = "c2d_" + name + "_" + type;
        std::replace(array_name.begin(), array_name.end(), '-', '_');
        array_name.erase(std::remove(array_name.begin(), array_name.end(), '.'), array_name.end());
        FILE *fp = fopen(path.c_str(), "w");

        fprintf(fp, "#include \"ps4_shaders.h\"\n\nconst char %s[%i] = {", array_name.c_str(), size);
        for (int i = 0; i < file_size; ++i) {
            if (need_comma)
                fprintf(fp, ", ");
            else
                need_comma = 1;
            if ((i % 11) == 0)
                fprintf(fp, "\n\t");
            fprintf(fp, "0x%.2x", buf[i] & 0xff);
        }
        fprintf(fp, "\n};\n\n");
        fclose(fp);

        // add shader to header file
        fp = fopen(header_path.c_str(), "a");
        fprintf(fp, "extern const char %s[];\n", array_name.c_str());
        fprintf(fp, "const int %s_length = %i;\n", array_name.c_str(), size);
        fclose(fp);
    }

    free(buf);
}

#endif
