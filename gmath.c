
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE

  Ambient light is represeneted by a color value

  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.

  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  color i_amb, i_dif, i_spec;

  i_amb = calculate_ambient(alight,areflect);
  i_dif = calculate_diffuse(light,dreflect,normal);
  i_spec = calculate_specular(light,sreflect,view,normal);

  //set r,g,b of resulting i individually:
  i.red = i_amb.red + i_dif.red + i_spec.red;
  i.green = i_amb.green + i_dif.green + i_spec.green;
  i.blue = i_amb.blue + i_dif.blue + i_spec.blue;

  limit_color(&i);
  //printf("%d %d %d\n",i.red,i.green,i.blue);

  return i;

}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];

  limit_color(&a);
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double l[3];

  l[0] = light[0][0];
  l[1] = light[0][1];
  l[2] = light[0][2];
  //normalize n and l
  normalize(normal);
  normalize(l);
  double temp = dot_product(normal, l);
  if (temp < 0) temp = 0;

  d.red = light[1][0] * dreflect[0] * temp;

  // temp = dot_product(normal[1], light[0][1]);
  d.green = light[1][1] * dreflect[1] * temp;

  // temp = dot_product(normal[2], light[0][2]);
  d.blue = light[1][2] * dreflect[2] * temp;

  limit_color(&d);
  return d;

}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double nl, prod;
  double lighting_vals[3];
  int power = 10;
  //normalize n, v, and l
  normalize(normal);
  //normalize(light[0]);
  normalize(view);
  //normalize light values
  double l[3];
  l[0] = light[0][0];
  l[1] = light[0][1];
  l[2] = light[0][2];
  normalize(l);

  // int i;
  // // for (i=0;i<3;i++) {
  // //   printf("%d\t",light[0][i]);
  // // }

  nl = dot_product(normal, l);
  if (nl < 0) nl =0;

  //create lighting_vals vector
  lighting_vals[0] = 2 * normal[0] * nl - l[0];
  lighting_vals[1] = 2 * normal[1] * nl - l[1];
  lighting_vals[2] = 2 * normal[2] * nl - l[2];

  prod = dot_product(lighting_vals, view);
  prod = pow(prod,power);

  //RED
  s.red = light[1][0] * sreflect[0] * prod;
  // s.red = pow(s.red, power);

  //GREEN
  s.green = light[1][1] * sreflect[1] * prod;
  // s.green = pow(s.green,power);

  //BLUE
  s.blue = light[1][2] * sreflect[2] * prod;
  // s.blue = pow(s.blue, power);

  limit_color(&s);
  return s;

}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  // if ((*c).red > 255) (*c).red = 255;
  // else if ((*c).red < 0) (*c).red = 0;
  //
  // if ((*c).green > 255) (*c).green = 255;
  // else if ((*c).green < 0) (*c).green = 0;
  //
  // if ((*c).blue > 255) (*c).blue = 255;
  // else if ((*c).blue < 0) (*c).blue = 0;


  if (c->red > 255) c->red = 255;
  else if (c->red < 0) c->red = 0;

  if (c->green > 255) c->green = 255;
  else if (c->green < 0) c->green = 0;

  if (c->blue > 255) c->blue = 255;
  else if (c->blue < 0) c->blue = 0;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
