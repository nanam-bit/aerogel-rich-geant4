"""Quick look at the Geant4 RICH output using uproot.

Usage:  python read_rich_output.py [rich_sim_output.root]
Requires: pip install uproot awkward numpy matplotlib

Geant4 units in the file: positions in mm, energy in eV (as filled).
"""
import sys
import numpy as np
import uproot
import matplotlib.pyplot as plt

fname = sys.argv[1] if len(sys.argv) > 1 else "rich_sim_output.root"

# ---- geometry (mm) from DetectorConstruction.cc; edit if you change it ----
N_AEROGEL = 1.05
THICKNESS = 50.0    # radiator thickness
GAP = 274.0         # radiator exit face (z=-125 mm) to detector face (z=149 mm)
DET_HALF = 150.0    # detector half-width

with uproot.open(fname) as f:
    print("Objects in file:", f.keys())
    hits = f["PhotonHits"].arrays(library="np")
    events = f["Events"].arrays(library="np")

x, y, E = hits["X"], hits["Y"], hits["energy"]
r = np.hypot(x, y)

n_events = len(events["nArrivals"])
n_arr = events["nArrivals"].sum()
n_hit = events["nHits"].sum()

print(f"\nEvents:               {n_events}")
print(f"Photons at detector:  {n_arr}  ({n_arr / n_events:.1f} per event)")
print(f"Detected hits:        {n_hit}  ({n_hit / n_events:.1f} per event)")
print(f"Hit rows in ntuple:   {len(x)}  (should equal detected hits)")

# see DetectorConstruction.cc where the efficiency is defined
if n_arr > 0:
    print(f"Hits / arrivals:      {n_hit / n_arr:.3f}  (expect ~0.3 from the efficiency curve)")

# ---- ring radii for beta = .99901 for 3 GeV Pion ----
theta_c = np.arccos(1.0 / (N_AEROGEL*.99901))
theta_out = np.arcsin(N_AEROGEL * np.sin(theta_c))
r_min = GAP * np.tan(theta_out)
r_max = r_min + THICKNESS * np.tan(theta_c)
print(f"\nAnalytic theta_c = {np.degrees(theta_c):.2f} deg, ring radius {r_min:.1f}-{r_max:.1f} mm")
if len(r):
    print(f"Simulated hit radius: mean {r.mean():.1f} mm, std {r.std():.1f} mm, max {r.max():.1f} mm")
    print(f"Hits beyond detector edge region (r > {DET_HALF} mm): {(r > DET_HALF).sum()}")

# ---- plots ----
fig, ax = plt.subplots(1, 3, figsize=(16, 4.8))

# hit map (to see ring)
ax[0].hist2d(x, y, bins=120, cmap="viridis")
ax[0].set_aspect("equal")
ax[0].set(title="Hit map", xlabel="x [mm]", ylabel="y [mm]")

# hit radius 
ax[1].hist(r, bins=100, color="C0")
ax[1].axvspan(r_min, r_max, color="C3", alpha=0.25, label="analytic range")
ax[1].axvline(DET_HALF, color="k", ls="--", label="detector edge")
ax[1].set(title="Hit radius", xlabel="r [mm]", ylabel="hits")
ax[1].legend()

# detected photon energy distribution
ax[2].hist(E, bins=60, color="C2")
ax[2].set(title="Detected photon energy", xlabel="E [eV]", ylabel="hits")

plt.tight_layout()
plt.savefig("rich_overview.png", dpi=150)
plt.show()
