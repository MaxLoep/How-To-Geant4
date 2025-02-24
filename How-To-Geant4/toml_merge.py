#!/usr/bin/env python3
# ./merger.py [OPTIONS] [files to merge] z.B.: ./merger.py -o output.txt files/*
# optionen: -o [file] : gibt output datei an --ignore-stable : ignoriert stabile Teilchen (noch nicht implementiert)

import tomllib
from sys import argv
import glob
import platform
from typing import Tuple
import re


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
        res.pcount += other.count
        return res


def to_dict(file: str) -> dict:
    res = {}
    with open(file, 'rb') as handle:
        dict = tomllib.load(handle)

        for name in dict:
            res[name] = particle_data(dict[name], name)
    return res


def add_abundance_info(data: dict) -> dict:
    primary = list(filter(lambda x: data[x].is_primary, data))

    secondaries = {}
    for particle in data:
        if not data[particle].is_primary:
            secondaries[particle] = data[particle]

    if len(primary) > 1:
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


def main():
    argc = len(argv)
    if argc < 2:
        print("please provide a pattern to find the files to merge")
        return

    output_file = "out.txt"
    if "-o" in argv:
        output_file = argv[argv.index("-o") + 1]

    ignore_stable = False
    if "--ignore-stable" in argv:
        ignore_stable = True

    files = argv[1:]
    if output_file in files:
        files.remove(output_file)
    if "-o" in files:
        files.remove("-o")
    if "--ignore-stable" in files:
        files.remove("--ignore_stable")

    # catch the case of windows
    if platform.system() == "Windows":
        files = glob.glob(files[0])

    master_dict = {}

    for file in files:
        file_content = to_dict(file)
        for particle in file_content:
            if particle in master_dict:
                try:
                    master_dict[particle] += file_content[particle]
                except:
                    print("skipped merge")
            else:
                master_dict[particle] = file_content[particle]

    master_dict = add_abundance_info(master_dict)

    with open(output_file, 'w') as out_handle:
        out_handle.writelines([str(master_dict[pd]) for pd in master_dict])
    #print(master_dict)


if __name__ == "__main__":
    main()
