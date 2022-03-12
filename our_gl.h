#include "tgaimage.h"
#include "geometry.h"

void viewport(const int x, const int y, const int w, const int h);
void projection(const double coeff=0); // coeff = -1/c
void lookat(const vec3 eye, const vec3 center, const vec3 up);

struct IShader {
    static TGAColor sample2D(const TGAImage &img, vec2 &uvf) {
        return img.get(uvf[0] * img.width(), uvf[1] * img.height());
    }
    static TGAColor sampleBilinear2D(const TGAImage &img, vec2 &uvf) {
        auto ui = uvf[0] * img.width();
        auto vi = uvf[1] * img.height();
        double umin = floor(ui);
        double umax = ceil(ui);
        double vmin = floor(vi);
        double vmax = ceil(vi);

        auto c00 = img.get(umin, vmin),
             c11 = img.get(umax, vmax),
             c10 = img.get(umax, vmin),
             c01 = img.get(umin, vmax);

        double t1 = (ui - umin) / 1.0;
        double t2 = (vi - vmin) / 1.0;
        vec3 c0(c00[2]*(1-t1)+c10[2]*t1, c00[1]*(1-t1)+c10[1]*t1, c00[0]*(1-t1)+c10[0]*t1);
        vec3 c1(c01[2]*(1-t1)+c11[2]*t1, c01[1]*(1-t1)+c11[1]*t1, c01[0]*(1-t1)+c11[0]*t1);
        vec3 c = c0 * (1 - t2) + c1 * t2;

        return TGAColor(c[0], c[1], c[2], 255);
    }
    virtual bool fragment(const vec3 bar, TGAColor &color) = 0;
};

void triangle(const vec4 clip_verts[3], IShader &shader, TGAImage &image, std::vector<double> &zbuffer, std::vector<TGAColor>& colorbuffer);

