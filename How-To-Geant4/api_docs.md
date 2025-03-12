<details>
<summary> <ins> scope and concept </ins> </summary>

The simplified api aims to wrap some basic geant4 capabilities into a more user friendly form, thus enabling quicker simulation of simple setups for rate approximation or estimating contamination.
It aims to only require limited C++ from the user and reduce the prerequisite amount of experience with geant4.
This by necesseity limits the things that can be done, hence only a few select features are represented.

</details>
<details>
	<summary> <ins> api::make_sd(...) </ins></summary>

	void make_sd(
		std::string name,
		std::string particle,
		std::vector<property> properties,
		std::string l_volume = ""
	);
	
Creates a sensitive detector with the given name.
It is sensitive to _particle_. If _particle_ is "all", the SD will be sensitive to all particles[^1], if _particle_ is "primary" the SD will be sensitive to the runs primary particle.

_properties_ is a vector of particle properties, which will be logged (the columns will be be created in the order in which the properties are found in the vector).

*l_volume* is an optional parameter. If it's omitted, the logical volume to which the SD gets attached is given by prepending _name_ with "l" otherwise, the SD will be attached to *l_volume*. Make sure *l_volume* exists.

</details>

<details> 
<summary> <ins> api::place_geometry(...) </ins> </summary>

	void place_geometry(
		std::string l_volume_name,
		std::string geometry_name,
		std::map<std::string, double> placement_params = {},
		Materials::MaterialMaker material = Materials::Vacuum
	);

This function is used to generically place named logical volumes. *l_volume_name* is the name of the logical volume. This name is used to referr to the created volume from different places (no two logical volumes should be named the same).

*geometry_name* referrs to the object to be placed (i.e. "sphere"). These are either provided default geometries or self defined and registered geometries.

*placement_params* contains numerical parameters required for the placement. What is needed will vary per object, often things such as size, position, rotation will be present.

_material_ sets the material of the volume. If ommited, vacuum is the default. Predefined materials and custom materials can be found in the _Material_ namespace.
</details>
<details> 
<summary> <ins> api::add_placer(...) </ins> </summary>

	void api::add_placer(std::string name, placer_func func)

This utility function is used to add custom geometry placement functions. It adds _func_ into the map of known geometries under _name_. _placer_func_ is an alias for std::function<G4LogicalVolume*(std::string, std::map<std::string, double>&, Materials::MaterialMaker)>. 
</details>

<details> 
<summary> <ins> api::make_ps(...) </ins> </summary>

	void make_ps(
		std::string name,
		std::string quantity,
		bool filter,
		ConfigStructs::ParticleSpec particle_filter,
		std::string l_volume = ""
	);

Creates a primitive scorer named _name_ for _quantity_. For possible _quantity_ options see https://geant4.web.cern.ch/documentation/dev/bfad_html/ForApplicationDevelopers/Detector/commandScore.html . If the scorer should be filtered, set _filter_ to true and specify _particle_filter_ to the particle which you wish to detect.
</details>

<details> 
<summary> <ins> api::setup_sim() </ins> </summary>

Probably not permanent. For now, code that builds the actualy concrete simulation setup (geometries, detectors and the like) goes here. 
</details>


[^1]: not yet implemented
