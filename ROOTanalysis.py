import uproot
import numpy as np
import matplotlib.pyplot as plt

##### extracting basic information #####
# Open ROOT file -- the root file is located in DataOutput file
file = uproot.open("rich_sim_output_t0.root")

# Inspect contents
print("File contents:")
print(file.keys())

# Get the TTree
tree = file["PhotonHits"]

# Inspect tree
print("\nBranches:")
tree.show()

# Load all branches
data = tree.arrays(library="np")

# Extract individual quantities
x = data["X"]
y = data["Y"]
z = data["Z"]
edep = data["Edep"]

# Basic information
print("\nNumber of hits:", len(x))

print("X range:", x.min(), "to", x.max())
print("Y range:", y.min(), "to", y.max())
print("Z range:", z.min(), "to", z.max())

print("Total deposited energy:", edep.sum())
print("Mean deposited energy:", edep.mean())

##### 3D plot of the hits #####

data = tree.arrays(["X", "Y", "Z"], library="np")

x = data["X"]
y = data["Y"]
z = data["Z"]

fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")

ax.scatter(x, y, z, s=1)

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")

plt.show()

##### plot of energy distribution #####
# this should just be zero since it is tracking optical photons
# which are killed upon detection (see SensitiveDetector.cc in src file)
# so this part is just to make sure it is in fact working as intended.
# You could also just see that edep = 0 for all instances but 
# in the future if I want to change it to track other particles
# or just not have the fStopAndKill Command

edep = tree["Edep"].array(library="np")

plt.hist(edep, bins=100)
plt.xlabel("Deposited Energy")
plt.ylabel("Number of Hits")
plt.show()

