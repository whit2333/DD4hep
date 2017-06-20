//==========================================================================
//  AIDA Detector description implementation 
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
#ifndef DD4HEP_GEOMETRY_SEGMENTATIONSINTERNA_H
#define DD4HEP_GEOMETRY_SEGMENTATIONSINTERNA_H

// Framework include files
#include "DD4hep/Handle.h"
#include "DD4hep/Objects.h"
#include "DD4hep/BitField64.h"
#include "DDSegmentation/Segmentation.h"

// C/C++ include files

/// Namespace for the AIDA detector description toolkit
namespace dd4hep {

  // Forward declarations
  class DetElementObject;
  class SegmentationObject;
  class SensitiveDetectorObject;

  /// Implementation class supporting generic Segmentation of sensitive detectors
  /**
   *  The SegmentationObject wraps the functionality of the DDSegmentation base class.
   *
   *  \author  M.Frank
   *  \version 1.0
   *  \ingroup DD4HEP_GEOMETRY
   */
  class SegmentationObject {
  public:
    /// Standard constructor
    SegmentationObject(DDSegmentation::Segmentation* s = 0);
    /// Default destructor
    virtual ~SegmentationObject();
    /// Access the encoding string
    std::string fieldDescription() const;
    /// Access the segmentation name
    const std::string& name() const;
    /// Set the segmentation name
    void setName(const std::string& value);

    /// Access the segmentation type
    const std::string& type() const;
    /// Access the description of the segmentation
    const std::string& description() const;
    /// Access the underlying decoder
    const BitField64* decoder() const;
    /// Set the underlying decoder
    void setDecoder(BitField64* decoder) const;
    /// Access to parameter by name
    DDSegmentation::Parameter  parameter(const std::string& parameterName) const;
    /// Access to all parameters
    DDSegmentation::Parameters parameters() const;
    /// Set all parameters from an existing set of parameters
    void setParameters(const DDSegmentation::Parameters& parameters);

    /** Segmentation interface  */
    /// Determine the local position based on the cell ID
    Position position(const CellID& cellID) const;
    /// Determine the cell ID based on the position
    CellID cellID(const Position& localPosition,
                  const Position& globalPosition,
                  const VolumeID& volumeID) const;
    /// Determine the volume ID from the full cell ID by removing all local fields
    VolumeID volumeID(const CellID& cellID) const;
    /// Calculates the neighbours of the given cell ID and adds them to the list of neighbours
    void neighbours(const CellID& cellID, std::set<CellID>& neighbours) const;

    /** Data members.                                          */
    /// Magic word to check object integrity
    unsigned long magic;
    /// Flag to use segmentation for hit positioning
    unsigned char useForHitPosition;
    /// Reference to hosting top level DetElement structure
    Handle<DetElementObject> detector;      
    /// Reference to hosting top level sensitve detector structure
    Handle<SensitiveDetectorObject> sensitive;
    /// Reference to base segmentation
    DDSegmentation::Segmentation* segmentation;      
  };

  /// Concrete wrapper class for segmentation implementation based on DDSegmentation objects
  /**
   * \author  M.Frank
   * \version 1.0
   * \ingroup DD4HEP_GEOMETRY
   */
  template <typename IMP> class SegmentationWrapper : public SegmentationObject {
  public:
    /// DDSegmentation aggregate
    IMP* implementation;
  public:
    /// Standard constructor
    SegmentationWrapper(BitField64* decoder);
    /// Default destructor
    virtual ~SegmentationWrapper();
  };

  /// Standard constructor
  template <typename IMP> inline
  SegmentationWrapper<IMP>::SegmentationWrapper(BitField64* decode)
    :  SegmentationObject(implementation=new IMP(decode))
  {
  }

  /// Default destructor
  template <typename IMP> inline SegmentationWrapper<IMP>::~SegmentationWrapper()  {
  }
}         /* End namespace dd4hep                       */
#endif    /* DD4HEP_GEOMETRY_SEGMENTATIONSINTERNA_H     */
