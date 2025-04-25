from run_builder import *

def sim_thickness(d):
    place("shield", "cube", (0., 0., 0.), material= "carbon", size_x = 10. * mm, size_y = 10. * mm, size_z = d / 2.)

    place("source_marker", "sphere", (0., 0., -5. * cm), material="vacuum", radius = 2. * mm, alpha=0.5, red=0, green = 100)
    place("surround", "sphere", (0., 0., 0.), material="vacuum", radius = 20.*cm, inner_radius=19.*cm, alpha=0.2, red=100, green = 0)

    place("detector", "cube", (0., 0., d + 1. * cm), material= "vacuum", size_x = 10. * mm, size_y = 10. * mm, size_z = 5. * um)
    make_sd("behind_shield", "detector", ["ekin"], "primary")

    make_beam_source("neutron", 28., (0, 0., -5. * cm), (0, 0, 1), sigma_r = 0.4* mm)
    config_run(1e5, 8)
    start_run(f"thickness{d}.run")
    #start_run()


for d in range(1, 10):
    sim_thickness(float(d) * cm)

build_cluster_tar(1, "../../build/How-To-Geant4", "neutron_shielding")
