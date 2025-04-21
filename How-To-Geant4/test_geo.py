from run_builder import *
from math import pi

custom_material("ybco", 6.4, yttrium=13.35, barium=41.23, copper=28.62, oxygen=16.81)
#place("target", "cube", (0., 0., 0.), material= "ybco", size_x = 0.1 * mm, size_y = 10 * mm, size_z = 20 * mm)

place("x_block", "cube", (2. * cm, 0., 0.), material="vacuum", size_x = 2 * cm, size_y = 1 * mm, size_z = 1 * mm, red = 255, green = 0, blue = 0, alpha = 0.5)
place("y_block", "cube", (0., 2. * cm, 0.), material="vacuum", size_y = 2 * cm, size_x = 1 * mm, size_z = 1 * mm, red = 0, green = 255, blue = 0, alpha = 0.5)
place("z_block", "cube", (0., 0., 2. * cm), material="vacuum", size_z = 2 * cm, size_y = 1 * mm, size_x = 1 * mm, red = 0, green = 0, blue = 255, alpha = 0.5)

place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10 * cm, inner_radius = 9.9 * cm, alpha=0.01, red=255, green = 0)
make_sd("scatter", "detector", ["ekin", "theta", "phi"], "primary")
make_particle_source("alpha", 7.5, (0, 0., 0.), (0, 1, 0))
#config_run(1e6, 8)
start_run()
