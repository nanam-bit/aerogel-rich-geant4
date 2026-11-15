
## Introduction

Ring Imaging Cherenkov (RICH) Detectors identify electrically charged subatomic particles by detecting the cone of optical photons emitted by the particles as it travels faster than the speed of light in a certain medium.

The objective of this project is to simulate the characteristic ring pattern of the photons and compare the simulation results against theory. 

## Theory

A charged particle with speed $\beta c$ in a medium of refractive index $n$ emits light at

$$\cos\theta_c = \frac{1}{n\beta}$$

Measuring $\theta_c$ gives $\beta$, and with a known momentum this identifies the particle. For a pion with $T = 3$ GeV, $m = 139.57$ MeV/$c^2$ and $n = 1.05$:

$$E = T + m = 3139.57\ \text{MeV},\quad \beta = \sqrt{1-(m/E)^2} = 0.99901,\quad \theta_c = 17.57^\circ$$

Photons leave the aerogel (index $n$) into vacuum, so Snell's law bends them outward:

$$\sin\theta_{out} = n\sin\theta_c \;\Rightarrow\; \theta_{out} = 18.48^\circ$$

A photon emitted at depth $z_e$ below the radiator entry face lands at

$$r = (T-z_e)\tan\theta_c + D\tan\theta_{out}$$

where $T = 50$ mm is the radiator thickness and $D = 274$ mm is the gap from the radiator exit face to the detector (see `DetectorConstruction.cc` for setup details). Because photons are emitted all along the track, the ring has a finite width:

$$r_{min} = D\tan\theta_{out} = 91.6\ \text{mm},\qquad r_{max} = r_{min} + T\tan\theta_c = 107.4\ \text{mm}$$

The Frank-Tamm formula gives the number of photons emitted per unit length:

$$\frac{dN}{dx} \approx 370\,\sin^2\theta_c\ \Delta E\ \ \text{(photons per cm, with } \Delta E \text{ in eV)}$$

With $\sin^2\theta_c = 0.0912$ and $\Delta E = 2$ eV (the 2 to 4 eV window), $dN/dx = 67.5$ photons/cm, or **337 photons** over 5 cm. Aerogel absorption (100 cm absorption length) removes about 2.6% on average, leaving **about 329** photons reaching the detector.

## Simulation Setup

For this simulation, a particle travels in a straight line along the z-axis through an aerogel radiator, emitting photons uniformly along its path, where those photons project onto a flat SiC detector array located at a distance (appx. 30 cm) away (see richDetectorConstruction.cc in src file).

| Item | Value |
|---|---|
| Radiator | Silica aerogel, 100 x 100 x 50 mm, $n = 1.05$ (constant), absorption length 100 cm |
| Detector | SiC plane, 300 x 300 x 2 mm, gap $D$ = 274 mm from radiator exit |
| Photon energy range | 2 to 4 eV (about 310 to 620 nm) |
| Quantum efficiency | 25% / 35% / 25% at 2 / 3 / 4 eV (average 30%) |
| Detector surface | `dielectric_metal`, reflectivity 0, so a photon is either detected (QE) or absorbed |
| Beam | $\pi^-$, 3 GeV kinetic energy, along +z, 1000 events |
| Physics | `FTFP_BERT` + `G4OpticalPhysics` (Cherenkov on, scintillation off) |

Photons that are detected are killed at the detector boundary, so a sensitive detector attached to the SiC volume would never see them. Instead, `SteppingAction` counts every photon crossing into the detector (arrivals) and records a hit when `G4OpBoundaryProcess` reports `Detection`. The hit position is the point on the detector face. 

## Data Output

| Quantity | Theory | Simulation |
|---|---|---|
| Photons reaching detector per event | 328.7 | 329.5 |
| Hits / arrivals | 0.300 (mean of the QE curve) | 0.300 |
| Ring radius range | 91.6 to 107.4 mm | ring contained in this range |
| Mean hit radius | 99.5 mm (midpoint) | 99.5 mm |

- The photon yield agrees with Frank-Tamm plus absorption to within 0.3%.
- The measured detection fraction matches the average quantum efficiency.
- The ring sits inside the predicted radius band and the detector fully contains it (the ring ends about 43 mm from the detector edge).
- A small number of hits (43 of 98,702, or 0.04%) land beyond 150 mm. These are likely Cherenkov photons from electrons knocked out of the aerogel.
- The 15.8 mm ring width comes from the 5 cm radiator thickness (emission point smearing), and it is the main limit on angular resolution in this design.

## Analysis

The `ROOTanalysis.py` file is meant for simple output analysis using uproot. This generates a 3D plot of the photon hits using the optical photon coordinates and plots energy deposition distribution. Since this project tracks G4OpticalPhotons, the energy deposition is zero (and is killed after hitting the detector). The  `ROOTanalysis_UPDATED.py` is the updated version that calculates the detection fraction (Hits/Arrivals), Cherenkov angle, ring and hit radius, and hits beyond the edge region. Both require the Python package `uproot`. If the geometry or constants are changed, 

The output is written to one file, `rich_sim_output.root` with two trees: `PhotonHits`, which contains X,Y,Z, and energy for each detected photon and `Events`, which contains teh number of arrivals `nArrivals` and number of hits `nHits` per event.

The exact output of `ROOTanalysis_UPDATED.py` is below, for `rich_sim_output.root`: 

Objects in file: ['PhotonHits;1', 'Events;1']

Events:               1000
Photons at detector:  329517  (329.5 per event)
Detected hits:        98702  (98.7 per event)
Hit rows in ntuple:   98702  (should equal detected hits)
Hits / arrivals:      0.300  (expect ~0.3 from the efficiency curve)

Analytic theta_c = 17.57 deg, ring radius 91.6-107.4 mm
Simulated hit radius: mean 99.5 mm, std 5.5 mm, max 208.8 mm
Hits beyond detector edge region (r > 150.0 mm): 43

## Build Instructions

To build this project, Geant4 (with data files) and CMake are required. Use the CMakeLists.txt to build the project and run the executable file. The project can be built the same way as Geant4 example projects. The file `run.mac` sets the particle's energy and runs the events, setting particle as pi-, energy 3 GeV, and 1000 runs. Note that the 3 GeV is kinetic energy, not momentum. The file `init_vis.mac` and `vis.mac` can be run to view the geometry, and screenshots of the visuals are shown in DataOutput. 


## Updates to be made / Limitations

Chromatic dispersion (where refractive index n of a radiator medium changes with the wavelength of the emitted photon) is not modeled since the refractive index is held constant. The emission angle is governed by $$\cos\theta_c = \frac{1}{n\beta}$$, so different wavelengths of light are emitted at slightly different angles for the same particle velocity. Therefore, the angular resolution may be affected. 


