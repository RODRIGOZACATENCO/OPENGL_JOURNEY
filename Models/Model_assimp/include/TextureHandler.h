#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H
#include <string>

class Texture2d {
public:
    unsigned int textureID;
    unsigned int activeTexture;
    int width, height,nrchannels;
    Texture2d(std::string texturePath,unsigned int aTexture) {
        activeTexture = aTexture;
        unsigned  char *data=stbi_load(texturePath.c_str(),&width,&height,&nrchannels,0);
        glGenTextures(1,&textureID);
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLenum format=GL_RED;
        if (nrchannels == 1)
            format = GL_RED;
        else if (nrchannels == 3)
            format = GL_RGB;
        else if (nrchannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    void use() {
        glActiveTexture(GL_TEXTURE0 + activeTexture);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    void setWrapMode(GLint wrapS,GLint WrapT)const {
        glActiveTexture(GL_TEXTURE0 + activeTexture);
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrapS);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,WrapT);
        glBindTexture(GL_TEXTURE_2D,0);
    }
 };




#endif TEXTUREHANDLER_H