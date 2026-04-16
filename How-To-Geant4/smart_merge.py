from sys import argv
from glob import glob
import re
import os


def get_unique_SD_names(files):
    return set([f.split("_")[-1][:-4] for f in files])


def get_unique_sims(files):
    return set([f.split("/")[-3] for f in files])


def main():
    if "-h" in argv:
        print("smart_merge.py [target directory] [optional: thread count of simulation]")
    target_dir = argv[1]
    if not target_dir[-1] == "/":
        target_dir += "/"
    # thread_count = int(argv[2]) if len(argv) > 2 else 1000
    particle_list_mask = target_dir + "*/Lists_of_generated_Particles/*"
    all_files = glob(particle_list_mask, recursive=True)
    # print(all_files)
    sims = get_unique_sims(all_files)

    detectors_per_sim = {}

    for sim in sims:
        regex = re.compile(target_dir + sim + "/Lists_of_generated_Particles/.*")
        per_sim_files = filter(lambda x: regex.match(x), all_files)

        sds_of_sim = get_unique_SD_names(per_sim_files)
        detectors_per_sim[sim] = sds_of_sim

    print(detectors_per_sim)


if __name__ == "__main__":
    main()
