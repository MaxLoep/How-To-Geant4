import uproot
from sys import argv


def main():
    file = uproot.open(argv[1])
    for key in file.keys():
        if type(file[key]) == uproot.dynamic.Model_TTree_v5:
            print(file[key].arrays())
        else:
            print(file[key].axis().edges())
            print(file[key].values())

if __name__ == "__main__":
    main()
