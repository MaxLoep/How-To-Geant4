from sys import argv
import json
import urllib.request

def load_ext_periodic_table():
    # using periodic table data provided by periodictableofelements.org 
    req = urllib.request.Request('https://api.periodictableofelements.org/elements/')
    with urllib.request.urlopen(req) as response:
        return json.load(response)


def compact_table(ptoe):
    res = {}
    for element in ptoe:
        res[element["symbol"]] = {
            "mass": element["atomic_mass"],
            "atomic_number": element["atomic_number"]
        }

    return res


if __name__ == "__main__":
    out = argv[1]
    raw_table = load_ext_periodic_table()
    c_table = compact_table(raw_table)
    with open(out, "w") as file:
        json.dump(c_table, file)
