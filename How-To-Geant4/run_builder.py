from typing import Tuple


def place(
    name: str, object: str, position: Tuple[float, float, float], **kwdargs):
    ...


def make_sd(name: str):
    ...


def make_ps(name: str):
    ...


def make_source(particle: str, pos: Tuple[float, float, float]):
    ...


def set_run_macro(path: str):
    ...


def build_cluster_tar(job_count: int, bin_path: str):
    # packages the geometry file, macro file, jdl file and
    # all necessary shell scripts into a single tar, which can
    # be used to start the cluster job.
    # simply unpack the tar on desktop.physik and use launch.sh
    # to start the cluster jobs
    ...


def launch_local_job():
    ...


def build_geo_file(path: str):
    # writes the geometry file. for debug or local use
    # or calling the run binary manually (for whatever reason)
    ...
