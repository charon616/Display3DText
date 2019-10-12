#include "dShader.hpp"

using namespace std;

GLuint dShader::createShader()
{
    //バーテックスシェーダのコンパイル
    GLuint vShaderId = glCreateShader(GL_VERTEX_SHADER);
    string vertexShader = R"#(
    attribute vec3 position;
    void main(void){
        gl_Position = vec4(position, 1.0);
    }
    )#";
    const char* vs = vertexShader.c_str();
    glShaderSource(vShaderId, 1, &vs, NULL);
    glCompileShader(vShaderId);
    
    //フラグメントシェーダのコンパイル
    GLuint fShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    string fragmentShader = R"#(
    void main(void){
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    )#";
    const char* fs = fragmentShader.c_str();
    glShaderSource(fShaderId, 1, &fs, NULL);
    glCompileShader(fShaderId);
    
    //プログラムオブジェクトの作成
    GLuint programId = glCreateProgram();
    glAttachShader(programId,vShaderId);
    glAttachShader(programId,fShaderId);
    
    // リンク
    glLinkProgram(programId);
    
    glUseProgram(programId);
    
    return programId;
}
