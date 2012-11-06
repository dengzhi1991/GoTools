#ifndef LRBSPLINE2DUTILS_H
#define LRBSPLINE2DUTILS_H

#include <vector>
#include <set>
#include "GoTools/utils/checks.h"
#include "GoTools/lrsplines2D/Mesh2D.h"
#include "GoTools/lrsplines2D/LRBSpline2D.h"


namespace Go
{

namespace LRBSpline2DUtils
{
  // Generates the two LRBSpline2Ds ('new_1', 'new_2') that results
  // from splitting the LRBSpline2D 'orig' in direction 'd', along
  // the knotvalue referenced by 'new_knot_ix'.  Since the LRBSpline2Ds
  // only contain indices to knots, not the knotvals themselves, it is 
  // necessary to supply the knotvals of the concerned direction explicitly 
  // by the two arrays pointed to by 'knotvalues'
  void split_function(const LRBSpline2D& orig, 
		      Direction2D d, 
		      const double* const knotvalues,
		      int new_knot_ix,
		      LRBSpline2D& new_1,
		      LRBSpline2D& new_2);


  // Recursively splits a B-spline function 'b_orig' with support in 'mesh' into new
  // B-spline functions, until the new B-spline functions all have minimal support 
  // in the mesh.  Resulting B-spline functions are returned in the 'result' vec, which
  // is _not_ cleared before execution (the function calls itself recursively and fills
  // up the result vector gradually).
  // 
  // It is assumed that the B-spline function 'b_orig' has a support in 'mesh', otherwise
  // the function throws.
  // 
  // @@ NB: In some cases (particularly for higher orders, such as cubic), the number of 
  // intermediately generated b-spline-functions can be immense!  This function should 
  // therefore generally be avoided!  It is provided here primarily for reference.
  void recursively_split(const LRBSpline2D& b_orig,
			 const Mesh2D& mesh,
			 std::vector<LRBSpline2D>& result);


// if 'b' can be split at least once in the mesh 'm', split it once, and return the 
// result through 'b1' and 'b2'.  The function never carries out more than one split, 
// even when several splits are possible.
bool try_split_once(const LRBSpline2D& b, const Mesh2D& mesh, LRBSpline2D& b1, LRBSpline2D& b2);

// Comparison functor which only takes into account the support of the LRBSpline2Ds, 
// not the coefficient or gamma.  This functor is sometimes useful when generating
// STL structures that keep track of LRBSpline2Ds with unique support.
//==============================================================================
struct support_compare
//==============================================================================
{
  bool operator()(const LRBSpline2D& b1, const LRBSpline2D& b2) const  {
    // to compare b1 and b2, compare the x-knotvectors.  If these are identical, compare
    // the y-knotvectors instead.
    const int tmp1 = compare_seq(b1.kvec(XFIXED).begin(), b1.kvec(XFIXED).end(),
				 b2.kvec(XFIXED).begin(), b2.kvec(XFIXED).end());
    if (tmp1 != 0) return (tmp1 < 0);
    const int tmp2 = compare_seq(b1.kvec(YFIXED).begin(), b1.kvec(YFIXED).end(),
				 b2.kvec(YFIXED).begin(), b2.kvec(YFIXED).end());
    return (tmp2 < 0);

  }
}; 

}; // End namespace LRBSpline2DUtils
}; // End namespace Go
#endif