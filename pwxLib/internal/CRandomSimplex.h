#pragma once
#ifndef PWX_RANDOM_SIMPLEX_H
#define PWX_RANDOM_SIMPLEX_H

#ifndef PWX_RANDOM_H_INCLUDED
# error "Don't include CRandomSimplex.h directly! Include pwxLib/CRandom.h instead!"
#endif


/** @internal
  * @file CRandomSimplex.h
  *
  * @brief Implementations for CRandom - Simplex Noise Generation
  *
  * @see CRandom.h for documentsation and history
  *
**/

namespace pwx {
namespace rng {
// Note: The namespaces are documented in CRandom.h

/* -------------------------------------- *
 * --- Public Methods Implementations --- *
 * -------------------------------------- */

/** @brief calculate a one dimensional simplex noise value
  *
  * This method returns a simplex noise value of one dimension.
  *
  * - zoom and smooth have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
**/
double CRandom::simplex1D(double x, double zoom, double smooth)
{
  if (zoom      < 0.001) zoom = 0.001;
  if (smooth    < 1.0)   smooth = 1.0;

  x += seed;

  return (getSpx1D(x / zoom) / smooth);
}


/** @brief calculate a one dimensional simplex wave
  *
  * This method returns a simplex wave of one dimension. The number of waves specified will overlay
  * each others with multiplied smoothing, specified by 'reduction'. The default value of 1 wave
  * just returns the simplex noise value in an intervall of [-1, 1]. While having more than one waves
  * makes it _possible_ to stay in this intervall, the results will most probably be nearer to zero.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * - reduction has a minimum value of 1.0 to ensure the savety of the resulting intervall.
  *
  * - waves has a minimum value of 1.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
  * @param[in] reduction Multiplier for the smoothing factor in each round.
  * @param[in] waves Number of waves to overlay. The default of 1 returns the pure Simplex Noise Value.
**/
double CRandom::simplex1D(double x, double zoom, double smooth, double reduction, int32_t waves)
{
  if (zoom      < 0.001) zoom = 0.001;
  if (smooth    < 1.0)   smooth = 1.0;
  if (reduction < 1.0)   reduction = 1.0;
  if (waves     < 1)     waves = 1;

  x += seed;

  double result = getSpx1D(x / zoom) / smooth;

  if (waves > 1)
    {
      double currWave   = 1.0;
      double currSmooth = smooth;
      double factor     = 1.0;
      double currZoom, dX;

      while (currWave < waves)
        {
          currWave   += 1.0;
          currSmooth *= reduction;
          currZoom    = zoom / ::std::pow(currWave, 2);
          dX          = x / currZoom;
          result     += getSpx1D(dX) / currSmooth;
          factor     += 1.0 / currSmooth;
        }
      result /= factor;
    }

  return (result);
}


/** @brief calculate a two dimensional simplex noise value
  *
  * This method returns a simplex noise value of two dimensions.
  *
  * - zoom and smooth have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
**/
double CRandom::simplex2D(double x, double y, double zoom, double smooth)
{
  if (zoom   < 0.001) zoom = 0.001;
  if (smooth < 1.0)   smooth = 1.0;

  x += seed;
  y += seed;

  return (getSpx2D(x / zoom, y / zoom) / smooth);
}


/** @brief calculate a two dimensional simplex wave
  *
  * This method returns a simplex wave of two dimensions. The number of waves specified will overlay
  * each others with multiplied smoothing, specified by 'reduction'. The default value of 1 wave just
  * returns the simplex noise value in an intervall of [-1, 1]. While having more than one waves makes
  * it _possible_ to stay in this intervall, the results will most probably be nearer to zero.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * - reduction has a minimum value of 1.0 to ensure the savety of the resulting intervall.
  *
  * - waves has a minimum value of 1.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
  * @param[in] reduction Multiplier for the smoothing factor in each round.
  * @param[in] waves Number of waves to overlay. The default of 1 returns the pure Simplex Noise Value.
**/
double CRandom::simplex2D(double x, double y, double zoom, double smooth, double reduction, int32_t waves)
{
  if (zoom      < 0.001) zoom = 0.001;
  if (smooth    < 1.0)   smooth = 1.0;
  if (reduction < 1.0)   reduction = 1.0;
  if (waves     < 1)     waves = 1;

  x += seed;
  y += seed;

  double result = getSpx2D(x / zoom, y / zoom) / smooth;

  if (waves > 1)
    {
      double currWave   = 1.0;
      double currSmooth = smooth;
      double factor     = 1.0;
      double currZoom, dX, dY;

      while (currWave < waves)
        {
          currWave   += 1.0;
          currSmooth *= reduction;
          currZoom    = zoom / ::std::pow(currWave, 2);
          dX          = x / currZoom;
          dY          = y / currZoom;
          result     += getSpx2D(dX, dY) / currSmooth;
          factor     += 1.0 / currSmooth;
        }
      result /= factor;
    }

  return (result);
}


/** @brief calculate a three dimensional simplex noise value
  *
  * This method returns a simplex wave of three dimensions.
  *
  * - zoom and smooth have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] z Z-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
**/
double CRandom::simplex3D(double x, double y, double z, double zoom, double smooth)
{
  if (zoom   < 0.001) zoom = 0.001;
  if (smooth < 1.0)   smooth = 1.0;

  x += seed;
  y += seed;
  z += seed;

  return (getSpx3D(x / zoom, y / zoom, z / zoom) / smooth);
}


/** @brief calculate a three dimensional simplex wave
  *
  * This method returns a simplex wave of three dimensions. The number of waves specified will overlay
  * each others with reduced zoom, specified by 'reduction'. The default value of 1 wave just returns
  * the simplex noise value in an intervall of [-1, 1]. While having more than one waves makes it
  * _possible_ to stay in this intervall, the results will most probably be nearer to zero.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * - reduction has a minimum value of 1.0 to ensure the savety of the resulting intervall.
  *
  * - waves has a minimum value of 1.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] z Z-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
  * @param[in] reduction Multiplier for the smoothing factor in each round.
  * @param[in] waves Number of waves to overlay. The default of 1 returns the pure Simplex Noise Value.
**/
double CRandom::simplex3D(double x, double y, double z, double zoom, double smooth, double reduction, int32_t waves)
{
  if (zoom      < 0.001) zoom = 0.001;
  if (smooth    < 1.0)   smooth = 1.0;
  if (reduction < 1.0)   reduction = 1.0;
  if (waves     < 1)     waves = 1;

  x += seed;
  y += seed;
  z += seed;

  double result = getSpx3D(x / zoom, y / zoom, z / zoom) / smooth;

  if (waves > 1)
    {
      double currWave   = 1.0;
      double currSmooth = smooth;
      double factor     = 1.0;
      double currZoom, dX, dY, dZ;

      while (currWave < waves)
        {
          currWave   += 1.0;
          currSmooth *= reduction;
          currZoom    = zoom / ::std::pow(currWave, 2);
          dX          = x / currZoom;
          dY          = y / currZoom;
          dZ          = z / currZoom;
          result     += getSpx3D(dX, dY, dZ) / currSmooth;
          factor     += 1.0 / currSmooth;
        }
      result /= factor;
    }

  return (result);
}


/** @brief calculate a four dimensional simplex noise value
  *
  * This method returns a simplex noise value of four dimension.
  *
  * - zoom and smooth have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] z Z-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] w W-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
**/
double CRandom::simplex4D(double x, double y, double z, double w, double zoom, double smooth)
{
  if (zoom   < 0.001) zoom = 0.001;
  if (smooth < 1.0)   smooth = 1.0;

  x += seed;
  y += seed;
  z += seed;
  w += seed;

  return (getSpx4D(x / zoom, y / zoom, z / zoom, w / zoom) / smooth);
}


/** @brief calculate a four dimensional simplex wave
  *
  * This method returns a simplex wave of four dimension. The number of waves specified will overlay
  * each others with reduced zoom, specified by 'reduction'. The default value of 1 wave just returns
  * the simplex noise value in an intervall of [-1, 1]. While having more than one waves makes it
  * _possible_ to stay in this intervall, the results will most probably be nearer to zero.
  *
  * - zoom, smooth, reduction and waves all have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * - reduction has a minimum value of 1.0 to ensure the savety of the resulting intervall.
  *
  * - waves has a minimum value of 1.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] z Z-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] w W-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
  * @param[in] reduction Multiplier for the smoothing factor in each round.
  * @param[in] waves Number of waves to overlay. The default of 1 returns the pure Simplex Noise Value.
**/
double CRandom::simplex4D(double x, double y, double z, double w, double zoom, double smooth, double reduction, int32_t waves)
{
  if (zoom      < 0.001) zoom = 0.001;
  if (smooth    < 1.0)   smooth = 1.0;
  if (reduction < 1.0)   reduction = 1.0;
  if (waves     < 1)     waves = 1;

  x += seed;
  y += seed;
  z += seed;
  w += seed;

  double result = getSpx4D(x / zoom, y / zoom, z / zoom, w / zoom) / smooth;

  if (waves > 1)
    {
      double currWave   = 1.0;
      double currSmooth = smooth;
      double factor     = 1.0;
      double currZoom, dX, dY, dZ, dW;

      while (currWave < waves)
        {
          currWave   += 1.0;
          currSmooth *= reduction;
          currZoom    = zoom / ::std::pow(currWave, 2);
          dX          = x / currZoom;
          dY          = y / currZoom;
          dZ          = z / currZoom;
          dW          = w / currZoom;
          result     += getSpx4D(dX, dY, dZ, dW) / currSmooth;
          factor     += 1.0 / currSmooth;
        }
      result /= factor;
    }

  return (result);
}

/* --------------------------------------- *
 * --- Private Methods Implementations --- *
 * --------------------------------------- */

/** @brief get Simplex Dot for one dimension
**/
double CRandom::getSimpDot(int32_t index, double x)
{
  assert((index >= 0) && (index < 4));
  return ( (constants::spxGrTab[index][0] * x));
}


/** @brief get Simplex Dot for second dimension
**/
double CRandom::getSimpDot(int32_t index, double x, double y)
{
  assert((index >= 0) && (index < 8));
  return ( (constants::spxGrTab[index][0] * x)
           +(constants::spxGrTab[index][1] * y));
}


/** @brief get Simplex Dot for third dimension
**/
double CRandom::getSimpDot(int32_t index, double x, double y, double z)
{
  assert((index >= 0) && (index < 12));
  return ( (constants::spxGrTab[index][0] * x)
           +(constants::spxGrTab[index][1] * y)
           +(constants::spxGrTab[index][2] * z));
}


/** @brief get Simplex Dot for fourth dimension
**/
double CRandom::getSimpDot(int32_t index, double x, double y, double z, double w)
{
  assert((index >= 0) && (index < 32));
  return ( (constants::spxGrTab[index][0] * x)
           +(constants::spxGrTab[index][1] * y)
           +(constants::spxGrTab[index][2] * z)
           +(constants::spxGrTab[index][3] * w));
}


/** @brief simplex noise 1D
  *
  * Simplex noise for one dimensions. This method returns a noise value between -1.0 and 1.0
  *
  * @param[in] x X-Coordinate of the simplex point
  * @return Noise value between -1.0 and 1.0
**/
double CRandom::getSpx1D(double x)
{
  double contrib = 0.0;

  spxNorms[0] = static_cast<int32_t>(::std::floor(x)); // Normalized X-Coordinate
  spxPerms[0] = spxNorms[0] & 0x000000ff; // X-Coordinate factor for Permutation Table

  // Distances from left and right edge
  spxDist[0][0] = x   - spxNorms[0];
  spxDist[1][0] = 1.0 - spxDist[0][0];

  // Permutated numbers, normalized to a range of 0 to 3
  spxGrads[0] = spxTab[spxPerms[0]]     % 4;
  spxGrads[1] = spxTab[spxPerms[0] + 1] % 4;

  // Calculate the contribution from the two edges
  contrib = 0.75 - ::std::pow(spxDist[0][0], 2);
  spxCorn[0] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[0], noiseD(x)) : 0.0;
  contrib = 0.75 - ::std::pow(spxDist[1][0], 2);
  spxCorn[1] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[1], noiseD(x + 1)) : 0.0;


  // Add contributions from each corner to get the final noise value.
  // The result is a value in the interval [-1,1].
  double result = 3.16049383304737219191338226664811 * (spxCorn[0] + spxCorn[1]);
  // Note: This factor has been found by searching the factor needed
  //       To get 1.0 with the largest result out of 100M iterations
  if (result >  1.0l) result =  1.0l;
  if (result < -1.0l) result = -1.0l;
  return (result);
}


/** @brief simplex noise 2D
  *
  * Simplex noise for two dimensions. This method returns a noise value between -1.0 and 1.0
  *
  * @param[in] x X-Coordinate of the simplex point
  * @param[in] y Y-Coordinate of the simplex point
  * @return Noise value between -1.0 and 1.0
**/
double CRandom::getSpx2D(double x, double y)
{
  double contrib = x + ((x + y) * constants::spxSkew[0][0]);
  spxNorms[0] = static_cast<int32_t>(::std::floor(contrib)); // Normalized X-Coordinate
  contrib = y + ((x + y) * constants::spxSkew[0][0]);
  spxNorms[1] = static_cast<int32_t>(::std::floor(contrib)); // Normalized Y-Coordinate
  spxPerms[0] = spxNorms[0] & 0x000000ff; // X-Coordinate factor for Permutation Table
  spxPerms[1] = spxNorms[1] & 0x000000ff; // Y-Coordinate factor for Permutation Table

  // Distances from corners, middle and last corner are filled when offsets are clear
  spxDist[0][0] = x - (spxNorms[0] - ((spxNorms[0] + spxNorms[1]) * constants::spxSkew[0][1]));
  spxDist[0][1] = y - (spxNorms[1] - ((spxNorms[0] + spxNorms[1]) * constants::spxSkew[0][1]));

  spxOffs[0][0] = (spxDist[0][0] > spxDist[0][1]) ? 1 : 0; // Upper triangle (1, 0),
  spxOffs[0][1] = (spxDist[0][0] > spxDist[0][1]) ? 0 : 1; // Lower triangle (0, 1).

  // Distance from middle corner
  spxDist[1][0] = spxDist[0][0] - spxOffs[0][0] + constants::spxSkew[0][1];
  spxDist[1][1] = spxDist[0][1] - spxOffs[0][1] + constants::spxSkew[0][1];

  // Distance from last corner
  spxDist[2][0] = spxDist[0][0] - 1.0 + (2.0 * constants::spxSkew[0][1]);
  spxDist[2][1] = spxDist[0][1] - 1.0 + (2.0 * constants::spxSkew[0][1]);

  // Permutated numbers, normalized to a range of 0 to 7
  spxGrads[0] = spxTab[spxPerms[0] + spxTab[spxPerms[1]]] % 8;
  spxGrads[1] = spxTab[spxPerms[0] + spxOffs[0][0] + spxTab[spxPerms[1] + spxOffs[0][1]]] % 8;
  spxGrads[2] = spxTab[spxPerms[0] + 1 + spxTab[spxPerms[1] + 1]] % 8;

  // Calculate the contribution from the three corners
  contrib = 0.5 - ::std::pow(spxDist[0][0], 2) - ::std::pow(spxDist[0][1], 2);
  spxCorn[0] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[0], spxDist[0][0], spxDist[0][1]) : 0.0;
  contrib = 0.5 - ::std::pow(spxDist[1][0], 2) - ::std::pow(spxDist[1][1], 2);
  spxCorn[1] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[1], spxDist[1][0], spxDist[1][1]) : 0.0;
  contrib = 0.5 - ::std::pow(spxDist[2][0], 2) - ::std::pow(spxDist[2][1], 2);
  spxCorn[2] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[2], spxDist[2][0], spxDist[2][1]) : 0.0;
  // Note: This is not looped, because the loop would produce more overhead than it is worth to just have 3 less lines.

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  double result = 70.14805770653948968629265436902642 * (spxCorn[0] + spxCorn[1] + spxCorn[2]);
  // Note: This factor has been found by searching the factor needed
  //       To get 1.0 with the largest result out of 100M iterations
  if (result >  1.0l) result =  1.0l;
  if (result < -1.0l) result = -1.0l;
  return (result);
}


/** @brief simplex noise 3D
  *
  * Simplex noise for three dimensions. This method returns a noise value between -1.0 and 1.0
  *
  * @param[in] x X-Coordinate of the simplex point
  * @param[in] y Y-Coordinate of the simplex point
  * @param[in] z Z-Coordinate of the simplex point
  * @return Noise value between -1.0 and 1.0
**/
double CRandom::getSpx3D(double x, double y, double z)
{
  double contrib = x + ((x + y + z) * constants::spxSkew[1][0]);
  spxNorms[0] = static_cast<int32_t>(::std::floor(contrib)); // Normalized X-Coordinate
  contrib = y + ((x + y + z) * constants::spxSkew[1][0]);
  spxNorms[1] = static_cast<int32_t>(::std::floor(contrib)); // Normalized Y-Coordinate
  contrib = z + ((x + y + z) * constants::spxSkew[1][0]);
  spxNorms[2] = static_cast<int32_t>(::std::floor(contrib)); // Normalized Z-Coordinate
  spxPerms[0] = spxNorms[0] & 0x000000ff; // X-Coordinate factor for Permutation Table
  spxPerms[1] = spxNorms[1] & 0x000000ff; // Y-Coordinate factor for Permutation Table
  spxPerms[2] = spxNorms[2] & 0x000000ff; // Z-Coordinate factor for Permutation Table

  // Distances from corners, second, third and last corner are filled when offsets are clear
  spxDist[0][0] = x - (spxNorms[0] - ((spxNorms[0] + spxNorms[1] + spxNorms[2]) * constants::spxSkew[1][1]));
  spxDist[0][1] = y - (spxNorms[1] - ((spxNorms[0] + spxNorms[1] + spxNorms[2]) * constants::spxSkew[1][1]));
  spxDist[0][2] = z - (spxNorms[2] - ((spxNorms[0] + spxNorms[1] + spxNorms[2]) * constants::spxSkew[1][1]));

  // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
  // Determine which simplex we are in.
  if (spxDist[0][0] >= spxDist[0][1])
    {
      if (spxDist[0][1] >= spxDist[0][2])
        {
          // X Y Z order
          spxOffs[0][0] = 1;
          spxOffs[0][1] = 0;
          spxOffs[0][2] = 0;
          spxOffs[1][0] = 1;
          spxOffs[1][1] = 1;
          spxOffs[1][2] = 0;
        }
      else if (spxDist[0][0] >= spxDist[0][2])
        {
          // X Z Y order
          spxOffs[0][0] = 1;
          spxOffs[0][1] = 0;
          spxOffs[0][2] = 0;
          spxOffs[1][0] = 1;
          spxOffs[1][1] = 0;
          spxOffs[1][2] = 1;
        }
      else
        {
          // Z X Y order
          spxOffs[0][0] = 0;
          spxOffs[0][1] = 0;
          spxOffs[0][2] = 1;
          spxOffs[1][0] = 1;
          spxOffs[1][1] = 0;
          spxOffs[1][2] = 1;
        }
    }
  else   // spxDist[0][0] < spxDist[0][1]
    {
      if (spxDist[0][1] < spxDist[0][2])
        {
          // Z Y X order
          spxOffs[0][0] = 0;
          spxOffs[0][1] = 0;
          spxOffs[0][2] = 1;
          spxOffs[1][0] = 0;
          spxOffs[1][1] = 1;
          spxOffs[1][2] = 1;
        }
      else if (spxDist[0][0] < spxDist[0][2])
        {
          // Y Z X order
          spxOffs[0][0] = 0;
          spxOffs[0][1] = 1;
          spxOffs[0][2] = 0;
          spxOffs[1][0] = 0;
          spxOffs[1][1] = 1;
          spxOffs[1][2] = 1;
        }
      else
        {
          // Y X Z order
          spxOffs[0][0] = 0;
          spxOffs[0][1] = 1;
          spxOffs[0][2] = 0;
          spxOffs[1][0] = 1;
          spxOffs[1][1] = 1;
          spxOffs[1][2] = 0;
        }
    }

  // Distance from second corner
  spxDist[1][0] = spxDist[0][0] - spxOffs[0][0] + constants::spxSkew[1][1];
  spxDist[1][1] = spxDist[0][1] - spxOffs[0][1] + constants::spxSkew[1][1];
  spxDist[1][2] = spxDist[0][2] - spxOffs[0][2] + constants::spxSkew[1][1];

  // Distance from third corner
  spxDist[2][0] = spxDist[0][0] - spxOffs[1][0] + (2.0 * constants::spxSkew[1][1]);
  spxDist[2][1] = spxDist[0][1] - spxOffs[1][1] + (2.0 * constants::spxSkew[1][1]);
  spxDist[2][2] = spxDist[0][2] - spxOffs[1][2] + (2.0 * constants::spxSkew[1][1]);

  // Distance from last corner
  spxDist[3][0] = spxDist[0][0] - 1.0 + (3.0 * constants::spxSkew[1][1]);
  spxDist[3][1] = spxDist[0][1] - 1.0 + (3.0 * constants::spxSkew[1][1]);
  spxDist[3][2] = spxDist[0][2] - 1.0 + (3.0 * constants::spxSkew[1][1]);

  // Permutated numbers, normalized to a range of 0 to 11
  spxGrads[0] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2]]]] % 12;
  spxGrads[1] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxOffs[0][2]] + spxOffs[0][1]] + spxOffs[0][0]] % 12;
  spxGrads[2] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxOffs[1][2]] + spxOffs[1][1]] + spxOffs[1][0]] % 12;
  spxGrads[3] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + 1] + 1] + 1] % 12;

  // Calculate the contribution from the four corners
  contrib = 0.6 - ::std::pow(spxDist[0][0], 2) - ::std::pow(spxDist[0][1], 2) - ::std::pow(spxDist[0][2], 2);
  spxCorn[0] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[0], spxDist[0][0], spxDist[0][1], spxDist[0][2]) : 0.0;
  contrib = 0.6 - ::std::pow(spxDist[1][0], 2) - ::std::pow(spxDist[1][1], 2) - ::std::pow(spxDist[1][2], 2);
  spxCorn[1] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[1], spxDist[1][0], spxDist[1][1], spxDist[1][2]) : 0.0;
  contrib = 0.6 - ::std::pow(spxDist[2][0], 2) - ::std::pow(spxDist[2][1], 2) - ::std::pow(spxDist[2][2], 2);
  spxCorn[2] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[2], spxDist[2][0], spxDist[2][1], spxDist[2][2]) : 0.0;
  contrib = 0.6 - ::std::pow(spxDist[3][0], 2) - ::std::pow(spxDist[3][1], 2) - ::std::pow(spxDist[3][2], 2);
  spxCorn[3] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[3], spxDist[3][0], spxDist[3][1], spxDist[3][2]) : 0.0;

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  double result = 36.11293688087369702088835765607655 * (spxCorn[0] + spxCorn[1] + spxCorn[2] + spxCorn[3]);
  // Note: This factor has been found by searching the factor needed
  //       To get 1.0 with the largest result out of 100M iterations
  if (result >  1.0l) result =  1.0l;
  if (result < -1.0l) result = -1.0l;
  return (result);
}


/** @brief simplex noise 4D
  *
  * Simplex noise for four dimensions. This method returns a noise value between -1.0 and 1.0
  *
  * @param[in] x X-Coordinate of the simplex point
  * @param[in] y Y-Coordinate of the simplex point
  * @param[in] z Z-Coordinate of the simplex point
  * @param[in] w W-Coordinate of the simplex point
  * @return Noise value between -1.0 and 1.0
**/
double CRandom::getSpx4D(double x, double y, double z, double w)
{
  int32_t    traverse;
  double contrib = x + ((x + y + z + w) * constants::spxSkew[2][0]);
  spxNorms[0] = static_cast<int32_t>(::std::floor(contrib)); // Normalized X-Coordinate
  contrib = y + ((x + y + z + w) * constants::spxSkew[2][0]);
  spxNorms[1] = static_cast<int32_t>(::std::floor(contrib)); // Normalized X-Coordinate
  contrib = z + ((x + y + z + w) * constants::spxSkew[2][0]);
  spxNorms[2] = static_cast<int32_t>(::std::floor(contrib)); // Normalized X-Coordinate
  contrib = w + ((x + y + z + w) * constants::spxSkew[2][0]);
  spxNorms[3] = static_cast<int32_t>(::std::floor(contrib)); // Normalized X-Coordinate

  spxPerms[0] = spxNorms[0] & 0x000000ff; // X-Coordinate factor for Permutation Table
  spxPerms[1] = spxNorms[1] & 0x000000ff; // Y-Coordinate factor for Permutation Table
  spxPerms[2] = spxNorms[2] & 0x000000ff; // Z-Coordinate factor for Permutation Table
  spxPerms[3] = spxNorms[3] & 0x000000ff; // W-Coordinate factor for Permutation Table

  // Distances from corners, second, third and last corner are filled when offsets are clear
  spxDist[0][0] = x - (spxNorms[0] - ((spxNorms[0] + spxNorms[1] + spxNorms[2] + spxNorms[3]) * constants::spxSkew[2][1]));
  spxDist[0][1] = y - (spxNorms[1] - ((spxNorms[0] + spxNorms[1] + spxNorms[2] + spxNorms[3]) * constants::spxSkew[2][1]));
  spxDist[0][2] = z - (spxNorms[2] - ((spxNorms[0] + spxNorms[1] + spxNorms[2] + spxNorms[3]) * constants::spxSkew[2][1]));
  spxDist[0][3] = w - (spxNorms[3] - ((spxNorms[0] + spxNorms[1] + spxNorms[2] + spxNorms[3]) * constants::spxSkew[2][1]));

  // For the 4D case, the simplex is a 4D shape.
  // The method below is a good way of finding the ordering of x,y,z,w and
  // then find the correct traversal order for the simplex we’re in.
  // First, six pair-wise comparisons are performed between each possible pair
  // of the four coordinates, and the results are used to add up binary bits
  // for an integer index.
  traverse =  ((spxDist[0][0] > spxDist[0][1]) ? 32 : 0)
              + ((spxDist[0][0] > spxDist[0][2]) ? 16 : 0)
              + ((spxDist[0][1] > spxDist[0][2]) ?  8 : 0)
              + ((spxDist[0][0] > spxDist[0][3]) ?  4 : 0)
              + ((spxDist[0][1] > spxDist[0][3]) ?  2 : 0)
              + ((spxDist[0][2] > spxDist[0][3]) ?  1 : 0);

  // Now we can use constants::spxTrTab to set the coordinates in turn from the largest magnitude.
  // The number 3 is at the position of the largest coordinate.
  spxOffs[0][0] = constants::spxTrTab[traverse][0]>=3 ? 1 : 0;
  spxOffs[0][1] = constants::spxTrTab[traverse][1]>=3 ? 1 : 0;
  spxOffs[0][2] = constants::spxTrTab[traverse][2]>=3 ? 1 : 0;
  spxOffs[0][3] = constants::spxTrTab[traverse][3]>=3 ? 1 : 0;
  // The number 2 is at the second largest coordinate.
  spxOffs[1][0] = constants::spxTrTab[traverse][0]>=2 ? 1 : 0;
  spxOffs[1][1] = constants::spxTrTab[traverse][1]>=2 ? 1 : 0;
  spxOffs[1][2] = constants::spxTrTab[traverse][2]>=2 ? 1 : 0;
  spxOffs[1][3] = constants::spxTrTab[traverse][3]>=2 ? 1 : 0;
  // The number 1 is at the second smallest coordinate.
  spxOffs[2][0] = constants::spxTrTab[traverse][0]>=1 ? 1 : 0;
  spxOffs[2][1] = constants::spxTrTab[traverse][1]>=1 ? 1 : 0;
  spxOffs[2][2] = constants::spxTrTab[traverse][2]>=1 ? 1 : 0;
  spxOffs[2][3] = constants::spxTrTab[traverse][3]>=1 ? 1 : 0;
  // The fifth corner has all coordinate offsets = 1, so no need to look that up.

  // Distance from second corner
  spxDist[1][0] = spxDist[0][0] - spxOffs[0][0] + constants::spxSkew[2][1];
  spxDist[1][1] = spxDist[0][1] - spxOffs[0][1] + constants::spxSkew[2][1];
  spxDist[1][2] = spxDist[0][2] - spxOffs[0][2] + constants::spxSkew[2][1];
  spxDist[1][3] = spxDist[0][3] - spxOffs[0][3] + constants::spxSkew[2][1];

  // Distance from third corner
  spxDist[2][0] = spxDist[0][0] - spxOffs[1][0] + (2.0 * constants::spxSkew[2][1]);
  spxDist[2][1] = spxDist[0][1] - spxOffs[1][1] + (2.0 * constants::spxSkew[2][1]);
  spxDist[2][2] = spxDist[0][2] - spxOffs[1][2] + (2.0 * constants::spxSkew[2][1]);
  spxDist[2][3] = spxDist[0][3] - spxOffs[1][3] + (2.0 * constants::spxSkew[2][1]);

  // Distance from fourth corner
  spxDist[3][0] = spxDist[0][0] - spxOffs[2][0] + (3.0 * constants::spxSkew[2][1]);
  spxDist[3][1] = spxDist[0][1] - spxOffs[2][1] + (3.0 * constants::spxSkew[2][1]);
  spxDist[3][2] = spxDist[0][2] - spxOffs[2][2] + (3.0 * constants::spxSkew[2][1]);
  spxDist[3][3] = spxDist[0][3] - spxOffs[2][3] + (3.0 * constants::spxSkew[2][1]);

  // Distance from last corner
  spxDist[4][0] = spxDist[0][0] - 1.0 + (4.0 * constants::spxSkew[2][1]);
  spxDist[4][1] = spxDist[0][1] - 1.0 + (4.0 * constants::spxSkew[2][1]);
  spxDist[4][2] = spxDist[0][2] - 1.0 + (4.0 * constants::spxSkew[2][1]);
  spxDist[4][3] = spxDist[0][3] - 1.0 + (4.0 * constants::spxSkew[2][1]);

  // Permutated numbers, normalized to a range of 0 to 32
  spxGrads[0] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3]]]]] % 32;
  spxGrads[1] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3] + spxOffs[0][3]] + spxOffs[0][2]] + spxOffs[0][1]] + spxOffs[0][0]] % 32;
  spxGrads[2] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3] + spxOffs[1][3]] + spxOffs[1][2]] + spxOffs[1][1]] + spxOffs[1][0]] % 32;
  spxGrads[3] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3] + spxOffs[2][3]] + spxOffs[2][2]] + spxOffs[2][1]] + spxOffs[2][0]] % 32;
  spxGrads[4] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3] + 1] + 1] + 1] + 1] % 32;

  // Calculate the contribution from the four corners
  contrib = 0.6 - ::std::pow(spxDist[0][0], 2) - ::std::pow(spxDist[0][1], 2) - ::std::pow(spxDist[0][2], 2) - ::std::pow(spxDist[0][3], 2);
  spxCorn[0] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[0], spxDist[0][0], spxDist[0][1], spxDist[0][2], spxDist[0][3]) : 0.0;
  contrib = 0.6 - ::std::pow(spxDist[1][0], 2) - ::std::pow(spxDist[1][1], 2) - ::std::pow(spxDist[1][2], 2) - ::std::pow(spxDist[1][3], 2);
  spxCorn[1] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[1], spxDist[1][0], spxDist[1][1], spxDist[1][2], spxDist[1][3]) : 0.0;
  contrib = 0.6 - ::std::pow(spxDist[2][0], 2) - ::std::pow(spxDist[2][1], 2) - ::std::pow(spxDist[2][2], 2) - ::std::pow(spxDist[2][3], 2);
  spxCorn[2] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[2], spxDist[2][0], spxDist[2][1], spxDist[2][2], spxDist[2][3]) : 0.0;
  contrib = 0.6 - ::std::pow(spxDist[3][0], 2) - ::std::pow(spxDist[3][1], 2) - ::std::pow(spxDist[3][2], 2) - ::std::pow(spxDist[3][3], 2);
  spxCorn[3] = (contrib > 0.0) ? ::std::pow(contrib, 4) * getSimpDot(spxGrads[3], spxDist[3][0], spxDist[3][1], spxDist[3][2], spxDist[3][3]) : 0.0;

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  double result = 31.91239940056049206873467483092099 * (spxCorn[0] + spxCorn[1] + spxCorn[2] + spxCorn[3]);
  // Note: This factor has been found by searching the factor needed
  //       To get 1.0 with the largest result out of 100M iterations
  if (result >  1.0l) result =  1.0l;
  if (result < -1.0l) result = -1.0l;
  return (result);
}

} // End of namespace rng

} // End of namespace pwx

#endif // PWX_RANDOM_SIMPLEX_H
