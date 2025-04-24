from run_builder import *

place("collimator", "collimator", (0., 0., 0.), material="vacuum", shield_thickness = 5. * cm, b = 5. * cm, c = 5. * cm, d = 5. * cm)

place("source_marker", "sphere", (0., 0., -5. * cm), material="vacuum", radius = 2. * mm, alpha=0.5, red=0, green = 100)

place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10 * m, inner_radius = 9.5 * m, alpha=0.01, red=255, green = 0)
make_sd("RBS", "detector", ["ekin", "theta", "phi"], "primary")
make_sd("PIXE", "detector", ["ekin", "theta", "phi"], "gamma")
make_beam_source("deuteron", 26.5, (0, 0., -5. * cm), (0, 0, 1), sigma_r = 1.7 * mm)
config_run(1e6, 8)
start_run()
