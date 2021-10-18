# How-To-Geant4
V0.4.1:\
-The installed Geant4 version will be detected in the main file "How-to-Geant4.cc". "G4RunManagerFactory" will be used in version >=10.7.0 otherwise the old RunManager is used for backward compatibility.\
-Running the simulation will create a new output file (RunData_X.root) with increased numbering X instead of deleting the existing one.

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
-GeneralParticleSource (GPS) integrated as a way to produce primary particles\
-Some code cleanup

V0.1 (Initial commit):\
-The Geant4 'B1-example' was taken and all kinds of geometries and materials were added to 'B1DetectorConstruction.cc'

Basic Idea:\
Geant4 is a powerful simulation toolkit, but hard to get into. I started collecting code-snippets and putting them together because I don't find the Basic-Examples basic enough. You can copy these code-snippets and paste them into any simulation and it 'should' work.
So the basic idea in principle is:\
->'You want a Cylinder-geometry in your simulation? here, copy this snippet and change the parameters to fit your needs!'\
->'You want a particle source was gaussian distribution and energy E, standard deviation sigma, etc.? Here, copy this snippet and change the parameters!'\
->'You want ABC...XYZ? Here, copy this snippet...'

