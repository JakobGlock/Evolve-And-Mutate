/* This code is included in the shaders that are created within my program */

#define PI     3.14159
#define TWO_PI 6.28319

float hash(float n)
{
    return fract(sin(n)*43758.5453);
}

/* Variations converted into code from the formulas which can be found here -
    http://www.flam3.com/flame_draves.pdf
*/

vec2 sinusoidal(vec2 p, float a, float t)
{
    return vec2(a * sin(p.x + t), a * sin(p.y + t));
}

vec2 hyperbolic(vec2 p, float amount, float t)
{
  float r = length(p) + 1.0-10;
  float theta = atan(p.x, p.y);
  float x = amount * sin(theta + t) / r;
  float y = amount * cos(theta + t) * r;
  return vec2(x, y);
}

vec2 julia(vec2 v, float amount, float t)
{
    float r = amount * sqrt(length(v));

    float theta = 0.0;

    float rand = 1024. * v.y + v.x;

    if(hash(rand) >= 0.5)
    {
        theta = 0.5 * atan(v.x, v.y) + PI;
    }
    else
    {
        theta = 0.5 * atan(v.x, v.y);
    }

    float x = r * cos(theta + t);
    float y = r * sin(theta + t);
    return vec2(x, y);
}

vec2 swirl(vec2 p, float a, float t)
{
    float r = length(p) * length(p);
    return vec2(p.x*sin(r + t)-p.y*cos(r + t), p.x*cos(r + t)+p.y*sin(r + t)) * a;
}

float cosh(float x){ return 0.5 * (exp(x) + exp(-x));}
float sinh(float x){ return 0.5 * (exp(x) - exp(-x));}

vec2 sech(vec2 p, float weight, float t)
{
  float d = cos(2.0*p.y) + cosh(2.0*p.x);
  if (d != 0)
  {
      d = weight * 2.0 / d;
  }
  return vec2(d * cos(p.y + t) * cosh(p.x), -d * sin(p.y + t + t) * sinh(p.x));
}

vec2 hank(vec2 p, float a, float t)
{
    float r = sqrt(length(p) * length(p)) * a;
    float theta = atan(p.y, p.x);
    return r * vec2(sin(theta + r + t), cos(theta - r + t));
}

vec2 eyeFish(vec2 p, float a, float t)
{
    float r = sqrt(length(p) * length(p));
    return (2/ (r + 1)) * p;
}

float pdjA = 0.5;
float pdjB = 1.5;
float pdjC = -1.0;
float pdjD = -1.0;

vec2 pdj(vec2 p, float amount, float t)
{
  return vec2( amount * (sin(pdjA * p.y) - cos(pdjB * p.x)),
                      amount * (sin(pdjC * p.x + t) - cos(pdjD * p.y + t)));
}

vec2 spherical(vec2 p, float a, float t)
{
    float r = a * (length(p) * length(p));
    return (1.0/r) * p;
}

vec2 spiral(vec2 p, float a, float t)
{
    float r = a * (1.0 / sqrt(length(p) * length(p)));
    float theta = atan(p.x, p.y);
    vec2 pos = r * vec2(cos(theta * 2. + t) + sin(r * 24. + t), sin(theta * 2. + t) - cos(r * 24. + t));
    return pos;
}

vec2 exponential(vec2 p, float a, float t)
{
    vec2 pos = exp(p.x - 1) * vec2(cos(PI*p.y + t), sin(PI*p.y + t));
    return pos;
}

vec2 cross(vec2 p, float f, float t)
{
    float a = sqrt(1.0/pow(p.x * p.x - p.y * p.y, 2.));
    vec2 pos = a * p;
    return pos;
}

vec2 heart(vec2 p, float f, float t)
{
   float r = f * sqrt(length(p) * length(p));
   float theta = atan(p.x/p.y);
   vec2 pos = vec2(sin(theta * r), -cos(theta * r));
   return r * pos;
}

vec2 disc(vec2 p, float f, float t)
{
    float r = f * sqrt(length(p) * length(p));
    float theta = atan(p.x/p.y);
    vec2 pos = theta/PI * vec2(sin(PI*r), cos(PI*r));
    return pos;
}

