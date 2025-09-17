from run_builder import *
from numpy import pi

custom_material("petg", 1.4, carbon=62.5, hydrogen=4.2, oxygen=33.3)
place("target", "cube", (0., 0., 0.), material= "petg", size_x = 10. * mm, size_y = 10. * mm, size_z = 5. * mm)

place("source_marker", "sphere", (0., 0., -5. * cm), material="vacuum", radius = 2. * mm, alpha=0.5, red=0, green = 100)

place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10 * cm, inner_radius = 9.5 * cm, phi_min= -0.5 * pi, phi_max= 0.5 * pi, alpha=0.1, red=255, green = 0)
make_sd("detector", "detector", ["ekin", "theta", "phi"], "all")
make_beam_source("alpha", 14., (0, 0., -5. * cm), (0, 0, 1), sigma_r = 0.4* mm)

set_output_path("activation_sim")
set_run_name("activation_sim")
config_run(1e7, 8)
start_run()
