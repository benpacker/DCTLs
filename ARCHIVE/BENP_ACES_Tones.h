// DEFINE TONES
  __DEVICE__ float3 tone_001( float3 rgb, float mid_grey ) {
    float3 ych = rgb_2_ych(rgb);
    float tone_r = cubic_basis_shaper((ych.x - mid_grey), 1.75f);
    float tone_g = cubic_basis_shaper((ych.x - mid_grey), 1.3f);
    float tone_b = cubic_basis_shaper((ych.x - mid_grey), 1.2f);
    float tone = 1 - (cubic_basis_shaper((ych.x - mid_grey), 0.66f) );
    float3 toning;

    if (rgb.x > mid_grey) {
  		toning.x = ((rgb.x - mid_grey) * 0.90f) + mid_grey;
  	} else {
  		toning.x = ((rgb.x - mid_grey) * 1.25f) + mid_grey;
  	}
  	if (rgb.y > mid_grey) {
  		toning.y = ((rgb.y - mid_grey) * 0.78f) + mid_grey;
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

    return toning;
  }
