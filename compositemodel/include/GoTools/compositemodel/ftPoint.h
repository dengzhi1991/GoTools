//===========================================================================
//                                                                           
// File: ftPoint.h                                                           
//                                                                           
// Created: Thu Mar 23 11:32:48 2000                                         
//                                                                           
// Author: Atgeirr F Rasmussen <atgeirr@sintef.no>
//                                                                           
// Revision: $Id: ftPoint.h,v 1.2 2008-04-02 07:18:28 vsk Exp $
//                                                                           
// Description: ftPoint class
//                                                                           
//===========================================================================

#ifndef _FTPOINT_H
#define _FTPOINT_H


#include "GoTools/utils/Point.h"

namespace Go
{

class ftSurface;

//===========================================================================
/** ftPoint - represents a point, possibly lying on a surface
 * ftPoint contains representations for both a 3D point in space
 * and possibly also parameters for the point on a specific surface.
 *
 * \author Atgeirr F Rasmussen <atgeirr@sintef.no>
 */
//===========================================================================
class ftPoint
{
protected:
    Point pt_;              // The point
    ftSurface* surface_;         // The surface on which it lies (if nonzero)
    double u_, v_;            // Coordinates on that surface

public:
    /// Constructor
    /// \param pt Point
    /// \param sf Associated surface
    /// \param u First parameter in surface corresponding to point
    /// \param v Second parameter in surface corresponding to point
    ftPoint(Point pt, ftSurface* sf = 0, double u = 0, double v = 0)
	: pt_(pt), surface_(sf), u_(u), v_(v) {}
    /// Constructor. Point in 3d given by space coordinates
    ftPoint(double x, double y, double z)
	: pt_(x, y, z), surface_(0), u_(0), v_(0) {}
    /// Position of point
    const Point& position() const { return pt_; }
    /// Associated face/surface
    ftSurface* face() const { return surface_; }
    /// 1. parameter in surface
    double u() const { return u_; }
    /// 2. parameter in surface
    double v() const { return v_; }
    /// Normal of surface in point
    Point normal() const;
};

} // namespace Go


#endif // _FTPOINT_H

