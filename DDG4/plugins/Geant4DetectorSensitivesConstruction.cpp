//==========================================================================
//  AIDA Detector description implementation 
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
//  \author Markus Frank
//  \date   2015-11-09
//
//==========================================================================

// Framework include files
#include "DDG4/Geant4DetectorConstruction.h"

/// Namespace for the AIDA detector description toolkit
namespace dd4hep {

  /// Namespace for the Geant4 based simulation part of the AIDA detector description toolkit
  namespace sim {

    /// Class to create Geant4 detector geometry from TGeo representation in memory
    /**
     *  On demand the sensitive detectors are created and attached to all sensitive
     *  volumes. The relevant  callback is executed when the call to 
     *  ConstructSDandField() of the corresponding G4VUserDetectorConstruction
     *  instance is called. The call is thread-local!
     *
     *  \author  M.Frank
     *  \version 1.0
     *  \ingroup DD4HEP_SIMULATION
     */
    class Geant4DetectorSensitivesConstruction : public Geant4DetectorConstruction   {
    public:
      /// Initializing constructor for DDG4
      Geant4DetectorSensitivesConstruction(Geant4Context* ctxt, const std::string& nam);
      /// Default destructor
      virtual ~Geant4DetectorSensitivesConstruction();
      /// Sensitives construction callback. Called at "ConstructSDandField()"
      void constructSensitives(Geant4DetectorConstructionContext* ctxt);
    };
  }    // End namespace sim
}      // End namespace dd4hep


// Framework include files
#include "DD4hep/InstanceCount.h"
#include "DD4hep/Printout.h"
#include "DD4hep/Plugins.h"
#include "DD4hep/Detector.h"

#include "DDG4/Geant4Mapping.h"
#include "DDG4/Factories.h"

// ROOT include files
#include "TGeoManager.h"
// Geant4 include files
#include "G4SDManager.hh"
#include "G4PVPlacement.hh"
#include "G4VSensitiveDetector.hh"

using namespace std;
using namespace dd4hep;
using namespace dd4hep::sim;

DECLARE_GEANT4ACTION(Geant4DetectorSensitivesConstruction)

/// Initializing constructor for other clients
Geant4DetectorSensitivesConstruction::Geant4DetectorSensitivesConstruction(Geant4Context* ctxt, const string& nam)
  : Geant4DetectorConstruction(ctxt,nam)
{
  InstanceCount::increment(this);
}

/// Default destructor
Geant4DetectorSensitivesConstruction::~Geant4DetectorSensitivesConstruction() {
  InstanceCount::decrement(this);
}

/// Sensitive detector construction callback. Called at "ConstructSDandField()"
void Geant4DetectorSensitivesConstruction::constructSensitives(Geant4DetectorConstructionContext* ctxt)   {
  Geant4GeometryInfo* p = Geant4Mapping::instance().ptr();
  for(const auto& iv : p->sensitives )  {
    SensitiveDetector sd = iv.first;
    string typ = sd.type(), nam = sd.name();
    G4VSensitiveDetector* g4sd = 
      PluginService::Create<G4VSensitiveDetector*>(typ, nam, &ctxt->description);
    if (!g4sd) {
      string tmp = typ;
      tmp[0] = ::toupper(tmp[0]);
      typ = "Geant4" + tmp;
      g4sd = PluginService::Create<G4VSensitiveDetector*>(typ, nam, &ctxt->description);
      if ( !g4sd ) {
        PluginDebug dbg;
        g4sd = PluginService::Create<G4VSensitiveDetector*>(typ, nam, &ctxt->description);
        if ( !g4sd )  {
          throw runtime_error("ConstructSDandField: FATAL Failed to "
                              "create Geant4 sensitive detector " + nam + 
                              " of type " + typ + ".");
        }
      }
    }
    g4sd->Activate(true);
    G4SDManager::GetSDMpointer()->AddNewDetector(g4sd);
    for(const TGeoVolume* vol : iv.second )  {
      G4LogicalVolume* g4v = p->g4Volumes[vol];
      if ( !g4v )  {
        throw runtime_error("ConstructSDandField: Failed to access G4LogicalVolume for SD "+
                            nam + " of type " + typ + ".");
      }
      ctxt->setSensitiveDetector(g4v,g4sd);
    }
  }
  print("Geant4Converter", "++ Handled %ld sensitive detectors.",p->sensitives.size());
}
