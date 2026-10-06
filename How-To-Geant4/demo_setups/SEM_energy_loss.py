from run_builder import *
from numpy import pi

#Primary beam Detection plane
place("Plane_0", "cube", (0., 0., -0.1 * cm), material= "vacuum",   size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 0.1 * cm/2)
#HV-Foil 1
place("Alu_HV1",  "cube", (0., 0., 0. * cm), material= "aluminum",  size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 4.5 * um/2)
#SEM Foil 1
place("Carbon1",  "cube", (0., 0., 0.1 * cm), material= "carbon",   size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 400. * nm/2)
place("Alu_SEM1", "cube", (0., 0., 0.2 * cm), material= "aluminum", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 4.5 * um/2)
place("Carbon2",  "cube", (0., 0., 0.3 * cm), material= "carbon",   size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 400. * nm/2)
#HV-Foil 2
place("Alu_HV2",  "cube", (0., 0., 0.4 * cm), material= "aluminum", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 4.5 * um/2)
#SEM Foil 2
place("Carbon3",  "cube", (0., 0., 0.5 * cm), material= "carbon",   size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 400. * nm/2)
place("Alu_SEM2", "cube", (0., 0., 0.6 * cm), material= "aluminum", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 4.5 * um/2)
place("Carbon4",  "cube", (0., 0., 0.7 * cm), material= "carbon",   size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 400. * nm/2)
#HV-Foil 3
place("Alu_HV3",  "cube", (0., 0., 0.8 * cm), material= "aluminum", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 4.5 * um/2)
#SEM Eloss Detection plane
place("Plane_1",  "cube", (0., 0., 1. * cm), material= "vacuum", size_x = 20. * cm/2, size_y = 20. * cm/2, size_z = 0.1 * cm/2)
#Lenard Window
place("Alu_LW",   "cube", (0., 0., 1.4 * cm), material= "aluminum", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 30 * um/2)
#Lenard Window Eloss Detection plane
place("Plane_2",  "cube", (0., 0., 2. * cm), material= "vacuum", size_x = 20. * cm/2, size_y = 20. * cm/2, size_z = 0.1 * cm/2)
#Air
place("Air_1",  "cube", (0., 0., 10. * cm), material= "air", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 15. * cm/2)
place("Aluwrap",  "cube", (0., 0., 18. * cm), material= "aluminum",  size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 4.5 * um/2)
#Air Eloss Detection plane
place("Plane_3",  "cube", (0., 0., 20. * cm), material= "vacuum", size_x = 20. * cm/2, size_y = 20. * cm/2, size_z = 0.1 * cm/2)



# place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10 * cm, inner_radius = 9.5 * cm, phi_min= -0.5 * pi, phi_max= 0.5 * pi, alpha=0.1, red=255, green = 0)
# make_sd("RBS", "detector", ["ekin", "theta", "phi"], "primary")
# make_sd("PIXE", "detector", ["ekin", "theta", "phi"], "gamma")

make_sd("Plane_0", "Plane_0", ["ekin", "pos_x", "pos_y"], "primary") # Primary beam
make_sd("Plane_1", "Plane_1", ["ekin", "pos_x", "pos_y"], "primary") # after SEM
make_sd("Plane_2", "Plane_2", ["ekin", "pos_x", "pos_y"], "primary") # after Lenard Window
make_sd("Plane_3", "Plane_3", ["ekin", "pos_x", "pos_y"], "primary") # after air + alu wrapping

place("source_marker", "sphere", (0., 0., -1*cm), material="vacuum", radius = 3. * mm, alpha=0.5, red=0, green = 100)

particle = "proton"
# particle = "deuteron"
# particle = "alpha"
energy = 14.0

make_beam_source(particle,   energy, (0, 0., -1. * cm), (0, 0, 1), sigma_r = 3.0 * mm)
# make_beam_source("deuteron", 28.0, (0, 0., -1. * cm), (0, 0, 1), sigma_r = 3.0 * mm)
# make_beam_source("alpha",    56.0, (0, 0., -1. * cm), (0, 0, 1), sigma_r = 3.0 * mm)
set_output_path("SEM_Energy_Loss")
set_run_name("SEM_Eloss_"+ particle +"_" + str(int(energy)) + "_MeV" )


config_run(1.0e3, 8)
# make_ui_commands()
start_run()
