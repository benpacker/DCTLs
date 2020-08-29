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
    // target_sat = make_float3(0.0f, 0.0f, 0.0f);
    // target_val = make_float3(0.0f, 0.0f, 0.0f);
    cntr_hue = center_hue(in_ych.z, target_hue.x);
    cntr_tgt = center_hue(in_ych.z, target_hue.z);
    HH = cubic_basis_shaper(cntr_hue, target_hue.y);
    // SS = gaussian_shaper(pre_hsv.y, target_sat.x, target_sat.y);
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
      new_change.y = C00_ych.y * (4.0f + C00_ych.y);
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
    look = _saturatef(look);

    return look;
  }
