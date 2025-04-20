import sys
from types import NoneType
from typing import Tuple, List, Dict

command_list = []

#units:
mm = 1e-3
cm = 1e-2
m = 1.
um = 1e-6

def clear_setup():
    # resets the command queue, so that multiple files
    # can be built in one session
    global command_list
    command_list = []


def spherical(r: Tuple[float, float, float]):
    # converts spherical to carthesian coordiantes
    # makes placing things easier sometimes
    return r # TODO: implement

def can_float(string: str) -> bool:
    try:
        float(string)
        return True
    except:
        return False


def string_arg_count(argument: Dict) -> int:
    count = 0
    for key in argument:
        if not can_float(argument[key]):
            count += 1
    return count


def write_simple_ff(command: Dict):
    write_str = f"{command["command"]}; {str(string_arg_count(command))}; "

    # first put the string args
    for key in command:
        if not can_float(command[key]):
            write_str += f"{key}={command[key]}; "

    # then put the numerical args
    for key in command:
        if can_float(command[key]):
            write_str += f"{key}={command[key]}; "

    global command_list
    command_list.append(write_str)


def place(
    name: str, object: str,
    position: Tuple[float, float, float],
    material = "vacuum",
    rotation = (0., 0., 0.), **kwdargs
):
    x, y, z = position
    rx, ry, rz = rotation

    command_dict = {
        "command": "place",
        "object": object,
        "name" : name,
        "pos_x" : x,
        "pos_y" : y,
        "pos_z" : z,
        "material" : material,
        "rot_x" : rx,
        "rot_y" : ry,
        "rot_z" : rz,
    }

    for key in kwdargs:
        command_dict[key] = kwdargs[key]

    write_simple_ff(command_dict)


def make_sd(name: str, attach_to: str, attributes: List[str], sensitive_to = "all"):
    command_dict = {
        "command": "make_sd",
        "attach": "l" + attach_to,
        "name": name,
        "attrib_count": len(attributes),
        "particle": sensitive_to
    }

    i = 0
    for a in attributes:
        command_dict[f"attrib_{i}"] = a
        i += 1

    write_simple_ff(command_dict)


def make_ps(name: str, attach_to: str):
    command_dict = {
        "command": "make_ps",
        "name": name,
        "attach": "l" + attach_to
    }

    write_simple_ff(command_dict)


def custom_material(name: str, density: float, normalize = True, **components):
    # adds a new, usable material. components should be material=float, with float being the
    # fraction of the custom material which is material
    # expects density in g/cm3 (nope, not adding units for that... for now)
    command_dict = {
        "command": "make_custom_material",
        "name": name,
        "density": density
    }

    norm = 0.
    for key in components:
        norm += components[key]

    norm = 1. if not normalize else norm

    for key in components:
        command_dict[key] = components[key] / norm

    write_simple_ff(command_dict)


def custom_molecule(name: str, density: float, **components):
    # adds a new, usable material. components should be element=int,
    # according to the molecular formular
    command_dict = {
        "command": "make_custom_molecule",
        "name": name,
        "density": density
    }

    norm = 0.
    for key in components:
        norm += components[key]

    for key in components:
        command_dict[key] = components[key] / norm

    write_simple_ff(command_dict)


def no_macro_run_file(event_count: int, thread_count = 1):
    # enables running without macro file.
    # will become default soon
    command_dict = {
        "command": "no_macro_f",
        "event_count": event_count,
        "thread_count": thread_count
    }

    write_simple_ff(command_dict)


def make_particle_source(particle: str, energy: float, position: Tuple[float, float, float], rotation = (0., 0., 0.)):
    # only works when no macro file is used
    x, y, z = position
    rx, ry, rz = rotation
    command_dict = {
        "command": "particle_source",
        "particle": particle,
        "energy": energy, # TODO: make energy distribution an option
        "mono_e": "true",
        "sigma": 0.,
        "x_pos": x,
        "y_pos": y,
        "z_pos": z,
        "x_rot": rx,
        "y_rot": ry,
        "z_rot": rz,
    }

    write_simple_ff(command_dict)



def build_cluster_tar(job_count: int, bin_path: str):
    # packages the geometry file, macro file, jdl file and
    # all necessary shell scripts into a single tar, which can
    # be used to start the cluster job.
    # simply unpack the tar on desktop.physik and use launch.sh
    # to start the cluster jobs
    ...


def launch_local_job():
    ...


def build_geo_file(path = None):
    # writes the geometry file. for debug or local use
    # or calling the run binary manually (for whatever reason)
    global command_list

    if type(path) == NoneType:
        _ = [print(line) for line in command_list]
        return

    with open(path, "w") as file:
        file.writelines(command_list)
