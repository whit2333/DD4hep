/*
 * StereoStrip.cpp
 *
 *  Created on: April 12, 2020
 *      Author: Whitney Armstrong, ANL
 *              
 */

#include "DDSegmentation/StereoStrip.h"
#include "DD4hep/Objects.h"

namespace dd4hep {
namespace DDSegmentation {
/// default constructor using an encoding string
StereoStrip::StereoStrip(const std::string& cellEncoding) : CartesianStrip(cellEncoding) {
    // define type and description
    _type = "StereoStrip";
    _description = "Strip segmentation U rotated some angle from local X axis";

    // register all necessary parameters
    registerParameter("strip_angle", "Strip stereo angle in U direction", _stripAngle, 1., SegmentationParameter::AngleUnit);
    registerParameter("strip_size", "Cell size in U", _stripSizeU, 1., SegmentationParameter::LengthUnit);
    registerParameter("offset_u", "Cell offset in u", _offsetU, 0., SegmentationParameter::LengthUnit, true);
    registerIdentifier("identifier_u", "Cell ID identifier for U", _uId, "u");
}

/// Default constructor used by derived classes passing an existing decoder
StereoStrip::StereoStrip(const BitFieldCoder* decode) : CartesianStrip(decode) {
    // define type and description
    _type = "StereoStrip";
    _description = "Cartesian segmentation on the local X axis";

    // register all necessary parameters
    registerParameter("strip_angle", "Strip stereo angle in U direction", _stripAngle, 1., SegmentationParameter::AngleUnit);
    registerParameter("strip_size", "Cell size in U", _stripSizeU, 1., SegmentationParameter::LengthUnit);
    registerParameter("offset_u", "Cell offset in U", _offsetU, 0., SegmentationParameter::LengthUnit, true);
    registerIdentifier("identifier_u", "Cell ID identifier for U", _uId, "u");
}

/// destructor
StereoStrip::~StereoStrip() {}

/// determine the position based on the cell ID
Vector3D StereoStrip::position(const CellID& cID) const {
    Position cellPosition;
    cellPosition.SetX( binToPosition(_decoder->get(cID, _uId), _stripSizeU, _offsetU));
    cellPosition = RotationZ(_stripAngle)*cellPosition;
    return cellPosition;
}

/// determine the cell ID based on the position
CellID StereoStrip::cellID(const Vector3D& localPosition, const Vector3D& /* globalPosition */,
                               const VolumeID& vID) const {
    CellID cID = vID;
    _decoder->set(cID, _uId, positionToBin((RotationZ(-1.0*_stripAngle)*localPosition).X, _stripSizeU, _offsetU));
    return cID;
}

std::vector<double> StereoStrip::cellDimensions(const CellID&) const {
#if __cplusplus >= 201103L
    return {_stripSizeU};
#else
    std::vector<double> cellDims(1, 0.0);
    cellDims[0] = _stripSizeU;
    return cellDims;
#endif
}

REGISTER_SEGMENTATION(StereoStrip)
}  // namespace DDSegmentation
} /* namespace dd4hep */
