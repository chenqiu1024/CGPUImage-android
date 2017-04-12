/**
 * Created by lvHiei on 17-4-12.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */

#include "GPUImagePinchDistortionFilter.h"


// 片元着色器
const char _pinchDistortion_fragment_shader[]=
"precision mediump float;\n"
"varying vec2 textureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"uniform float aspectRatio;\n"
"uniform vec2 center;\n"
"uniform float radius;\n"
"uniform float scale;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 textureCoordinateToUse = vec2(textureCoordinate.x, (textureCoordinate.y * aspectRatio + 0.5 - 0.5 * aspectRatio));\n"
"    float dist = distance(center, textureCoordinateToUse);\n"
"    textureCoordinateToUse = textureCoordinate;\n"
"\n"
"    if (dist < radius)\n"
"    {\n"
"        textureCoordinateToUse -= center;\n"
"        float percent = 1.0 + ((0.5 - dist) / 0.5) * scale;\n"
"        textureCoordinateToUse = textureCoordinateToUse * percent;\n"
"        textureCoordinateToUse += center;\n"
"\n"
"        gl_FragColor = texture2D(inputImageTexture, textureCoordinateToUse );\n"
"    }\n"
"    else\n"
"    {\n"
"        gl_FragColor = texture2D(inputImageTexture, textureCoordinate );\n"
"    }\n"
"}"
;


GPUImagePinchDistortionFilter::GPUImagePinchDistortionFilter()
    : GPUImageFilter(_pinchDistortion_fragment_shader)
{
    m_fRadius = 1.0f;
    m_fScale = 0.5f;

    m_pCenter[0] = 0.5f;
    m_pCenter[1] = 0.5f;

//    m_fAspectRatio = 1280.0f / 720.0;
    m_fAspectRatio = 720.0f / 1280.0;

    m_iCenterUnifornLocation = -1;
    m_iRadiusUnifornLocation = -1;
    m_iScaleUnifornLocation = -1;
}

GPUImagePinchDistortionFilter::~GPUImagePinchDistortionFilter()
{

}

void GPUImagePinchDistortionFilter::setCenter(float x, float y)
{
    m_pCenter[0] = x;
    m_pCenter[1] = y;
}

void GPUImagePinchDistortionFilter::setRadius(float radius)
{
    m_fRadius = radius;
}

void GPUImagePinchDistortionFilter::setScale(float scale)
{
    m_fScale = scale;
}

void GPUImagePinchDistortionFilter::setRadius(int percent)
{
    if(percent < 0){
        percent = 0;
    }

    if(percent > 100){
        percent = 100;
    }

    float incremental = (2.0f - 0.0f) / 100;

    m_fRadius = 0.0f + incremental * percent;
}

void GPUImagePinchDistortionFilter::setScale(int percent)
{
    if(percent < 0){
        percent = 0;
    }

    if(percent > 100){
        percent = 100;
    }

    float incremental = (2.0f - (-2.0f)) / 100;

    m_fScale = 0.0f + incremental * percent;
}


void GPUImagePinchDistortionFilter::setAspectRatio(float aspectRatio)
{
    m_fAspectRatio = aspectRatio;
}


bool GPUImagePinchDistortionFilter::createProgramExtra()
{
    m_iCenterUnifornLocation = glGetUniformLocation(m_uProgram, "center");
    m_iRadiusUnifornLocation = glGetUniformLocation(m_uProgram, "radius");
    m_iScaleUnifornLocation = glGetUniformLocation(m_uProgram, "scale");
    m_iAspectUnifornLocation= glGetUniformLocation(m_uProgram, "aspectRatio");

    return GPUImageFilter::createProgramExtra();
}

bool GPUImagePinchDistortionFilter::beforeDrawExtra()
{
    glUniform1f(m_iAspectUnifornLocation, m_fAspectRatio);
    glUniform1f(m_iRadiusUnifornLocation, m_fRadius);
    glUniform1f(m_iScaleUnifornLocation, m_fScale);
    glUniform2fv(m_iCenterUnifornLocation, 1, m_pCenter);

    return GPUImageFilter::beforeDrawExtra();
}


