from run_builder import *

Energy=float(sys.argv[1]) if len(sys.argv) > 1 else 14.0
Sigma=float(sys.argv[2]) if len(sys.argv) > 1 else 0.0

# place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10.00 * m, inner_radius = 9.99 * m, alpha=0.01, red=255, green = 0)
# make_sd("Neutron", "detector", ["ekin", "theta", "phi"], "neutron")
# make_sd("Gamma", "detector", ["ekin", "theta", "phi"], "gamma")

place("target", "cylinder", (0., 0., 1.0*mm), material= "graphite", radius = 2.0*mm, length=2.0*mm/2 )

place("detector", "sphere", (0., 0., 0), material="vacuum", radius = 100. * cm, alpha=0.5, red=100, green = 0)
make_sd("N_SphereSD", "detector", ["ekin", "theta", "phi"], "neutron")

place("source_marker", "sphere", (0., 0., -10.*cm), material="vacuum", radius = 1. * cm, alpha=0.5, red=0, green = 100)

make_beam_source("proton", Energy, (0, 0., -10.*cm), (0, 0, 1), sigma_r = Sigma * mm)
# make_beam_source("deuteron", Energy, (0, 0., -1), (0, 0, 1), sigma_r = Sigma * mm)
# make_beam_source("alpha", Energy, (0, 0., -1), (0, 0, 1), sigma_r = Sigma * mm)

make_ui_commands()
set_output_path("tny_P_14MeV_G4_Al")
set_run_name("tny_P_14MeV_G4_Al")
# config_run(1e8, 8)
start_run()
