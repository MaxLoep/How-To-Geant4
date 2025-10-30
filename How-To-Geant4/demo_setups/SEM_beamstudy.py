from run_builder import *
from numpy import pi

place("Plane_0", "cube", (0., 0., -0.1 * cm), material= "vacuum", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 0.1 * cm/2)
place("target", "cube", (0., 0., 0. * cm), material= "aluminum", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 4. * um/2)
place("Plane_1", "cube", (0., 0., 100. * cm), material= "vacuum", size_x = 100. * cm/2, size_y = 100. * cm/2, size_z = 0.1 * cm/2)



# place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10 * cm, inner_radius = 9.5 * cm, phi_min= -0.5 * pi, phi_max= 0.5 * pi, alpha=0.1, red=255, green = 0)
# make_sd("RBS", "detector", ["ekin", "theta", "phi"], "primary")
# make_sd("PIXE", "detector", ["ekin", "theta", "phi"], "gamma")

make_sd("Plane_0", "Plane_0", ["ekin", "pos_x", "pos_y"], "primary")
make_sd("Plane_1", "Plane_1", ["ekin", "pos_x", "pos_y"], "primary")

place("source_marker", "sphere", (0., 0., -1*cm), material="vacuum", radius = 3. * mm, alpha=0.5, red=0, green = 100)

# make_beam_source("proton", 0.75, (0, 0., -1. * cm), (0, 0, 1), sigma_r = 3.0 * mm)
# set_output_path("beamstudy_P_0-75_MeV_3_mm_4_um")
# set_run_name("beamstudy_P_0-75_MeV_3_mm_4_um")

# make_beam_source("proton", 1.50, (0, 0., -1. * cm), (0, 0, 1), sigma_r = 3.0 * mm)
# set_output_path("beamstudy_P_1-5_MeV_3_mm_4_um")
# set_run_name("beamstudy_P_1-5_MeV_3_mm_4_um")

# make_beam_source("alpha", 3.0, (0, 0., -1. * cm), (0, 0, 1), sigma_r = 3.0 * mm)
# set_output_path("beamstudy_A_3_MeV_3_mm_4_um")
# set_run_name("beamstudy_A_3_MeV_3_mm_4_um")

make_beam_source("alpha", 6.0, (0, 0., -1. * cm), (0, 0, 1), sigma_r = 3.0 * mm)
set_output_path("beamstudy_A_6_MeV_3_mm_4_um")
set_run_name("beamstudy_A_6_MeV_3_mm_4_um")


config_run(1.0e7, 8)
start_run()
