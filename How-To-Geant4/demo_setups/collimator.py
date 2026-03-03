from run_builder import *

place("collimator", "collimator", (0., 0., 0.), material="vacuum", shield_thickness = 20.*cm, b = 3.10*cm, c = 1.315*cm, d = 1.657*cm)

place("Det130", "cube", (0., 0., 130.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det150", "cube", (0., 0., 150.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det170", "cube", (0., 0., 170.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det200", "cube", (0., 0., 200.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det250", "cube", (0., 0., 250.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det300", "cube", (0., 0., 300.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det350", "cube", (0., 0., 350.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
place("Det400", "cube", (0., 0., 400.*cm), material= "vacuum", size_x = 30.*cm/2, size_y = 30.*cm/2, size_z = 1.*mm/2)
# place("Wall1", "cube", (0., 0., 450.*cm), material= "concrete", size_x = 1000.*cm/2, size_y = 1000.*cm/2, size_z = 1.*m/2)
# place("Wall2", "cube", (0., 0., -450.*cm), material= "concrete", size_x = 1000.*cm/2, size_y = 1000.*cm/2, size_z = 1.*m/2)
# place("Wall3", "cube", (0., 450.*cm, 0), material= "concrete", size_x = 800.*cm/2, size_y = 1.*m/2, size_z = 800.*cm/2)
# place("Wall4", "cube", (0., -450.*cm, 0), material= "concrete", size_x = 800.*cm/2, size_y = 1.*m/2, size_z = 800.*cm/2)
# place("Wall5", "cube", (450.*cm, 0, 0), material= "concrete", size_x = 1.*m/2, size_y = 1000.*cm/2, size_z = 800.*cm/2)
# place("Wall6", "cube", (-450.*cm, 0, 0), material= "concrete", size_x = 1.*m/2, size_y = 1000.*cm/2, size_z = 800.*cm/2)
place("Foil-Al", "cylinder", (0., 0., 134.*cm), material= "aluminum", radius = 2.5*cm, length=100.0*um/2 )
place("Foil-In", "cylinder", (0., 0., 135.*cm), material= "indium", radius = 2.5*cm, length=100.0*um/2 )    #Indium: 0.1 - 2 mm
place("Foil-Cu", "cylinder", (0., 0., 136.*cm), material= "copper", radius = 2.5*cm, length=100.0*um/2 )
place("Foil-Fe", "cylinder", (0., 0., 137.*cm), material= "iron", radius = 2.5*cm, length=100.0*um/2 )
place("Foil-Ni", "cylinder", (0., 0., 138.*cm), material= "nickel", radius = 2.5*cm, length=100.0*um/2 )    #Nickel: 0.1 - 1 mm
place("Foil-Ti", "cylinder", (0., 0., 139.*cm), material= "titan", radius = 2.5*cm, length=100.0*um/2 )
place("Foil-Zn", "cylinder", (0., 0., 140.*cm), material= "zinc", radius = 2.5*cm, length=100.0*um/2 )
place("Foil-Co", "cylinder", (0., 0., 141.*cm), material= "cobalt", radius = 2.5*cm, length=100.0*um/2 )

make_sd("Det130", "Det130", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det130_g", "Det130", ["ekin", "pos_x", "pos_y"], "gamma")
make_sd("Det150", "Det150", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det170", "Det170", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det200", "Det200", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det250", "Det250", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det300", "Det300", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det350", "Det350", ["ekin", "pos_x", "pos_y"], "neutron")
make_sd("Det400", "Det400", ["ekin", "pos_x", "pos_y"], "neutron")
# make_sd("lWorld", "World", ["ekin", "pos_x", "pos_y"], "proton")

# place("detector", "sphere", (0., 0., 0.), material="vacuum", radius = 10.00 * m, inner_radius = 9.99 * m, alpha=0.01, red=255, green = 0)
# make_sd("Neutron", "detector", ["ekin", "theta", "phi"], "neutron")
# make_sd("Gamma", "detector", ["ekin", "theta", "phi"], "gamma")



# place("source_marker", "sphere", (0., 0., 0), material="vacuum", radius = 1. * cm, alpha=0.5, red=0, green = 100)
# make_beam_source("deuteron", 26.5, (0, 0., 0), (0, 0, 1), sigma_r = 0.0 * mm)
make_beam_source("deuteron", 26.0, (0, 0., 0), (0, 0, 1), sigma_r = 3.0 * mm)

make_ui_commands()
set_output_path("collimator_output")
set_run_name("collimator")
# config_run(1e8, 8)
start_run()

