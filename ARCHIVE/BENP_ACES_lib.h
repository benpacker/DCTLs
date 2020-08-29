// DEFINE CONSTANTS
  __CONSTANT__ float pie = 3.14159265358979323f;
  __CONSTANT__ float e = 2.718281828459045f;


// DECLARE COLOURSPACE CONVERSIONS
  __DEVICE__ float3 rgb_2_yab(float3 rgb);
  __DEVICE__ float3 yab_2_ych(float3 yab);
  __DEVICE__ float3 rgb_2_ych(float3 rgb);
  __DEVICE__ float3 yab_2_rgb(float3 yab);
  __DEVICE__ float3 ych_2_rgb(float3 ych);
  __DEVICE__ float3 ych_2_yab(float3 ych);
  __DEVICE__ float3 rgb_2_hsv(float3 rgb);
  __DEVICE__ float3 hsv_2_rgb(float3 hsv);

  __DEVICE__ float3 ARRI_2_ACEScct(float3 rgb);
  __DEVICE__ float3 RED_2_ACEScct(float3 rgb);
  __DEVICE__ float3 SONY_2_ACEScct(float3 rgb);

  __DEVICE__ float3 ACEScct_2_ARRI(float3 rgb);
  __DEVICE__ float3 ACEScct_2_RED(float3 rgb);
  __DEVICE__ float3 ACEScct_2_SONY(float3 rgb);


// DECLARE SELECTIVE SHAPER TOOLS
  __DEVICE__ float center_hue(float hue, float centerH);
  __DEVICE__ float uncenter_hue(float hueCentered, float centerH);
  __DEVICE__ float cubic_basis_shaper(float x, float w);
  __DEVICE__ float gaussian_shaper(float x, float target, float w);
  __DEVICE__ float sigmoid_shaper(float x, float target, float w);


// DEFINE COLOURSPACE CONVERSIONS
  __DEVICE__ float3 rgb_2_yab(float3 rgb) {
    float3 yab;
    yab.x = (rgb.x * 0.333333f) + (rgb.y * 0.333333f) + (rgb.z * 0.333333f);
    yab.y = (rgb.x * 0.5f) + (rgb.y * -0.25f) + (rgb.z * -0.25f);
    yab.z = (rgb.x * 0.0f) + (rgb.y * 0.433013f) + (rgb.z * -0.433013f);
    return yab;
  }

  __DEVICE__ float3 yab_2_ych(float3 yab) {
    float3 ych;
    ych.x = yab.x;
    ych.y = _sqrtf( yab.y * yab.y + yab.z * yab.z );
    ych.z = _atan2f(yab.z, yab.y) * (180.0f / pie);
    if (ych.z < 0.0f) ych.z += 360.0f;
    return ych;
  }

  __DEVICE__ float3 rgb_2_ych(float3 rgb) {
    float3 yab;
    yab.x = (rgb.x * 0.333333f) + (rgb.y * 0.333333f) + (rgb.z * 0.333333f);
    yab.y = (rgb.x * 0.5f) + (rgb.y * -0.25f) + (rgb.z * -0.25f);
    yab.z = (rgb.x * 0.0f) + (rgb.y * 0.433013f) + (rgb.z * -0.433013f);
    float3 ych;
    ych.x = yab.x;
    ych.y = _sqrtf( yab.y * yab.y + yab.z * yab.z );
    ych.z = _atan2f(yab.z, yab.y) * (180.0f / pie);
    if (ych.z < 0.0f) ych.z += 360.0f;
    return ych;
  }

  __DEVICE__ float3 ych_2_yab(float3 ych) {
    float3 yab;
    yab.x = ych.x;
    yab.y = ych.y * _cosf(ych.z * (pie / 180.0f));
    yab.z = ych.y * _sinf(ych.z * (pie / 180.0f));
    return yab;
  }

  __DEVICE__ float3 yab_2_rgb(float3 yab) {
    float3 rgb;
    rgb.x = (yab.x * 1.0f) + (yab.y * 1.333333f) + (yab.z * 0.0f);
    rgb.y = (yab.x * 1.0f) + (yab.y * -0.666666f) + (yab.z * 1.154701f);
    rgb.z = (yab.x * 1.0f) + (yab.y * -0.666666f) + (yab.z * -1.154701f);
    return rgb;
  }

  __DEVICE__ float3 ych_2_rgb(float3 ych) {
    float3 yab;
    yab.x = ych.x;
    yab.y = ych.y * _cosf(ych.z * (pie / 180.0f));
    yab.z = ych.y * _sinf(ych.z * (pie / 180.0f));
    float3 rgb;
    rgb.x = (yab.x * 1.0f) + (yab.y * 1.333333f) + (yab.z * 0.0f);
    rgb.y = (yab.x * 1.0f) + (yab.y * -0.666666f) + (yab.z * 1.154701f);
    rgb.z = (yab.x * 1.0f) + (yab.y * -0.666666f) + (yab.z * -1.154701f);
    return rgb;
  }

  __DEVICE__ float3 rgb_2_hsv(float3 rgb) {
    float min = _fminf(_fminf(rgb.x, rgb.y), rgb.z);
    float max = _fmaxf(_fmaxf(rgb.x, rgb.y), rgb.z);
    float delta = max - min;
    float HH;
    float SS;
    float VV;
    if (delta == 0.f) {
      HH = 0.f;
    } else if (rgb.x == max) {
      HH = (rgb.y - rgb.z) / delta;
    } else if (rgb.y == max) {
      HH = 2 + (rgb.z - rgb.x) / delta;
    } else {
      HH = 4 + (rgb.x - rgb.y) / delta;
    }
    HH *= 1.0f / 6.;
    if (HH < 0) {
      HH += 1.0f;
    }
    if (max != 0.) {
      SS = delta / max;
    } else {
      SS = 0.f;
    }
    VV = max;

    float3 hsv = make_float3(HH, SS, VV);
    return hsv;
  }

  __DEVICE__ float3 hsv_2_rgb(float3 hsv) {
    float3 rgb;
    if (hsv.y == 0.0f) {
      rgb.x = rgb.y = rgb.z = hsv.z;
      return rgb;
    }
    hsv.x *= 6.0f;
    int i = _floor(hsv.x);
    float f = hsv.x - i;
    i = (i >= 0) ? (i % 6) : (i % 6) + 6;
    float p = hsv.z * (1.0f - hsv.y);
    float q = hsv.z * (1.0f - hsv.y * f);
    float t = hsv.z * (1.0f - hsv.y * (1.0f - f));
    rgb.x = i == 0 ? hsv.z : i == 1 ? q : i == 2 ? p : i == 3 ? p : i == 4 ? t : hsv.z;
    rgb.y = i == 0 ? t : i == 1 ? hsv.z : i == 2 ? hsv.z : i == 3 ? q : i == 4 ? p : p;
    rgb.z = i == 0 ? p : i == 1 ? p : i == 2 ? t : i == 3 ? hsv.z : i == 4 ? hsv.z : q;
    return rgb;
  }


// DEFINE SELECTIVE SHAPER TOOLS
  __DEVICE__ float center_hue( float hue, float centerH) {
  float hueCentered = hue - centerH;
  if (hueCentered < -180.0f) hueCentered = hueCentered + 360.0f;
  else if (hueCentered > 180.0f) hueCentered = hueCentered - 360.0f;
  return hueCentered;
  }

  __DEVICE__ float uncenter_hue( float hueCentered, float centerH) {
  float hue = hueCentered + centerH;
  if (hue < 0.0f) hue = hue + 360.0f;
  else if (hue > 360.0f) hue = hue - 360.0f;
  return hue;
  }

  __DEVICE__ float cubic_basis_shaper( float x, float w) {
  float M[4][4] = { {-1.0f/6.0f, 3.0f/6.0f, -3.0f/6.0f, 1.0f/6.0f}, {3.0f/6.0f, -1.0f, 3.0f/6.0f, 0.0f},
  {-3.0f/6.0f, 0.0f, 3.0f/6.0f, 0.0f}, {1.0f/6.0f, 4.0f/6.0f, 1.0f/6.0f, 0.0f} };
  float knots[5] = {-w/2.0f, -w/4.0f, 0.0f, w/4.0f, w/2.0f};
  float y = 0.0f;
  if ((x > knots[0]) && (x < knots[4])) {
  float knot_coord = (x - knots[0]) * 4.0f/w;
  int j = knot_coord;
  float t = knot_coord - j;
  float monomials[4] = { t * t * t, t * t, t, 1.0f };
  if ( j == 3) {
  y = monomials[0] * M[0][0] + monomials[1] * M[1][0] + monomials[2] * M[2][0] + monomials[3] * M[3][0];
  } else if ( j == 2) {
  y = monomials[0] * M[0][1] + monomials[1] * M[1][1] + monomials[2] * M[2][1] + monomials[3] * M[3][1];
  } else if ( j == 1) {
  y = monomials[0] * M[0][2] + monomials[1] * M[1][2] + monomials[2] * M[2][2] + monomials[3] * M[3][2];
  } else if ( j == 0) {
  y = monomials[0] * M[0][3] + monomials[1] * M[1][3] + monomials[2] * M[2][3] + monomials[3] * M[3][3];
  } else {
  y = 0.0f;
  }}
  return y * 3.0f/2.0f;
  }

  __DEVICE__ float gaussian_shaper( float x, float target, float w) {
    float base = 1.0f * e;
    float power = (_powf((x - target), 2.0f)) / (_powf((2.0f * w), 2.0f));
    float out = _powf(base, -power);
    return out;
  }

  __DEVICE__ float sigmoid_shaper( float x, float target, float w) {
    float out;
    out = (1 / (1.0f + _powf(e, (-8.9f * w) * (x - target))));
    return out;
  }




// DEFINE SKIN TREATMENT
  __DEVICE__ float skin_bypass( float3 rgb, float3 target_rgb, float factor ) {
    float3 pre_ych = rgb_2_ych(rgb);
    float3 target_ych = rgb_2_ych(target_rgb);
    float3 pre_hsv = rgb_2_hsv(rgb);
    float3 target_hsv = rgb_2_hsv(target_rgb);
    float cntr_hue = center_hue(pre_ych.z, target_ych.z);
    float HH = cubic_basis_shaper(cntr_hue, (360.0f * factor));
    float SS = cubic_basis_shaper((pre_hsv.y - (target_hsv.y + 0.05f)), (0.75f * factor));
    float VV = cubic_basis_shaper((pre_hsv.z - target_hsv.z), (0.75f * factor));
    float alpha = HH * SS * VV;
    return alpha;
  }
