//==========================================================================
//  AIDA Detector description implementation 
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
//==========================================================================

/*
 * StereoStrip.h
 *
 *  Created on: April 12, 2020
 *      Author: Whitney Armstrong, ANL
 */

#ifndef DDSegmentation_StereoStrip_H_
#define DDSegmentation_StereoStrip_H_

#include "DDSegmentation/CartesianStrip.h"

namespace dd4hep {
namespace DDSegmentation {

      /// Segmentation base class describing cartesian strip segmentation in X
class StereoStrip : public DDSegmentation::CartesianStrip {
   public:
    /// Default constructor passing the encoding string
    StereoStrip(const std::string& cellEncoding = "");
    /// Default constructor used by derived classes passing an existing decoder
    StereoStrip(const BitFieldCoder* decoder);
    /// destructor
    virtual ~StereoStrip();

    /// determine the position based on the cell ID
    virtual Vector3D position(const CellID& cellID) const;
    /// determine the cell ID based on the position
    virtual CellID cellID(const Vector3D& localPosition, const Vector3D& globalPosition,
                          const VolumeID& volumeID) const;
    /// access the strip size in X
    double stripSizeU() const { return _stripSizeU; }
    /// access the coordinate offset in X
    double offsetU() const { return _offsetU; }
    /// access the field name used for X
    const std::string& fieldNameU() const { return _uId; }
    /// set the strip size in X
    void setStripSizeU(double cellSize) { _stripSizeU = cellSize; }
    /// set the coordinate offset in X
    void setOffsetU(double offset) { _offsetU = offset; }
    /// set the field name used for X
    void setFieldNameU(const std::string& fieldName) { _uId = fieldName; }
    /** \brief Returns a vector<double> of the cellDimensions of the given cell ID
        in natural order of dimensions, e.g., dx/dy/dz, or dr/r*dPhi

        Returns a vector of the cellDimensions of the given cell ID
        \param cellID is ignored as all cells have the same dimension
        \return std::vector<double> size 1:
        -# size in x
    */
    virtual std::vector<double> cellDimensions(const CellID& cellID) const;

   protected:
    /// the U strip angle
    double _stripAngle;
    /// the strip size in X
    double _stripSizeU;
    /// the coordinate offset in X
    double _offsetU;
    /// the field name used for X
    std::string _uId;
};
}  // namespace DDSegmentation
} /* namespace dd4hep */
#endif  // DDSegmentation_StereoStrip_H_
