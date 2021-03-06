"""
   dd4hep example setup using the python configuration

   \author  M.Frank
   \version 1.0

"""
from __future__ import absolute_import, unicode_literals
import logging
import sys


logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)


def help():
  logging.info("Check_shape.py -option [-option]                           ")
  logging.info("       -geometry   <geometry file>   Geometry file         ")
  logging.info("       -vis                          Enable visualization  ")
  logging.info("       -batch                        Batch execution       ")


def run():
  geo = None
  vis = False
  batch = False
  for i in xrange(len(sys.argv)):
    c = sys.argv[i].upper()
    if c.find('BATCH') < 2 and c.find('BATCH') >= 0:
      batch = True
    if c[:4] == '-GEO':
      geo = sys.argv[i + 1]
    if c[:4] == '-VIS':
      vis = True

  if not geo:
    help()
    sys.exit(1)

  import DDG4
  kernel = DDG4.Kernel()
  # Configure UI
  geant4 = DDG4.Geant4(kernel, tracker='Geant4TrackerCombineAction')
  ui = geant4.setupCshUI(ui=None, vis=vis)
  if batch:
    kernel.UI = ''

  kernel.loadGeometry(geo)
  # Configure field
  geant4.setupTrackingField(prt=True)
  # Now build the physics list:
  geant4.setupPhysics('')
  kernel.physicsList().enableUI()
  DDG4.setPrintLevel(DDG4.OutputLevel.DEBUG)
  #
  #      '/ddg4/ConstructGeometry/DumpGDML   test.gdml',
  #      '/ddg4/ConstructGeometry/writeGDML',
  ui.Commands = [
      '/ddg4/ConstructGeometry/VolumePath /world_volume_1/Shape_Test_0/Shape_Test_vol_0_0',
      '/ddg4/ConstructGeometry/printVolume',
      'exit'
      ]
  kernel.NumEvents = 0
  kernel.configure()
  kernel.initialize()
  kernel.run()
  kernel.terminate()


if __name__ == "__main__":
  run()
