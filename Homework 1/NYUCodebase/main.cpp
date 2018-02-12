#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
SDL_Window* displayWindow;
GLuint LoadTexture(const char *filePath) {
    int w,h,comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    if(image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        //assert(false);
    }
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(image);
    return retTexture;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 360);
    ShaderProgram program1;
    ShaderProgram program2;
    ShaderProgram program3;
    ShaderProgram program4;
    
    program4.Load(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    GLuint emojiTexture = LoadTexture(RESOURCE_FOLDER "emoji.jpg");
    Matrix projectionMatrix4;
    Matrix modelMatrix4;
    Matrix viewMatrix4;
    
    program3.Load(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    GLuint guitarTexture = LoadTexture(RESOURCE_FOLDER "guitar.png");
    Matrix projectionMatrix3;
    Matrix modelMatrix3;
    Matrix viewMatrix3;
    
    
    program1.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    Matrix projectionMatrix1;
    Matrix modelMatrix1;
    Matrix viewMatrix1;
    
    GLuint mariachiTexture = LoadTexture(RESOURCE_FOLDER "mariachi.jpg");
    
    program2.Load(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    Matrix projectionMatrix2;
    Matrix modelMatrix2;
    Matrix viewMatrix2;
    
    projectionMatrix1.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    glUseProgram(program1.programID);
    
    projectionMatrix2.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    glUseProgram(program2.programID);
    
    
    
    modelMatrix1.Translate(2.0f, 0.0f, 0.0f);
    program1.SetModelMatrix(modelMatrix1);
   
    
    modelMatrix2.Translate(-2.0f, 0.0f, 0.0f);
    program1.SetModelMatrix(modelMatrix2);
    
    
    modelMatrix3.Translate(0.0f,-2.0f,0.0f);
    modelMatrix4.Translate(0.0f,2.0f,0.0f);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float lastFrameTicks = 0.0f;
    
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
           
        }
        glClear(GL_COLOR_BUFFER_BIT);
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        
        program1.SetModelMatrix(modelMatrix1);
        program1.SetProjectionMatrix(projectionMatrix1);
        program1.SetViewMatrix(viewMatrix1);
        float vertices[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f};
        
        
        glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program1.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(program1.positionAttribute);
        
        //std::cout<<pow(alternator,3);
        modelMatrix1.Translate((cos(3.1415826f/2 * elapsed))*2.0f/((10/elapsed)), 0.0f, 0.0f);
        
        program1.SetModelMatrix(modelMatrix2);
        program1.SetProjectionMatrix(projectionMatrix2);
        program1.SetViewMatrix(viewMatrix2);
        
        
       
        glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program1.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(program1.positionAttribute);
        SDL_GL_SwapWindow(displayWindow);
        modelMatrix1.Rotate(elapsed*(45.0f * (3.1415926f / 180.0f)));
        
        program3.SetModelMatrix(modelMatrix3);
        program3.SetProjectionMatrix(projectionMatrix3);
        program3.SetViewMatrix(viewMatrix3);
        
        glBindTexture(GL_TEXTURE_2D, mariachiTexture);
        
        float vertices3[] = {-1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1};
        
        glVertexAttribPointer(program3.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
        glEnableVertexAttribArray(program3.positionAttribute);
        float texCoords1[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program3.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords1);
        glEnableVertexAttribArray(program3.texCoordAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program3.positionAttribute);
        glDisableVertexAttribArray(program3.texCoordAttribute);
        
        program2.SetModelMatrix(modelMatrix4);
        program2.SetProjectionMatrix(projectionMatrix4);
        program2.SetViewMatrix(viewMatrix4);
        
        glBindTexture(GL_TEXTURE_2D, emojiTexture);
        
        float vertices1[] = {-1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1};
       
        glVertexAttribPointer(program2.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
        glEnableVertexAttribArray(program2.positionAttribute);
        float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program2.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program2.texCoordAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program2.positionAttribute);
        glDisableVertexAttribArray(program2.texCoordAttribute);
        
        glBindTexture(GL_TEXTURE_2D, guitarTexture);
        
        float vertices4[] = {-1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1};
        
        glVertexAttribPointer(program4.positionAttribute, 2, GL_FLOAT, false, 0, vertices4);
        glEnableVertexAttribArray(program4.positionAttribute);
        float texCoords2[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program4.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
        glEnableVertexAttribArray(program4.texCoordAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program4.positionAttribute);
        glDisableVertexAttribArray(program4.texCoordAttribute);

        
        
        lastFrameTicks = ticks;
        
    }
    
    SDL_Quit();
    return 0;
}
