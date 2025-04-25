<details>
<summary> <ins> scope and concept </ins> </summary>

The simplified api aims to wrap some basic geant4 capabilities into a more user friendly form, thus enabling quicker simulation of simple setups for rate approximation or estimating contamination.
It aims to only require limited C++ from the user and reduce the prerequisite amount of experience with geant4.
This by necesseity limits the things that can be done, hence only a few select features are represented.

</details>
<details>
<summary> <ins> make_sd(name: str, attach_to: str, attributes: List[str], sensitive_to = "all") </ins></summary>

Specifies that a placed geometry should act as a sensitive detector and which attributes to log.
The geometry (attach_to) must exist (by name).
Multiple SDs can attach to the same geometry.
</details>

<details>
<summary> <ins> prerun_macro(cmd: str) </ins></summary>

The given geant macro will be run before the run is started.
This is a function for when you know what you are doing.
probably don't use it too much.
</details>

<detail>
<summary> <ins> custom_material(name: str, density: float, normalize = True, **components) </ins></summary>

Speficies the makeup of a custom material. Density should be given in gram per cubic centimeter.
Components are the Materials that make up this new material, with the amounts in mass fractions.
If normalize is True, these will be normalized so that the sum of all components is 1.
</details>
