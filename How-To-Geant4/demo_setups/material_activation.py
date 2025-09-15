from run_builder import *
from numpy import pi

place("target", "cube", (0., 0., 10. * cm), material= "air", size_x = 10. * cm/2, size_y = 10. * cm/2, size_z = 10. * cm/2)
# place("target", "cube", (0., 0., 10. * cm), material= "argon", size_x = 3. * cm/2, size_y = 3. * cm/2, size_z = 3. * cm/2)



# place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10 * cm, inner_radius = 9.5 * cm, phi_min= -0.5 * pi, phi_max= 0.5 * pi, alpha=0.1, red=255, green = 0)
# make_sd("RBS", "detector", ["ekin", "theta", "phi"], "primary")
# make_sd("PIXE", "detector", ["ekin", "theta", "phi"], "gamma")

place("source_marker", "sphere", (0., 0., -1*cm), material="vacuum", radius = 2. * mm, alpha=0.5, red=0, green = 100)
make_beam_source("ion", 100., (0, 0., -1. * cm), (0, 0, 1), sigma_r = 0.0* mm, atomic_number = 7, atomic_mass = 14, charge=5)

set_output_path("material_activation_output")
set_run_name("nitrogen_air_activation")
# set_run_name("nitrogen_argon_activation")
config_run(6.242e4*8, 8)
start_run()
