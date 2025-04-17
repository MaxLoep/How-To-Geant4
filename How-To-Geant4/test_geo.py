from run_builder import *

place("target", "cube", (0., 0., 0.), material= "carbon", size_x = 0.1 * mm, size_y = 10 * mm, size_z = 10 * mm)
place("detector", "sphere", (0., 0., 0.), radius = 10 * cm, inner_radius = 9.9 * cm)
make_sd("exit_counter", "detector", ["ekin", "theta", "phi"], "all")
build_geo_file()
