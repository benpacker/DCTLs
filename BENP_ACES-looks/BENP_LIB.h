// DEFINE CONSTANTS
  __CONSTANT__ float pie = 3.14159265358979323f;
  __CONSTANT__ float e = 2.718281828459045f;


// DEFINE COLOURSPACE CONVERSIONS
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


// DEFINE TONES
  __DEVICE__ float3 tone_001( float3 rgb, float mid_grey ) {
    float3 ych = rgb_2_ych(rgb);
    float tone_r = cubic_basis_shaper((ych.x - mid_grey), 1.75f);
    float tone_g = cubic_basis_shaper((ych.x - mid_grey), 1.3f);
    float tone_b = cubic_basis_shaper((ych.x - mid_grey), 1.2f);
    float tone = 1 - (cubic_basis_shaper((ych.x - mid_grey), 0.66f) );
    float toe = sigmoid_shaper(ych.x, 0.145f, 4.0f);
    float3 toning;

    if (rgb.x > mid_grey) {
  		toning.x = ((rgb.x - mid_grey) * 0.90f) + mid_grey;
  	} else {
  		toning.x = ((rgb.x - mid_grey) * 1.25f) + mid_grey;
  	}
  	if (rgb.y > mid_grey) {
  		toning.y = ((rgb.y - mid_grey) * 0.81f) + mid_grey;
  	} else {
  		toning.y = ((rgb.y - mid_grey) * 1.05f) + mid_grey;
  	}
  	if (rgb.z > mid_grey) {
  		toning.z = ((rgb.z - mid_grey) * 0.65f) + mid_grey;
  	} else {
  		toning.z = ((rgb.z - mid_grey) * 0.85f) + mid_grey;
  	}

  	toning.x = (((tone_r * tone) * toning.x) + (rgb.x * ( 1 - (tone_r * tone))));
  	toning.y = (((tone_g * tone) * toning.y) + (rgb.y * ( 1 - (tone_g * tone))));
  	toning.z = (((tone_b * tone) * toning.z) + (rgb.z * ( 1 - (tone_b * tone))));

    if (toning.x > mid_grey) {
      toning.x = ((toning.x - mid_grey) * 0.975f) + mid_grey;
    } else {
      toning.x = ((toning.x - mid_grey) * 1.05f) + mid_grey;
    }
    if (toning.y > mid_grey) {
      toning.y = ((toning.y - mid_grey) * 0.975f) + mid_grey;
    } else {
      toning.y = ((toning.y - mid_grey) * 1.05f) + mid_grey;
    }
    if (toning.z > mid_grey) {
      toning.z = ((toning.z - mid_grey) * 0.975f) + mid_grey;
    } else {
      toning.z = ((toning.z - mid_grey) * 1.05f) + mid_grey;
    }

    toning.x = (toe * toning.x) + (0.11f * (1 - toe));
    toning.y = (toe * toning.y) + (0.11f * (1 - toe));
    toning.z = (toe * toning.z) + (0.11f * (1 - toe));

    return toning;
  }
  __DEVICE__ float3 tone_002( float3 rgb, float mid_grey ) {
    float3 ych = rgb_2_ych(rgb);
    float tone_r = cubic_basis_shaper((ych.x - mid_grey), 1.5f);
    float tone_g = cubic_basis_shaper((ych.x - mid_grey), 1.3f);
    float tone_b = cubic_basis_shaper((ych.x - mid_grey), 1.2f);
    float tone = (1 - (cubic_basis_shaper((ych.x - mid_grey), 0.75f)) * 0.45f);
    float toe = sigmoid_shaper(ych.x, 0.145f, 4.0f);
    float3 toning;

    if (rgb.x > mid_grey) {
  		toning.x = ((rgb.x - mid_grey) * 0.613f) + mid_grey;
  	} else {
  		toning.x = ((rgb.x - mid_grey) * 1.25f) + mid_grey;
  	}
  	if (rgb.y > mid_grey) {
  		toning.y = ((rgb.y - mid_grey) * 0.66f) + mid_grey;
  	} else {
  		toning.y = ((rgb.y - mid_grey) * 0.7f) + mid_grey;
  	}
  	if (rgb.z > mid_grey) {
  		toning.z = ((rgb.z - mid_grey) * 0.7f) + mid_grey;
  	} else {
  		toning.z = ((rgb.z - mid_grey) * 0.5f) + mid_grey;
  	}

  	toning.x = (((tone_r * tone) * toning.x) + (rgb.x * ( 1 - (tone_r * tone))));
  	toning.y = (((tone_g * tone) * toning.y) + (rgb.y * ( 1 - (tone_g * tone))));
  	toning.z = (((tone_b * tone) * toning.z) + (rgb.z * ( 1 - (tone_b * tone))));

    if (toning.x > mid_grey) {
      toning.x = ((toning.x - mid_grey) * 0.975f) + mid_grey;
    } else {
      toning.x = ((toning.x - mid_grey) * 0.88f) + mid_grey;
    }
    if (toning.y > mid_grey) {
      toning.y = ((toning.y - mid_grey) * 0.975f) + mid_grey;
    } else {
      toning.y = ((toning.y - mid_grey) * 1.0f) + mid_grey;
    }
    if (toning.z > mid_grey) {
      toning.z = ((toning.z - mid_grey) * 0.975f) + mid_grey;
    } else {
      toning.z = ((toning.z - mid_grey) * 1.0f) + mid_grey;
    }

    toning.x = (toe * toning.x) + (0.138f * (1 - toe));
    toning.y = (toe * toning.y) + (0.138f * (1 - toe));
    toning.z = (toe * toning.z) + (0.138f * (1 - toe));

    return toning;
  }


// DEFINE LOOKS
  __DEVICE__ float3 look_001( float3 rgb ) {
    float3 new_change;
    float3 target_hue;
    float3 target_sat;
    float3 target_val;
    float cntr_hue;
    float cntr_tgt;
    float HH;
    float SS;
    float VV;

  // INPUT COLOURSPACES
    float3 in_rgb = rgb;
    float3 in_ych = rgb_2_ych(rgb);
    float3 in_hsv = rgb_2_hsv(rgb);

  // C00 GLOBAL PRECHANGE
    new_change = in_hsv;
    new_change.y = new_change.y * 0.75f;
    float3 C00_rgb = hsv_2_rgb(new_change);
    float3 C00_ych = rgb_2_ych(C00_rgb);
    float3 C00_hsv = new_change;

  // C01 RED TWEAK (target, width, change)
    target_hue = make_float3(0.0f, 280.0f, 356.0f);
    target_sat = make_float3(0.5f, 0.5f, 1.2f);
    // target_val = make_float3(0.0f, 0.0f, 0.0f);
    cntr_hue = center_hue(in_ych.z, target_hue.x);
    cntr_tgt = center_hue(in_ych.z, target_hue.z);
    HH = cubic_basis_shaper(cntr_hue, target_hue.y);
    SS = gaussian_shaper(in_hsv.y, target_sat.x, target_sat.y);
    // VV = gaussian_shaper(pre_hsv.z, target_val.x, target_val.y);

    if (HH > 0.0f) {
      new_change.x = C00_ych.x - (C00_hsv.y * 0.45f);
      new_change.y = C00_ych.y * 2.35f;
      new_change.z = uncenter_hue(((HH * cntr_tgt) + (cntr_hue * ( 1 - HH))), target_hue.x);
    } else {
      new_change = C00_ych;
    }

    float C01_alpha = HH;
    float3 C01;
    C01 = (C00_rgb * ( 1 - C01_alpha)) + (C01_alpha * ych_2_rgb(new_change));
    // C01 = _saturatef(C01);


  // C02 YELLOW TWEAK (target, width, change)
    target_hue = make_float3(60.0f, 180.0f, 85.0f);
    // target_sat = make_float3(0.0f, 0.0f, 0.0f);
    target_val = make_float3(0.2f, 0.70f, 0.5f);
    cntr_hue = center_hue(in_ych.z, target_hue.x);
    cntr_tgt = center_hue(in_ych.z, target_hue.z);
    HH = cubic_basis_shaper(cntr_hue, target_hue.y);
    // SS = gaussian_shaper(pre_hsv.y, target_sat.x, target_sat.y);
    VV = cubic_basis_shaper(target_val.x, target_val.y);

    if (HH > 0.0f) {
      new_change.x = C00_ych.x - (C00_hsv.y * 0.2f);
      new_change.y = C00_ych.y * (30.0f + C00_ych.y);
      new_change.z = uncenter_hue(((HH * cntr_tgt) + (cntr_hue * ( 1 - HH))), target_hue.x);
    } else {
      new_change = C00_ych;
    }

    float C02_alpha = HH * VV;
    float3 C02;
    C02 = (C00_rgb * ( 1 - C02_alpha)) + (C02_alpha * ych_2_rgb(new_change));
    // C02 = _saturatef(C02);

  // C03 GREEN TWEAK (target, width, change)
    target_hue = make_float3(100.0f, 200.0f, 130.0f);
    target_sat = make_float3(0.4f, 0.8f, 0.0f);
    // target_val = make_float3(0.0f, 0.0f, 0.0f);
    cntr_hue = center_hue(in_ych.z, target_hue.x);
    cntr_tgt = center_hue(in_ych.z, target_hue.z);
    HH = cubic_basis_shaper(cntr_hue, target_hue.y);
    SS = sigmoid_shaper(in_ych.x, target_sat.x, target_sat.y);
    // VV = gaussian_shaper(pre_hsv.z, target_val.x, target_val.y);

    if (HH > 0.0f) {
      new_change.x = C00_ych.x - (C00_hsv.y * 0.1f);
      new_change.y = C00_ych.y * (1.65f + C00_ych.y);
      new_change.z = uncenter_hue(((HH * cntr_tgt) + (cntr_hue * ( 1 - HH))), target_hue.x);
    } else {
      new_change = C00_ych;
    }

    float C03_alpha = HH * SS;
    float3 C03;
    C03 = (C00_rgb * ( 1 - C03_alpha)) + (C03_alpha * ych_2_rgb(new_change));
    // C03 = _saturatef(C03);

  // C04 BLUE TWEAK (target, width, change)
    target_hue = make_float3(200.0f, 280.0f, 200.0f);
    target_sat = make_float3(0.7f, 5.0f, 0.0f);
    // target_val = make_float3(0.0f, 0.0f, 0.0f);
    cntr_hue = center_hue(in_ych.z, target_hue.x);
    cntr_tgt = center_hue(in_ych.z, target_hue.z);
    HH = cubic_basis_shaper(cntr_hue, target_hue.y);
    SS = cubic_basis_shaper(target_sat.x, target_sat.y);
    // VV = gaussian_shaper(pre_hsv.z, target_val.x, target_val.y);

    if (HH > 0.0f) {
      new_change.x = C00_ych.x - (C00_hsv.y * 0.35f);
      new_change.y = C00_ych.y * (2.85f + C00_ych.y);
      new_change.z = uncenter_hue(((HH * cntr_tgt) + (cntr_hue * ( 1 - HH))), target_hue.x);
    } else {
      new_change = C00_ych;
    }

    float C04_alpha = HH * SS;
    float3 C04;
    C04 = (C00_rgb * ( 1 - C04_alpha)) + (C04_alpha * ych_2_rgb(new_change));
    // C04 = _saturatef(C04);

  // C05 CYAN TWEAK (target, width, change)
    target_hue = make_float3(180.0f, 150.0f, 170.0f);
    target_sat = make_float3(0.5f, 0.5f, 0.0f);
    // target_val = make_float3(0.0f, 0.0f, 0.0f);
    cntr_hue = center_hue(in_ych.z, target_hue.x);
    cntr_tgt = center_hue(in_ych.z, target_hue.z);
    HH = cubic_basis_shaper(cntr_hue, target_hue.y);
    SS = cubic_basis_shaper(target_sat.x, target_sat.y);
    // VV = gaussian_shaper(pre_hsv.z, target_val.x, target_val.y);

    if (HH > 0.0f) {
      new_change.x = C00_ych.x - (C00_hsv.y * 0.05f);
      new_change.y = C00_ych.y * (2.25f + C00_ych.y);
      new_change.z = uncenter_hue(((HH * cntr_tgt) + (cntr_hue * ( 1 - HH))), target_hue.x);
    } else {
      new_change = C00_ych;
    }

    float C05_alpha = HH * SS;
    float3 C05;
    C05 = (C00_rgb * ( 1 - C05_alpha)) + (C05_alpha * ych_2_rgb(new_change));
    // C05 = _saturatef(C05);




  // OUTPUT AVERAGING
    float look_alpha = C01_alpha + C02_alpha + C03_alpha + C04_alpha + C05_alpha;
    new_change = ((C01_alpha * C01) + (C02_alpha * C02) + (C03_alpha * C03) + (C04_alpha * C04) + (C05_alpha * C05)) / look_alpha;
    // new_change = _saturatef(new_change);


    float3 look;
    if (look_alpha != 0.0f) {
      look = (C00_rgb * ( 1 - look_alpha)) + (look_alpha * new_change);
    } else {
      look = C00_rgb;
    }



  // FIXES - ZERO AXIS (target, softness, change)
    target_sat = make_float3(0.0f, 0.4f, 0.5f);
    float fix_alpha = gaussian_shaper(in_hsv.y, target_sat.x, target_sat.y);
    fix_alpha = _saturatef(fix_alpha - 0.55f);
    look = (fix_alpha * C00_rgb) + (look * ( 1 - fix_alpha));

  // FIXES - SHADOWS
    // target_hue = make_float3(180.0f, 200.0f, 170.0f);
    // target_sat = make_float3(0.0f, 0.0f, 0.0f);
    target_val = make_float3(0.0f, 0.125f, 0.1f);
    // cntr_hue = center_hue(in_ych.z, target_hue.x);
    // cntr_tgt = center_hue(in_ych.z, target_hue.z);
    // HH = cubic_basis_shaper(cntr_hue, target_hue.y);
    // SS = gaussian_shaper(pre_hsv.y, target_sat.x, target_sat.y);
    VV = gaussian_shaper(in_ych.x, target_val.x, target_val.y);
    look = (VV * C00_rgb) + (look * ( 1 - VV));

  // FIXES - High Saturation (target, softness, change)
    float3 fix_hsv = rgb_2_hsv(look);
    target_sat = make_float3(0.5f, 0.5f, 1.0f);
    fix_alpha = sigmoid_shaper(fix_hsv.y, target_sat.x, target_sat.y);
    if (fix_alpha > 0.0f) {
    new_change.x = fix_hsv.x;
    new_change.y = fix_hsv.y * (target_sat.z * fix_alpha);
    new_change.z = fix_hsv.z;
    } else {
    new_change = fix_hsv;
    }
    new_change = hsv_2_rgb(new_change);

    look = (look * ( 1 - fix_alpha)) + (fix_alpha * new_change);

    return look;
  }

  __DEVICE__ float3 look_002( float3 rgb ) {
    float3 new_change;
    float3 target_hue;
    float3 target_sat;
    float3 target_val;
    float cntr_hue;
    float cntr_tgt;
    float HH;
    float SS;
    float VV;

  // INPUT COLOURSPACES
    float3 in_rgb = rgb;
    float3 in_ych = rgb_2_ych(rgb);
    float3 in_hsv = rgb_2_hsv(rgb);

    return rgb;
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
