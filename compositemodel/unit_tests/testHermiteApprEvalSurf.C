/*
 * Copyright (C) 1998, 2000-2007, 2010, 2011, 2012, 2013 SINTEF ICT,
 * Applied Mathematics, Norway.
 *
 * Contact information: E-mail: tor.dokken@sintef.no                      
 * SINTEF ICT, Department of Applied Mathematics,                         
 * P.O. Box 124 Blindern,                                                 
 * 0314 Oslo, Norway.                                                     
 *
 * This file is part of GoTools.
 *
 * GoTools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. 
 *
 * GoTools is distributed in the hope that it will be useful,        
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with GoTools. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In accordance with Section 7(b) of the GNU Affero General Public
 * License, a covered work must retain the producer line in every data
 * file that is created or manipulated using GoTools.
 *
 * Other Usage
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the GoTools library without
 * disclosing the source code of your own applications.
 *
 * This file may be used in accordance with the terms contained in a
 * written agreement between you and SINTEF ICT. 
 */

#define BOOST_TEST_MODULE gotools-core/testHermiteApprEvalSurf
#include <boost/test/included/unit_test.hpp>

#include <fstream>
#include "GoTools/compositemodel/OffsetSurfaceUtils.h"
#include "GoTools/geometry/ObjectHeader.h"

using namespace Go;
using std::vector;
using std::ifstream;
using std::string;

BOOST_AUTO_TEST_CASE(testHermiteApprEvalSurf)
{

    vector<string> filenames;
    filenames.push_back("data/square.g2"); // Trivial, unit square for z = 0.0, bilinear.
    filenames.push_back("data/spline_surface_1.g2"); // Bicubic 18x17, almost flat.
    // filenames.push_back("data/test_bezier.g2"); // Tricky case with self-intersections for offset dist of appr 0.3 and larger.

    for (size_t kk = 0; kk < filenames.size(); ++kk)
    {
        ifstream infile(filenames[kk]);
        if (!(infile.good()))
        {
            BOOST_ERROR("Input file not found or file corrupted!");
            continue;
        }
    
        ObjectHeader header;
        infile >> header;
        if (header.classType() != Class_SplineSurface)
        {
            BOOST_ERROR("Input was not a SplineSurface, not yet supported!");
            continue;
        }

        shared_ptr<ParamSurface> sf(new SplineSurface());
        sf->read(infile);

        const double offset = 1.23; //0.2;
        const double epsgeo = 1e-06;
        shared_ptr<SplineSurface> offset_sf;
        vector<shared_ptr<ParamSurface> > sfs;
        sfs.push_back(sf);
        
        int status = OffsetSurfaceUtils::offsetSurfaceSet(sfs, offset, epsgeo, offset_sf);
        BOOST_CHECK_EQUAL(status, 0);
        
        const RectDomain& rect_dom = sf->containingDomain();
        const int num_samples = 73;
        const double umin = rect_dom.umin();
        const double vmin = rect_dom.vmin();
        const double umax = rect_dom.umax();
        const double vmax = rect_dom.vmax();
        const double ustep = (umax - umin)/(double)(num_samples - 1);
        const double vstep = (vmax - vmin)/(double)(num_samples - 1);
        double max_error = -1.0;
        double max_u, max_v;
        for (size_t kj = 0; kj < num_samples; ++kj)
        {
            double vpar = vmin + (double)kj*vstep;
            for (size_t ki = 0; ki < num_samples; ++ki)
            {
                double upar = umin + (double)ki*ustep;
                Point base_pt = sf->point(upar, vpar);
                Point offset_pt = offset_sf->ParamSurface::point(upar, vpar);
                double dist = base_pt.dist(offset_pt);
                double error = fabs(dist - offset);
                if (error > max_error)
                {
                    max_error = error;
                    max_u = upar;
                    max_v = vpar;
                }
                const bool disable_test = false;
                if (!disable_test)
                {
                    BOOST_CHECK_LE(error, epsgeo); // Checking if error <= epsgeo.
                }
            }
        }
        std::cout << "max_error: " << max_error << ", max_u: " << max_u << ", max_v: " << max_v << std::endl;

        std::ofstream fileout2("tmp/testHermiteApprEvalSurf_input.g2");
        sf->writeStandardHeader(fileout2);
        sf->write(fileout2);

        std::ofstream fileout("tmp/testHermiteApprEvalSurf_result.g2");
        offset_sf->writeStandardHeader(fileout);
        offset_sf->write(fileout);
    }
}
