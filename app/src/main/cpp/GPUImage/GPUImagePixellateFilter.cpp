/**
 * Created by lvHiei on 17-4-12.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */

#include "GPUImagePixellateFilter.h"


// 片元着色器
const char _pixellate_fragment_shader[]=
"precision mediump float;\n"
"varying vec2 textureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"uniform float fractionalWidthOfPixel;\n"
"uniform float aspectRatio;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 sampleDivisor = vec2(fractionalWidthOfPixel, fractionalWidthOfPixel / aspectRatio);\n"
"\n"
"    vec2 samplePos = textureCoordinate - mod(textureCoordinate, sampleDivisor) + 0.5 * sampleDivisor;\n"
"    gl_FragColor = texture2D(inputImageTexture, samplePos );\n"
"}"
;


GPUImagePixellateFilter::GPUImagePixellateFilter()
    : GPUImageFilter(_pixellate_fragment_shader)
{
    m_fAsepctRatio = 720.0f/1280.0f;

    m_fFractionalWidthOfAPixel = 0.05f;

    m_iAsepectRatioUniformLocation = -1;
    m_iFragUniformLocation = -1;
}


GPUImagePixellateFilter::GPUImagePixellateFilter(const char *fragment)
        : GPUImageFilter(fragment)
{
    m_fAsepctRatio = 720.0f/1280.0f;

    m_fFractionalWidthOfAPixel = 0.05f;

    m_iAsepectRatioUniformLocation = -1;
    m_iFragUniformLocation = -1;
}


GPUImagePixellateFilter::~GPUImagePixellateFilter()
{

}

void GPUImagePixellateFilter::setAspectRatio(float aspectRatio)
{
    m_fAsepctRatio = aspectRatio;
}

void GPUImagePixellateFilter::setFractionalWidthOfAPixel(float fraction)
{
    GLfloat min = 1.0 / 1280.0;
    if(fraction < min){
        fraction = min;
    }

    m_fFractionalWidthOfAPixel = fraction;
}

bool GPUImagePixellateFilter::createProgramExtra()
{
    m_iFragUniformLocation = glGetUniformLocation(m_uProgram, "fractionalWidthOfPixel");
    m_iAsepectRatioUniformLocation = glGetUniformLocation(m_uProgram, "aspectRatio");
    return GPUImageFilter::createProgramExtra();
}

bool GPUImagePixellateFilter::beforeDrawExtra()
{
    glUniform1f(m_iFragUniformLocation, m_fFractionalWidthOfAPixel);
    glUniform1f(m_iAsepectRatioUniformLocation, m_fAsepctRatio);
    return GPUImageFilter::beforeDrawExtra();
}


