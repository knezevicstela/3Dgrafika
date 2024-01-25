#include <iostream>
#include <algorithm>
#include <cmath>
#include "tgaimage.h"
using namespace std;

// compile with:
// g++ rasterizer.cpp tgaimage.cpp -o out.exe

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(y, x, color);    
}

float line(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

void line_naive(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t = 0; t < 1; t += 0.01)
    {
        int x = x0 * (1.0f - t) + x1 * t;
        int y = x0 * (1.0f - t) + y1 * t;
        set_color(x, y, image, color);
    }
}

void line_midpoint(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color, bool invert)
{
    int y = y0;
    int d = line(x0, y0, x1, y1, x0 + 1, y0 + 0.5);
    
    int dx = (x1 - x0);
    int dy = (y1 - y0);
    int increment = 1;
    
    if (dy < 0)
    {
        // pravac ide od gore prema dolje
        dy = -dy;
        increment = -increment;
    }
    
    for (int x = x0; x <= x1; ++x)
    {
        if (invert)
        {
            set_color(x, y, image, color);
        }
        else
        {
            set_color(y, x, image, color);       
        }
        
        if (d < 0)
        {
            y = y + increment;
            d = d + dx - dy;
        }
        else
        {
            d = d - dy;
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // 'transponiraj' duzinu ako je veci od 1
    bool invert = false;
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        swap(x0, y0);
        swap(x1, y1);
        invert = true;
    }
    
    // zamijeni tocke ako pravac ide s desna na lijevo
    if (x1 < x0)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    
    // nacrtaj duzinu
    line_midpoint(x0, y0, x1, y1, image, color, invert);
}

//baricentricne kooridnate
float baricentric(float x_a,float y_a,float x_c,float y_c,float x,float y){
    return((y_a - y_c) * x + (x_c - x_a) * y + x_a * y_c - x_c * y_a);
}

// 1.zad
void draw_triangle_2d(float x0, float y0, float x1, float y1, float x2, float y2, TGAImage &image, TGAColor color){
    
    if (y0>y1){
        swap(x0,x1); 
        swap(y0,y1);
    }
    if (y1>y2){
        swap(x1,x2); 
        swap(y1,y2);
    }

    float sl1 = (x1-x0)/static_cast<float>(y1-y0);
    float sl2 = (x1-x0)/static_cast<float>(y2-y0);
    float sl3 = (x1-x1)/static_cast<float>(y2-y1);

    for(float y=y0; y<=y2; y++){
        int pocX, krX;

        if (y<y1){
            pocX = static_cast<int>(x0+sl1*(y-y0));
            krX = static_cast<int>(x0+sl2*(y-y0));  
        }else{
            pocX = static_cast<int>(x1+sl3*(y-y1));
            krX = static_cast<int>(x0+sl2*(y-y0));
        }

        if (pocX>krX) swap(pocX, krX);

        for(int x=pocX; x<=krX; x++){
            set_color(y,x,image,color);
        }
    }
}



int f(float x0, float y0, float x1, float y1, float x, float y){
    return (y0 - y1)*x+(x1 - x0)*y+x0*y1-x1*y0;
}

void draw_triangle_2d_gouraud (float x0, float y0, float x1, float y1, float x2, float y2 ,TGAImage &image,TGAColor c0,TGAColor c1,TGAColor c2){
    float min_x = floor(min((min(x0,x1)), x2));
    float max_x = ceil(max((max(x0, x1)), x2));
    float min_y = floor(min((min(y0,y1)), y2));
    float max_y = ceil(max((max(y0, y1)), y2));

    for(float y = min_y; y<=max_y; y++){
        for(float x = min_x; x <= max_x; x++){
            float alpha = baricentric(x1, y1, x2, y2, x, y) / baricentric(x1, y1, x2, y2, x0, y0);
            float beta = baricentric(x2, y2, x0, y0, x, y) / baricentric(x2, y2, x0, y0, x1, y1);
            float gamma = baricentric(x0, y0, x1, y1, x, y) / baricentric(x0, y0, x1, y1, x2, y2);

            if((alpha >= 0 && beta >= 0 && gamma >= 0) && (alpha <= 1 && beta <= 1 && gamma <= 1)){
                if((alpha > 0 || (baricentric(x1, y1, x2, y2, x0, y0) * baricentric(x1, y1, x2, y2, -1, -1)) >= 0) &&
                (beta > 0 || (baricentric(x2, y2, x0, y0, x1, y1) * baricentric(x2, y2, x0, y0, -1, -1)) >= 0) &&
                (gamma > 0 || (baricentric(x0, y0, x1, y1, x2, y2) * baricentric(x0, y0, x1, y1, -1, -1)) >= 0)){
                    TGAColor c;
                    c.r = alpha*c0.r + beta*c1.r + gamma*c2.r;
                    c.g = alpha*c0.g + beta*c1.g + gamma*c2.g;
                    c.b = alpha*c0.b + beta*c1.b + gamma*c2.b;

                    set_color(x,y,image,c);
                }
            }
        }
    }
}

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);


int main()
{

    //1i2zad
    TGAImage image(width, height, TGAImage::RGB);
    draw_triangle_2d(250, 250, 150, 140, 50, 100, image, red);
    draw_triangle_2d(100, 100, 200, 180, 100, 150, image, green);
    draw_triangle_2d_gouraud(300, 300, 450, 250, 500, 10, image, red, green, blue);

    image.flip_vertically();
    image.write_tga_file("zad1_2.tga");

}