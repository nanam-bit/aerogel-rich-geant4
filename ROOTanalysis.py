# figuring out how to use uproot

# import headers
import uproot
import numpy as np
import matplotlib.pyplot as plt

# open a ROOT file for reading
file = uproot.open("C:\\Users\\nanak\\Geant4\\projdirec\\P1_cherenkov\\rich_sim_output_t0.root")

## print(file.keys()) # returns [PhotonHits;1]

tree = file["PhotonHits"]
print(tree.keys())
tree.show()

## file.keys() returns
##['hits per event;1',    'hits above threshold;1',
## 'Cerenkov;1',          'absorbed;1',
## 'boundary absorbed;1', 'E dep;1']
##

##hist = file["E dep"]
##
### hist.all_members --> prints all identification
##histName = hist.member("fName")
##
##values = hist.values() # bin values (y-axis)
##edges = hist.axis().edges() # bin edges (x-axis)
##errors = hist.errors()

##plt.stairs(values, edges)
##plt.title(histName)
##plt.show()


# --------------------------------------------------------------------------------------#
# --------------------------------------------------------------------------------------#
# --------------------------------------------------------------------------------------#

##fig, axs = plt.subplots(3,2)
##axs[0,0].stairs(values, edges)
##axs[0,1].stairs(values, edges)
##axs[1,0].stairs(values, edges)
##axs[1,1].stairs(values, edges)
##axs[2,0].stairs(values, edges)
##axs[2,1].stairs(values, edges)
##
##for ax in axs.flat:
##    ax.set(xlabel='x-label', ylabel='y-label')
##
####for ax in axs.flat:
####    ax.label_outer()

## plt.show()



























