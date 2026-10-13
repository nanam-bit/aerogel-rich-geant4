
## Introduction

Ring Imaging Cherenkov (RICH) Detectors identify electrically charged subatomic particles by detecting the cone of optical photons emitted by the particles as it travels faster than the speed of light in a certain medium.

This is overall a simple setup to demonstrate and simulate the generation of the characteristic ring pattern of the generated photons.

## Geometry

For this simulation, we assume that some particle travels in a straight line along the z-axis through an aerogel radiator of some thickness, emitting photons uniformly along its path, where those photons project onto a flat detector array (SiC) located at a distance. See richDetectorConstruction.cc in the src folder for geometry details.

## Build Instructions

To build this project, install Geant4 and CMake. Use the CMakeLists.txt to build the project and run the executable file. The project can be built the same way as Geant4 example projects. 

## Data Output

The ROOT file output (found in DataOutput folder) contains data on photon coordinates and energy deposition.

## Analysis

The ROOTanalysis.py file is meant for output analysis using uproot. This generates a 3D plot of the photon hits using the optical photon coordinates and plots energy deposition distribution. Since this project tracks G4OpticalPhotons, the energy deposition is zero (and is killed after hitting the detector). 

