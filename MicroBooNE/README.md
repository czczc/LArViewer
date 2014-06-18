## Instructions

### Dowload the Event Viewer 

    git clone https://github.com/czczc/LArViewer.git
    cd LArViewer/MicroBooNE

### Prepare Data 

(on the cluster where you do the simulation and reconstruction)

* copy the local [CTree/](https://github.com/czczc/LArViewer/tree/master/MicroBooNE/CTree) directory to the LarSoft code area (e.g. under srcs/uboonecode/uboone/)
* re-build the LarSoft (add "add_subdirectory(CTree)" to the CMakeLists.txt under src/uboonecode/uboone/ directory; cd into the build directory and "make install")
* lar -c ctree.fcl (input_file)

This will generate a "sample.root" file for the event display. You can then scp this file to your local [data/](https://github.com/czczc/LArViewer/tree/master/MicroBooNE/data) directory.

### Build the Application
    cd scripts
    root -b -q -l loadClasses.C

### 2D Event Display
    (under the scripts/ directory)
    root -l run2d.C

### 3D Event Display
    (under the scripts/ directory)
    root -l run3d.C


## (If You Have [RAKE](https://github.com/jimweirich/rake) Installed)

### Build the Application
    rake

### 2D Event Display

    rake 2d

### 3D Event Display
    
    rake 3d

### Show Full 3D Geometry (OpenGL)
    
    rake geo

