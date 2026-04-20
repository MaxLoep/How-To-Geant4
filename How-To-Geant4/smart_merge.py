import sys
from glob import glob
import re
import tomllib
import os


def some(v):
    return not isinstance(v, type(None))


class particle_data:
    def __init__(self, particle : dict, name : str):
        self.is_primary = "Primaries_" in name
        self.abundance = None
        self.name = name
        self.pcount = particle['count']
        self.stable = particle['stable']
        self.half_life = particle['half_life']
        if not self.stable:
            self.hrhl = particle['human_readable_half_life']

    def __str__(self):
        res = ""
        res += "[" + self.name + "]\n"
        res += "count = " + str(self.pcount) + "\n"
        res += "stable = " + str(self.stable) + "\n"
        res += "half_life = " + str(self.half_life) + "\n"
        if not self.stable:
            res += "human_readable_half_life = " + self.hrhl + "\n"
        if some(self.abundance):
            res += "abundance = " + str(self.abundance) + "\n"
        res += "\n"
        return res


    def __eq__(self, other):
        return self.name == other.name \
            and self.pcount == other.count \
            and self.half_life == other.half_life


    def __add__(self, other):
        if self.name != other.name:
            print("adding failure")
        res = self
        res.pcount += other.pcount
        return res


def to_dict(file: str) -> dict:
    res = {}
    with open(file, 'rb') as handle:
        try:
            dict = tomllib.load(handle)
            for name in dict:
                res[name] = particle_data(dict[name], name)
        except Exception as e:
            print(f"{e} ocurred in {file}:")
            print(handle.readlines())

    return res


def add_abundance_info(data: dict) -> dict:
    primary = list(filter(lambda x: data[x].is_primary, data))

    secondaries = {}
    for particle in data:
        if not data[particle].is_primary:
            secondaries[particle] = data[particle]

    if len(primary) > 1 or len(primary) <1:
        print("more then one kind of primary particle was encountered")
        print("merging without abundance calculations!")
        return data
    else:
        primary = data[primary[0]]

    pcount = primary.pcount
    for particle in secondaries:
        secondaries[particle].abundance = secondaries[particle].pcount / pcount

    res = secondaries
    res[primary.name] = primary
    return res

def filter_stable(data: dict) -> dict:
    primary = list(filter(lambda x: data[x].is_primary, data))

    secondaries = {}
    for particle in data:
        if not data[particle].stable:
            secondaries[particle] = data[particle]

    if len(primary) > 1 or len(primary) <1:
        print("more then one kind of primary particle was encountered")
        print("merging without abundance calculations!")
        return data
    else:
        primary = data[primary[0]]

    res = secondaries
    res[primary.name] = primary
    return res


def filter_excited(data: dict) -> dict:
    primary = list(filter(lambda x: data[x].is_primary, data))

    secondaries = {}
    for particle in data:
        if data[particle].half_life > 0:
            secondaries[particle] = data[particle]

    if len(primary) > 1 or len(primary) <1:
        print("more then one kind of primary particle was encountered")
        print("merging without abundance calculations!")
        return data
    else:
        primary = data[primary[0]]

    res = secondaries
    res[primary.name] = primary
    return res


def toml_merge(files, output_file, ignore_stable=False):
    master_dict = {}

    for file in files:
        file_content = to_dict(file)
        for particle in file_content:
            if particle in master_dict:
                try:
                    master_dict[particle] += file_content[particle]
                    # print(particle)
                    # print(file_content[particle])
                except Exception as _:
                    print("skipped merge")
            else:
                master_dict[particle] = file_content[particle]

    master_dict = add_abundance_info(master_dict)

    if ignore_stable:
        master_dict = filter_stable(master_dict)
        master_dict = filter_excited(master_dict)

    master_dict_sorted = dict(sorted(master_dict.items(), key=lambda x: x[0]))

    with open(output_file, 'w') as out_handle:
        # out_handle.writelines([str(master_dict[pd]) for pd in master_dict])
        out_handle.writelines([str(master_dict_sorted[pd]) for pd in master_dict_sorted])
    # pprint(master_dict_sorted)



def get_unique_SD_names(files):
    return set([f.split("_")[-1][:-4] for f in files])


def get_unique_sims(files):
    return set([f.split("/")[-3] for f in files])


def call_toml_merge(path, detector, all_files, expected_thread_count, delete):
    # regex = re.compile(path + f"/Lists_of_generated_Particles/.*_\\d_\\d_{detector}\\.txt")
    regex = re.compile(path + f"/Lists_of_generated_Particles/.+_.+_.+_{detector}\\.txt")
    per_sd_files = filter(lambda x: regex.match(x), all_files)

    sd_files = list(per_sd_files)
    print(sd_files)
    toml_merge(sd_files, f"{path}/particle_list_{detector}.txt")

    return len(sd_files) == expected_thread_count


def main():
    if "-h" in sys.argv:
        print("smart_merge.py [target directory] [optional: thread count of simulation]")
    target_dir = sys.argv[1]
    if not target_dir[-1] == "/":
        target_dir += "/"
    thread_count = int(sys.argv[2]) if len(sys.argv) > 2 else 1000
    delete = bool(sys.argv[3]) if len(sys.argv) > 3 else True
    particle_list_mask = target_dir + "*/Lists_of_generated_Particles/*"
    all_files = glob(particle_list_mask, recursive=True)
    # print(all_files)
    sims = get_unique_sims(all_files)

    detectors_per_sim = {}
    sim_paths = {}

    for sim in sims:
        regex = re.compile(target_dir + sim + "/Lists_of_generated_Particles/.*")
        per_sim_files = filter(lambda x: regex.match(x), all_files)

        sds_of_sim = get_unique_SD_names(per_sim_files)
        detectors_per_sim[sim] = sds_of_sim
        sim_paths[sim] = target_dir + sim # if multiple levels get supported, add that in here

    print(detectors_per_sim)
    print(sim_paths)

    for sim in detectors_per_sim:
        print(f"---------------- running for {sim}")
        correct_file_num = True
        for sd in detectors_per_sim[sim]:
            # TODO: check if file count is correct
            correct_file_num &= call_toml_merge(sim_paths[sim], sd, all_files, thread_count, delete)

        if correct_file_num and delete:
            os.system(f"rm -r {sim_paths[sim]}/Lists_of_generated_Particles")
        elif delete:
            print(f"skipping deletion on {sim_paths[sim]} because the number of files was wrong!!")
        # merge root files:
        # root_blob = uproot.concatenate(f"{sim_paths[sim]}/Root_Files/*")
        # merged_root = uproot.create(f"{sim_paths[sim]}/merged.root")
        # TODO: source root maybe
        os.system(f"hadd -f {sim_paths[sim]}/merged.root {sim_paths[sim]}/Root_Files/*")
        # print(f"hadd -f {sim_paths[sim]}/merged.root {sim_paths[sim]}/Root_Files/*")
        print("------------------- deletion")
        if len(glob(f"{sim_paths[sim]}/Root_Files/*")) == thread_count and delete:
            os.system(f"rm -r {sim_paths[sim]}/Root_Files/")
        elif delete:
            print("------------------- deletion skip")
            print(glob(f"{sim_paths[sim]}/Root_Files/*"))
            print(f"skipping deletion on {sim_paths[sim]} for root files because the number of files was wrong!!")
        


if __name__ == "__main__":
    main()
