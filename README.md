# How-To-Geant4

v1.0.1:\
-code cleanup
--moved self-written PhysicsLists "ElectromagneticPhysics.cc", "GammaNuclearPhysics.cc", "HadronElasticPhysicsHP.cc" and "NeutronHPMessenger.cc" to seperate folder\
--marked some things for future clean ups
 
v1.0.0:\
-migration from Geant4 v10.7.3 to Geant4 v11.0.3\
--Obsolete(?) custom physics lists in 'GammaNuclearPhysics.cc' commented out because Geant4 changed how to implement these\
--Selecting the .root-file format is now done by "G4AnalysisManager.hh" therefore things needed to be changed in "Anlysis.hh" and "RunAction.cc"\

V0.7.7:\
-started implementing import of a GDML file for DetectorConstruction and export geometry to a GDML file\
-custom commands added for import/export of GDML file

V0.7.6.6:\
-SensitiveDetectors are different metallic materials now (20um)

V0.7.6.5:\
-particle counter in every SensitiveDetector implemented\
-particles passing through are saved in file 'ListOfGeneratedParticles in SDX.txt'\
-some code cleanup and commenting 

V0.7.6.0:\
-started to implement counter in SensitiveDetector to count number of produced isotopes in volume

V0.7.5.2:\
-changes in 'PlotRootFile.py' to plot TH2D (not fully working)\
-changes in 'CmakeLists.txt'; macro areno longer copied to build directory

V0.7.5.1:\
-changes to 'CMakeLists.txt' to make it run nicely from VSCode's CMake extension

V0.7.5:\
-verbosity reduced via UI commands in How-To-Geant4.cc (console doesn't show wall-of-text with physics infos when run)\
-PlotRootFile.py got some changes

V0.7.4:\
-more advanced geometries

V0.7.3:\
-some fixes in 'CMakeLists.txt' for compilation with gcc version <9

V0.7.2:\
-much code clean up\
-root output files and 'ListOfGeneratedParticles.txt' are now saved in subfolders

V0.7.1:\
-some code clean up\
-added custom commands to change the parameters a,b,c,d,e in 'DetectorConstriction.cc' via macro commands (change_x)\
-added command to change 'dummy_mat' in 'DetectorConstriction.cc' via macro commands (setMat)\
-removed some old custom commands (setIsotopeMat, setThickness, setSizeYZ)\
-added Sensitive Detector 'SphereSD' at origin to detect only gammas (to be used with 'radioactivedaceay.mac' to study decay)\
-added 'protonFilter' to bigger box PS because it was missing

V0.7:\
-Moved example geometries to 'geometries'-folder\
--'BasicGeometries.cc' contains build-in geometries\
--'AdvancedGeometries.cc' contains how to import .stl- or .obj-files and some other more complex geometries\
--'oldDetectorConstruction.cc' is the original file from V0.6\
-'DetectorConstruction.cc' now features 5 flat boxes as Sensitive Detextor (SD) and a bigger box as Primitive Scorer (PS) (to be used with 'generalparticlesource.mac' to study proton beam spreading in air)

V0.6:\
-new macro files for different things:\
--'generalparticlesource.mac' for studying beams of particles\
--'radioactivedecay.mac' for studying decay chains and radio active sources\
--'scoringmesh.mac' for an example of how to create a scoring mesh\
--'vis.mac' as a default visualization script that is run if no other macro file is given\
--'run.mac' as test and default macro file\
-new header file 'CADMesh.hh' allows to import CAD files (like .obj, .stl) as geometries\
--'Lochflansch.obj', 'Lochflansch.stl' and 'Austrittsfenster.obj' were added to show how this is done\
-new python script 'PlotRootFile.py' to plot all TH1D and TTree in a root-file (Histograms)\
-new feature: the simulation takes the process-ID as initial random seed to ensure different results are generated everytime it is started\
--output file name changed to 'ID_processID.root'

V0.4.1:\
-The installed Geant4 version will be detected in the main file "How-to-Geant4.cc". "G4RunManagerFactory" will be used in version >=10.7.0 otherwise the old RunManager is used for backward compatibility.\
-Running the simulation will create a new output file (RunData_X.root) with increased numbering X instead of deleting the existing one.\
-Multiple runs can be chained together via the macro file (see 'run2.mac').\
-Macro file contains command to silence process verbosity at the start of a run (see 'run2.mac').\
-Random seed for a run can be set in the macro file (see 'run2.mac').\
-Root macro 'plotHisto.C' merges all output files together and plots them.

V0.4:\
-Changed file names to more neutral ones\
-Added code to either choose a predefined physics list or use a selfmade physics list (found in 'PhysicsList.cc') in 'How-To-Geant4.cc'\
-Tested (the most) physic lists in 'How-To-Geant4.cc' and 'PhysicsList.cc'\
-Added 2 ways of analysis: 'SensitiveDetector' (SD) and 'PrimitiveScorer' (PS)\
SD: Make a volume in 'DetectorConstruction.cc' a SD to read information about the particles passing through (this is done in 'BoxSD.cc')\
PS: Make a volume in 'DetectorConstruction.cc' a PS to read information about things related to this volume (e.g. energy deposition)\
-Analysis output possible in root-histograms and root-Ntuples

V0.3:\
-Started adding Analysis Manager from Example B4d

V0.2:\
-Started changing the name(s) from 'B1' to 'How-To-Geant4'\
-GeneralParticleSource (GPS) integrated as a way to produce primary particles. Change GPS parameters in the macro file.\
-Some code cleanup

V0.1 (Initial commit):\
-The Geant4 'B1-example' was taken and all kinds of geometries and materials were added to 'B1DetectorConstruction.cc'

Basic Idea:\
Geant4 is a powerful simulation toolkit, but hard to get into. I started collecting code-snippets and putting them together because I don't find the Basic-Examples basic enough. You can copy these code-snippets and paste them into any simulation and it 'should' work.
So the basic idea in principle is:\
->'You want a Cylinder-geometry in your simulation? here, copy this snippet and change the parameters to fit your needs!'\
->'You want a particle source was gaussian distribution and energy E, standard deviation sigma, etc.? Here, copy this snippet and change the parameters!'\
->'You want ABC...XYZ? Here, copy this snippet...'

