from run_builder import *

Energy=float(sys.argv[1]) if len(sys.argv) > 1 else 25.0
Sigma=float(sys.argv[2]) if len(sys.argv) > 1 else 3.0

place("collimator", "collimator", (0., 0., 0.), material="vacuum", shield_thickness = 20.*cm, b = 3.10*cm, c = 1.315*cm, d = 1.657*cm)

place("Det129", "cube", (0., 0., 129.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)#
place("Det130", "cube", (0., 0., 130.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det150", "cube", (0., 0., 150.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det162", "cube", (0., 0., 162.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)#
place("Det170", "cube", (0., 0., 170.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det198", "cube", (0., 0., 198.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)#
place("Det200", "cube", (0., 0., 200.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det234", "cube", (0., 0., 234.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)#
place("Det250", "cube", (0., 0., 250.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det252", "cube", (0., 0., 252.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)#
place("Det288", "cube", (0., 0., 288.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)#
place("Det300", "cube", (0., 0., 300.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
# place("Det350", "cube", (0., 0., 350.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
# place("Det400", "cube", (0., 0., 400.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
# place("Wall1", "cube", (0., 0., 450.*cm), material= "concrete", size_x = 1000.*cm/2, size_y = 1000.*cm/2, size_z = 1.*m/2)
# place("Wall2", "cube", (0., 0., -450.*cm), material= "concrete", size_x = 1000.*cm/2, size_y = 1000.*cm/2, size_z = 1.*m/2)
# place("Wall3", "cube", (0., 450.*cm, 0), material= "concrete", size_x = 800.*cm/2, size_y = 1.*m/2, size_z = 800.*cm/2)
# place("Wall4", "cube", (0., -450.*cm, 0), material= "concrete", size_x = 800.*cm/2, size_y = 1.*m/2, size_z = 800.*cm/2)
# place("Wall5", "cube", (450.*cm, 0, 0), material= "concrete", size_x = 1.*m/2, size_y = 1000.*cm/2, size_z = 800.*cm/2)
# place("Wall6", "cube", (-450.*cm, 0, 0), material= "concrete", size_x = 1.*m/2, size_y = 1000.*cm/2, size_z = 800.*cm/2)

# place("Foil-V",  "cube", (0., 0., 124.5*cm), material= "vanadium", size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 300.*um/2 )
# place("Foil-Al", "cube", (0., 0., 124.6*cm), material= "aluminum", size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 700.*um/2 )
# place("Foil-Fe", "cube", (0., 0., 124.7*cm), material= "iron",     size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 500.*um/2 )
# place("Foil-Cu", "cube", (0., 0., 124.8*cm), material= "copper",   size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 500.*um/2 )
# place("Foil-Ni", "cube", (0., 0., 124.9*cm), material= "nickel",   size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 500.*um/2 )   
# place("Foil-In", "cube", (0., 0., 125.0*cm), material= "indium",   size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 250.*um/2 )

#Foils - serious try
# place("Foil-V",  "cube", (0., 0., 124.5*cm), material= "vanadium", size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 1000.*um/2 )
# place("Foil-Al", "cube", (0., 0., 124.6*cm), material= "aluminum", size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 1000.*um/2 )
# place("Foil-Fe", "cube", (0., 0., 124.7*cm), material= "iron",     size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 1000.*um/2 )
# place("Foil-Cu", "cube", (0., 0., 124.8*cm), material= "copper",   size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 1000.*um/2 )
# place("Foil-Ni", "cube", (0., 0., 124.9*cm), material= "nickel",   size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 1000.*um/2 )   
# place("Foil-In", "cube", (0., 0., 125.0*cm), material= "indium",   size_x = 100.*mm/2, size_y = 100.*mm/2, size_z = 1000.*um/2 )

# place("Foil-Ti", "cylinder", (0., 0., 139.*cm), material= "titan", radius = 2.5*cm, length=100.0*um/2 )
# place("Foil-Zn", "cylinder", (0., 0., 140.*cm), material= "zinc", radius = 2.5*cm, length=100.0*um/2 )
# place("Foil-Co", "cylinder", (0., 0., 141.*cm), material= "cobalt", radius = 2.5*cm, length=100.0*um/2 )

# make_sd("Aluminum", "Foil-Al", ["ekin"], "neutron")
# make_sd("Vanadium", "Foil-V",  ["ekin"], "neutron")
# make_sd("Iron",     "Foil-Fe", ["ekin"], "neutron")
# make_sd("Copper",   "Foil-Cu", ["ekin"], "neutron")
# make_sd("Nickel",   "Foil-Ni", ["ekin"], "neutron")
# make_sd("Indium",   "Foil-In", ["ekin"], "neutron")

make_sd("Det129", "Det129", ["ekin", "pos_x", "pos_y"], "neutron")#
make_sd("Det130", "Det130", ["ekin", "pos_x", "pos_y"], "neutron")
# make_sd("Det130_g", "Det130", ["ekin", "pos_x", "pos_y"], "gamma")
make_sd("Det150", "Det150", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det162", "Det162", ["ekin", "pos_x", "pos_y"], "neutron")#
make_sd("Det170", "Det170", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det198", "Det198", ["ekin", "pos_x", "pos_y"], "neutron")#
make_sd("Det200", "Det200", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det234", "Det234", ["ekin", "pos_x", "pos_y"], "neutron")#
make_sd("Det250", "Det250", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det252", "Det252", ["ekin", "pos_x", "pos_y"], "neutron")#
make_sd("Det288", "Det288", ["ekin", "pos_x", "pos_y"], "neutron")#
make_sd("Det300", "Det300", ["ekin", "pos_x", "pos_y"], "neutron")
# make_sd("Det350", "Det350", ["ekin", "pos_x", "pos_y"], "neutron")
# make_sd("Det400", "Det400", ["ekin", "pos_x", "pos_y"], "neutron")
# make_sd("lWorld", "World", ["ekin", "pos_x", "pos_y"], "proton")

# place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10.00 * m, inner_radius = 9.99 * m, alpha=0.01, red=255, green = 0)
# make_sd("Neutron", "detector", ["ekin", "theta", "phi"], "neutron")
# make_sd("Gamma", "detector", ["ekin", "theta", "phi"], "gamma")



# place("source_marker", "sphere", (0., 0., 0), material="vacuum", radius = 1. * cm, alpha=0.5, red=0, green = 100)
# make_beam_source("deuteron", 26.5, (0, 0., 0), (0, 0, 1), sigma_r = 0.0 * mm)
# make_beam_source("deuteron", 18.0, (0, 0., 0), (0, 0, 1), sigma_r = 6.0 * mm)
make_beam_source("deuteron", Energy, (0, 0., 0), (0, 0, 1), sigma_r = Sigma * mm)
# make_beam_source("neutron", 10.0, (0, 0., 0), (0, 0, 1), sigma_r = 3.0 * mm)

# make_ui_commands()
set_output_path("collimator_output")
set_run_name("collimator")
config_run(1e8, 8)
start_run()
