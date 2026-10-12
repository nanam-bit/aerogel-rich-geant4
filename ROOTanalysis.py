import uproot
import numpy as np
import matplotlib.pyplot as plt

##### extracting basic information #####
# Open ROOT file
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

edep = tree["Edep"].array(library="np")

plt.hist(edep, bins=100)
plt.xlabel("Deposited Energy")
plt.ylabel("Number of Hits")
plt.show()

##### checking zero-valued energies ####
# this is worth investigating -- not sure why all entries are zero. Should check source code
print("Number of entries:", len(edep))
print("Minimum:", np.min(edep))
print("Maximum:", np.max(edep))
print("Mean:", np.mean(edep))
print("Total:", np.sum(edep))

print("Number exactly zero:", np.sum(edep == 0))
print("Number nonzero:", np.sum(edep != 0))

# Print some values in scientific notation
for value in edep[:20]:
    print(f"{value:.8e}")
