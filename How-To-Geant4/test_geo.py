from run_builder import *

custom_material("ybco", 6.4, yttrium=13.35, barium=41.23, copper=28.62, oxygen=16.81)
place("target", "cube", (0., 0., 0.), material= "ybco", size_x = 0.1 * mm, size_y = 10 * mm, size_z = 10 * mm)
place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10 * cm, inner_radius = 9.9 * cm)
make_sd("scatter", "detector", ["ekin", "theta", "phi"], "primary")
make_particle_source("alpha", 7.5, (-5. * cm, 0., 0.), (1, 0, 0))
config_run(1e6, 8)
build_geo_file()
