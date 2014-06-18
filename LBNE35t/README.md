## Instructions

### Dowload the Event Viewer 

    git clone https://github.com/czczc/LArViewer.git
    cd LArViewer/LBNE35t

### Prepare Data 

(on the cluster where you do the simulation and reconstruction)

* copy the local [DataConverter/](https://github.com/czczc/LArViewer/tree/master/LBNE35t/DataConverter) directory to the LarSoft code area (e.g. under srcs/lbnecode/lbne/)
* re-build the LarSoft (add "add_subdirectory(DataConverter)" to the CMakeLists.txt under src/lbnecode/lbne/ directory; cd into the build directory and "make install")
* (if necessary) cp the two .fcl file "reco35t.fcl" and "dc35t.fcl" under [DataConverter/](https://github.com/czczc/LArViewer/tree/master/LBNE35t/DataConverter) to your current working directory.
* (if you haven't produced a reco file) lar -c reco35t.fcl (raw_MC_file)
* lar -c dc35t.fcl (reco_file)

This will generate a "sample.root" file for the event display. You can then scp this file to your local [data/](https://github.com/czczc/LArViewer/tree/master/LBNE35t/data) directory.

### Build the Application
    cd scripts
    root -b -q -l loadClasses.C

### 2D Event Display
    (under the scripts/ directory)
    root -l run2d.C

### 3D Event Display
    (under the scripts/ directory)
    root -l run3d.C
