/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apregitz <apregitz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:38:36 by nweber            #+#    #+#             */
/*   Updated: 2025/11/15 16:30:29 by apregitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGS_H
# define SETTINGS_H

// basic settings
// SCALE: image size multiblied by the scale
// AA_MIN_SAMPLES: the minimum amount of rays per pixel
// MAX_DEPTH: the number of child rays
// SHADOW_SAMPLES: number of shadow rays per light for soft shadows
# define SCALE 70

# define AA_MIN_SAMPLES 16
# define MAX_DEPTH 32
# define SHADOW_SAMPLES 4

// for lume and color intensity(dont need to be modified normally)
# define AA_VAR_EPS 0.0004
# define COLOR_INTENSITY 0.5
# define LIGHT_MULTIPLIER 1.0

// will be set automaticly(dont change them)
// # define WIDTH (1920 * (float)((float)SCALE / 100))
// # define HEIGHT (1080 * (float)((float)SCALE / 100))
// # define ASPECT_RATIO ((double)WIDTH / (double)HEIGHT)
// # define AA_MAX_SAMPLES (AA_MIN_SAMPLES * 8)

# ifndef ANTI_ALIASING
#  define ANTI_ALIASING 0
# endif

# ifndef MULTI_THREADING
#  define MULTI_THREADING 1
# endif

# ifndef USE_BVH
#  define USE_BVH 1
# endif

// # ifndef BONUS
// #  define BONUS 0
// # endif

#endif