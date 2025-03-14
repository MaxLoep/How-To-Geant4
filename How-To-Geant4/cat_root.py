import uproot
from sys import argv

#Get the key names of the folder structure in root file
def getKeys(file):
    #open root file
    _mainfile = uproot.open(file)
    # e.g. mainfile = uproot.open("build\Output\Root_Files\ID_15188.root")
    
    #Get substructures (keys) in the file
    _mainkeys = _mainfile.keys()
    # print(mainkeys) #for debugging
 
    #create a dictionary to store branch name and according key
    mydict = dict()
    
    #create empty array for branches
    _branches = []
    
    #Iterate over keys and save branches & key in dictionary
    for i in range(len(_mainkeys)):
        #Get current Key
        key = _mainfile[str(_mainkeys[i])]
        # print(key) #for debugging
        
        #Keys look like "Key;1" - this reforms the string by cutting off the ";1" for better readability
        _mainkeys[i] = _mainkeys[i].replace(';1', '')
        
        #Fill dictionary with keys
        mydict[str(_mainkeys[i])] = []
        
        #Fill dictionary with branches
        try:#if TTree
            for j in range(len(key.branches)):
                    _branches += [key.branches[j].name]
                    mydict[str(_mainkeys[i])].append(key.branches[j].name)
        except:#if not TTree
            pass

    return mydict


#Test if the Key is a TTree
def TestTTree(path_to_file, key):
    #open root file
    _mainfile = uproot.open(path_to_file)
    _key = _mainfile[key]
    
    # try to identify _key a TTree
    try:
        # this will raise an error if _key is no TTree
        _key.tree
        TTree = "(TTree)"
            
    #if _key is no TTree, it is a TH1D histogramm or a TH2D  
    except: 
        TTree = "(Not a TTree)"

    return TTree

def main():
    file = uproot.open(argv[1])
    for key in file.keys():
        if type(file[key]) == uproot.dynamic.Model_TTree_v5:
            print(file[key].arrays())
        else:
            print(file[key].axis().edges())
            print(file[key].values())



    key_dict = getKeys(argv[1])
    print("\nThe Structure in this root file looks like this:")
    for key in key_dict:
        print(key, TestTTree(argv[1],key), " - Branches: ", key_dict[key])


if __name__ == "__main__":
    main()
