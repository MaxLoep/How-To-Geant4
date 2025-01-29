# to run in Spyder console: runfile('PlotRootFile.py', args)

#plot all
#runfile("PlotRootFile.py")
#runfile("PlotRootFile.py", "every")

#plot one single histo
#runfile("PlotRootFile.py", "single histo1")
#runfile("PlotRootFile.py", "single SD1/1")

#Plot Heatmap with two TTrees and cut data according to a third
#runfile("PlotRootFile.py", "heatmap histo1 histo2 'title' 'x-label' 'y-label' colorbarMIN colorbarMAX dataToCutWith  cutMIN cutMAX")
#runfile("PlotRootFile.py", "heatmap SD1/1 SD1/2 '' 'x-pos / cm' 'z-pos / cm' 1 1000 SD1/0 0 1")




import uproot                       #for reading .root files
import numpy as np                  #for nice arrays
import matplotlib                   #for plotting
import matplotlib.pyplot as plt     #for plotting
import sys                          #for getting argument if executet from console
import os
#from scipy import optimize as op
#import math


#Path to file
path = "How-To-Geant4\\build\Output\\range_N_100MeV_G4_Air\\Root_Files\\1737974101_737368000.root"
# path = "build\\Output\\Root Files\\28d-C-MobileCup-10PE.root"
# path = "build\\Output\\Root Files\\28d-C-MobileCup-20PE.root"
# path = "build\\Output\\Root Files\\ID_14964.root"
# path = "C26-5d_4_1_2_0.root"
# path = "C26-5d_4_1_3_0.root"
# path = "C26-5d_4_1_4_0.root"
# path = "C26-5d_4_2_3_0.root"
# path = "C26-5d_4_2_4_0.root"
# path = "C26-5d_4_2_5_0.root"
# path = "C26-5d_4_2_4_0_INCLXX_HP.root"

#Function to plot a TH1D Histogram from a root file with matplotlib
#UNNICE PLOT - no axis label, generic title, generic axis range
def plot_TH1D(name, title):
    print()
    
    # Plot stuff
    # Start a figure and set its size 
    fig = plt.figure(figsize=(6, 6))
    
    #create a 2x2 grid  (first is rows, second is columns) and add one ax (graphs are called axes) to position 1 
    ax1 = fig.add_subplot(1, 1, 1)
    
    #read bin edges and number of entries from file and plot
    counts_, bins_, _ = plt.hist((name.axis().edges()[:-1] + name.axis().edges()[1:]) / 2,
                                 bins=name.axis().edges(),
                                      weights=name.values(),
                                      range=(min(name.axis().edges()),
                                             max(name.axis().edges())),
                                      label="Counts: " + str(sum(name.values())) )
    #Set title for ax1
    ax1.set_title(str(title))
      
    # #Set axis labels for ax1
    # ax1.set_xlabel(" / cm")
    ax1.set_ylabel("counts")
    
    # Plot with grid
    ax1.grid()
     
    # Set legend for ax1
    ax1.legend(loc='best')
    
    #Show the figure
    plt.show()
      
    #Close the file/figure 
    plt.close()
 
# NOT WORKING
#Function to plot a TH2D Histogram from a root file with matplotlib
#UNNICE PLOT - no axis label, generic title, generic axis range
def plot_TH2D(name, title):   
    #Plot stuff
    #Start a figure and set its size 
    fig = plt.figure(figsize=(6, 6))
    # create a 1x2 grid and add one ax (graphs are called axes) to position 1 
    ax1 = fig.add_subplot(1,1,1)

    # color = 'Reds'
    color = 'rainbow'
    
    #read bin edges and number of entries from file and plot
    # counts_, bins_, _ = plt.hist((name.axis().edges()[:-1] + name.axis().edges()[1:]) / 2,
    #                               bins=name.axis().edges(),
    #                                   weights=name.values(),
    #                                   range=(min(name.axis().edges()),
    #                                           max(name.axis().edges())),
    #                                   label="Counts: " + str(sum(name.values())) )
    
    # h = ax1.hist2d(name.values(0), 
    #                 name.values(1), 
    #                 bins=[name.axis(0).edges(),
    #                       name.axis(1).edges()], 
    #                 cmap=color, norm=matplotlib.colors.LogNorm(vmin=1, vmax=100))
    
    h = ax1.hist2d(
                    (name.axis('x').edges()[:-1] + name.axis('x').edges()[1:]) / 2,
                    (name.axis('y').edges()[:-1] + name.axis('y').edges()[1:]) / 2,
                    # sum(name.values('x')),
                    # sum(name.values('y')),
                    # name.values(1).reshape(-1), 
                    # name.values(2).reshape(-1),
                    bins=100,
                    # bins=[name.axis('x').edges(),
                    #       name.axis('y').edges()],
                    # weights=[np.delete(sum(name.values('x')),0),
                    #          np.delete(sum(name.values('y')),0)],
                    # weights=np.array(name.values(0)),
                    # weights=name.values(0),
                    # weights=name.values('x').reshape(-1),
                    # weights=[sum(name.values('x')),
                    #           sum(name.values('y'))],
                    # range=[[min(name.axis(0).edges()), max(name.axis(0).edges())],
                    #       [min(name.axis(1).edges()), max(name.axis(1).edges())]],
                    cmap=color,
                    norm=matplotlib.colors.LogNorm(vmin=1, vmax=10))
    
    
    
    cb = fig.colorbar(h[3], ax=ax1)
    
    #Set title for ax1
    ax1.set_title(str(title))
      
    # #Set axis labels for ax1
    # ax1.set_xlabel(" / cm")
    ax1.set_ylabel("counts")
    
    # Plot with grid
    ax1.grid()
     
    # Set legend for ax1
    ax1.legend(loc='best')
    
    #Show the figure
    plt.show()
      
    #Close the file/figure 
    plt.close()
 
#Function to plot a TTree Histogram from a root file with matplotlib
#UNNICE PLOT - no axis label, generic title, generic axis range
def plot_TTree(data, TTreeFolder, TTreeBranch, bins): 
    #Plot stuff
    #Start a figure and set its size 
    fig = plt.figure(figsize=(6, 6))

    #create a 2x2 grid  (first is rows, second is columns) and add one ax (graphs are called axes) to position 1 
    ax1 = fig.add_subplot(1, 1, 1)
    
    #create histo from data
    counts_Ekin, bining_Ekin, patches_Ekin = ax1.hist(data, bins= bins, label="Counts: " + str(len(data)), zorder=10)
    
    #Set title for ax1
    ax1.set_title(str(TTreeFolder) + " - " +  str(TTreeBranch))
    
    # #Set axis labels for ax1
    # ax1.set_xlabel(" / cm")
    ax1.set_ylabel("counts")
    
    # Plot with grid
    ax1.grid()
     
    # Set legend for ax1
    ax1.legend(loc='best')
    
    #Show the figure
    plt.show()
      
    #Close the file/figure 
    plt.close()
  
#Plot all content of a Root file - UNNICE
def plot_all(path_to_file):
    #Plot the content of the following file
    mainfile = uproot.open(path_to_file)
    # mainfile = uproot.open("build\Output\Root Files\ID_15188.root")
    
    #Count substructures in the file
    mainkeys = mainfile.keys()
    print(str(len(mainkeys)) + " Substructures are in this file")
    
    #Keys look like "Key;1" - this reforms the string by cutting off the ";1"
    for i in range(len(mainkeys)):
        mainkeys[i] = mainkeys[i].replace(';1', '')
    
    #Iterate over keys and plot the content
    for i in range(len(mainkeys)):
        #Get current Key
        key = mainfile[str(mainkeys[i])]
        
        #try to identify key a TTree
        try:
            #this will raise an error if key is no TTree
            key.tree
            print(str(mainkeys[i]) + " is a TTree")
            
            #iterate over branches of TTree
            for j in range(len(key.branches)):
                # print(j)
                print(key.branches[j].name + " is a branch of " + mainkeys[i] + "\nIt's path is \n" + 
                      str(mainkeys[i]) + "/" + str(j) +"\n")
                #read data of branch in array
                array = mainfile[mainkeys[i]].branches[j].array()
                
                #plot the TTreeBranch data as histogramm
                plot_TTree(array, mainkeys[i], mainfile[mainkeys[i]].branches[j].name, bins=100)
                

        #if key is no TTree, it is a TH1D histogramm or a TH2D  
        except: 
            # print(str(mainkeys[i]) + " is not a TTree")
            try:              
                #plot TH1D histogram
                plot_TH1D(key, mainkeys[i])
                print(str(mainkeys[i]) + " is a TH1D")
            except:
                # plot_TH2D(key, mainkeys[i])
                print(str(mainkeys[i]) + " is a maybe a TH2D")
                
    
#Plot one Plot but nicely - with label, name, axis, etc.
def plot_one(path_to_file, histo_path, bins ):
    mainfile = uproot.open(path_to_file)
    mainkey = histo_path[:histo_path.rfind("/")]
    j = int(histo_path[histo_path.rfind("/")+1:])
    # #read data of branch in array
    array = mainfile[mainkey].branches[j].array()
    try:
        # this will raise an error if key is no TTree
        key = mainfile[str(mainkey)]
        key.tree
        #plot the TTreeBranch data as histogramm
        # plot_TTree(array, mainkey, mainfile[mainkey].branches[j].name, bins=bins)
        #Plot stuff
        #Start a figure and set its size 
        fig = plt.figure(figsize=(6, 6))

        #create a 2x2 grid  (first is rows, second is columns) and add one ax (graphs are called axes) to position 1 
        ax1 = fig.add_subplot(1, 1, 1)
        counts_Ekin, bining_Ekin, patches_Ekin = ax1.hist(array, bins= bins, label="Counts: " + str(len(array)), zorder=10)
        
        #Set title for ax1
        ax1.set_title(str(mainkey) + " - " +  str(mainfile[mainkey].branches[j].name))
        
        # #Set axis labels for ax1
        # ax1.set_xlabel("z-pos / cm")
        ax1.set_xlabel("E / MeV")
        ax1.set_ylabel("counts")
        
        #Set x- and y-range
        # ax1.set_xlim(-5,5)
        ax1.set_xlim(0.01,1)
        ax1.set_ylim(0, 250)
        
        # Plot with grid
        ax1.grid()
         
        # Set legend for ax1
        ax1.legend(loc='best')
        
        # plt.savefig('Plot0.pdf')
        
        #Show the figure
        plt.show()
          
        #Close the file/figure 
        plt.close()
        
    #if key is no TTree, it is a TH1D histogramm    
    except: 
        print(str(mainkey) + " is not a TTree")
        #plot the histogram
        # plot_TH1D(key, mainkey)
        # Plot stuff
        # Start a figure and set its size 
        fig = plt.figure(figsize=(6, 6))
        
        #create a 2x2 grid  (first is rows, second is columns) and add one ax (graphs are called axes) to position 1 
        ax1 = fig.add_subplot(1, 1, 1)
        
        counts_, bins_, _ = plt.hist((key.axis().edges()[:-1] + key.axis().edges()[1:]) / 2,
                                     bins=key.axis().edges(),
                                          weights=key.values(),
                                          range=(min(key.axis().edges()),
                                                 max(key.axis().edges())),
                                          label="Counts: " + str(sum(key.values())) )
        #Set title for ax1
        ax1.set_title(str(mainkey))
        
        # #Set axis labels for ax1
        # ax1.set_xlabel(" / cm")
        ax1.set_ylabel("counts")
        
        #Set x- and y-range
        # ax1.set_xlim(1,2.5)
        #ax1.set_ylim(0, 15)
        
        # Plot with grid
        ax1.grid()
         
        # Set legend for ax1
        ax1.legend(loc='best')
        
        # plt.savefig('Plot0.pdf')
        
        #Show the figure
        plt.show()
          
        #Close the file/figure 
        plt.close()
 
#Plot one Heatmap but nicely - with label, name, axis, etc.
def plot_heatmap(path_to_file, histo_path_1, histo_path_2, *args):
    #List of args:
    # args[0]=title
    # args[1]=xlabel
    # args[2]=ylabel
    # args[3]=colorbar min
    # args[4]=colorbar max
    # args[5]=data1 to cut with
    # args[6]=cut min value
    # args[7]=cut max value
    # args[8]=data2 to cut with
    # args[9]=cut min value
    # args[10]=cut max value
    
    mainfile = uproot.open(path_to_file)
    mainkey1 = histo_path_1[:histo_path_1.rfind("/")]
    j = int(histo_path_1[histo_path_1.rfind("/")+1:])
    # #read data of branch in array
    data1 = np.array(mainfile[mainkey1].branches[j].array())
    # print(data1)
    
    mainkey2 = histo_path_2[:histo_path_2.rfind("/")]
    k = int(histo_path_2[histo_path_2.rfind("/")+1:])
    # #read data of branch in array
    data2 = np.array(mainfile[mainkey2].branches[k].array())
    # print(data2)
 
    if len(args) >= 5 and len(args)<9:
        try:
            mainkey3 = args[5][:args[5].rfind("/")]
            l = int(args[5][args[5].rfind("/")+1:])
            # #read data of branch in array
            data3 = np.array(mainfile[mainkey3].branches[l].array())
            # print(data3)
            
            data1_clean = []
            for i in range(len(data3)):
                if (data3[i] > float(args[6])) and (data3[i] < float(args[7])):
                    data1_clean += [data1[i]]
            
            data2_clean = []
            for i in range(len(data3)):
                if (data3[i] > float(args[6])) and (data3[i] < float(args[7])):
                    data2_clean += [data2[i]]
        except IndexError:
            print("Missing arguments for cutting data")
            
    if len(args) >= 9:
        try:
            mainkey3 = args[5][:args[5].rfind("/")]
            l = int(args[5][args[5].rfind("/")+1:])
            # #read data of branch in array
            data3 = np.array(mainfile[mainkey3].branches[l].array())
            
            mainkey4 = args[8][:args[8].rfind("/")]
            l = int(args[8][args[8].rfind("/")+1:])
            # #read data of branch in array
            data4 = np.array(mainfile[mainkey4].branches[l].array())
            # print(data3)
            
            data1_clean = []
            for i in range(len(data4)):
                if (data3[i] > float(args[6])) and (data3[i] < float(args[7])) and (data4[i] > float(args[9])) and (data4[i] < float(args[10])):
                    data1_clean += [data1[i]]
            
            data2_clean = []
            for i in range(len(data3)):
                if (data3[i] > float(args[6])) and (data3[i] < float(args[7])) and (data4[i] > float(args[9])) and (data4[i] < float(args[10])):
                    data2_clean += [data2[i]]
        except IndexError:
            print("Missing arguments for cutting data")
    
    # try:
        # this will raise an error if key is no TTree
    key1 = mainfile[str(mainkey1)]
    key1.tree
    key2 = mainfile[str(mainkey2)]
    key2.tree
    #Plot stuff
    #Start a figure and set its size 
    fig = plt.figure(figsize=(6, 6), dpi=200)
    # create a 1x2 grid and add one ax (graphs are called axes) to position 1 
    ax1 = fig.add_subplot(1,1,1)

    # color = 'Reds'
    color = 'rainbow'
    bins = 50

    # print("Number of entries in heatmap: " + str(len(data1_clean)))
    # print("Mean of Data1: " + str(np.mean(data1_clean)))
    # print("Mean of Data2: " + str(np.mean(data2_clean)))

    try:
        try:
            h = ax1.hist2d(data1_clean, data2_clean, bins=bins, cmap=color, norm=matplotlib.colors.LogNorm(vmin=args[3], vmax=args[4]))
            print("Generating heatmap with cuts applied to data...")
            print("Generating heatmap with given scaling...")
            print(h[0].max())
        except IndexError:
            h = ax1.hist2d(data1_clean, data2_clean, bins=bins, cmap=color, norm=matplotlib.colors.LogNorm())
            print("Generating heatmap with cuts applied to data...")
            print("Autoscaling Heatmap...")
            print(h.max())
            
    except:
        try:
            h = ax1.hist2d(data1, data2, bins=bins, cmap=color, norm=matplotlib.colors.LogNorm(vmin=args[3], vmax=args[4]))
            print("Generating heatmap with NO cuts applied to data...")
            print("Generating heatmap with given scaling...")
            print(h.max())
        except IndexError or ValueError:
            h = ax1.hist2d(data1, data2, bins=bins, cmap=color, norm=matplotlib.colors.LogNorm())
            print("Generating heatmap with NO cuts applied to data...")
            print("Autoscaling Heatmap...")
    
    cb = fig.colorbar(h[3], ax=ax1)
    # cb.set_label('Number of entries')
    
    if len(args) >= 1:
        #Set title for ax3
        ax1.set_title(str(args[0]))
        # print(args[0])
        
    
    if len(args) >= 2:
        #Set axis labels for ax3
        ax1.set_xlabel(str(args[1]))
        ax1.set_ylabel(str(args[2]))
        # print(args)
     
    #Plot with grid
    ax1.grid()
    # plt.savefig(str(args[0])+'.pdf')
    
    x1=np.array([-30,30])
    y1=np.array([30,30])
    ax1.plot(x1, y1, 'black', linestyle='-', marker='', label="Collimator Dimensions")
    ax1.plot(x1, y1*(-1), 'black', linestyle='-', marker='')
    ax1.plot(y1, x1, 'black', linestyle='-', marker='')
    ax1.plot(y1*(-1), x1, 'black', linestyle='-', marker='')
    
    # Set legend for ax1
    # ax1.legend(loc='best')
    
    plt.show()

    plt.close(fig)

    #GET NEUTRON DAMAGE FUNCTION DATA------------------------------------------
    txt_path = os.path.join(os.path.dirname(__file__))
    neutrons = os.path.join(txt_path, 'neutrons.txt')   
    neutrons_data  = np.loadtxt(neutrons, usecols = (0,1), skiprows=1).flatten() 
    neutrons_xdata = neutrons_data[0::2]
    neutrons_ydata = neutrons_data[1::2]
    
    
    neutrons_centroids = (neutrons_xdata[:-1] + neutrons_xdata[1:]) / 2
    neutrons_bins = np.insert(neutrons_centroids, 0, 0)
    neutrons_bins = np.append(neutrons_bins, 9.02E+03)
    
    # print(neutrons_xdata)
    # print(len(neutrons_xdata))
    # print(neutrons_ydata)
    # print(len(neutrons_ydata))
    # print(neutrons_centroids)
    # print(neutrons_bins)
    # print(len(neutrons_bins))



    #PLOT 1D HISTO OF FIRST TTree----------------------------------------------    
    #Plot stuff
    #Start a figure and set its size 
    fig2 = plt.figure(figsize=(6, 6), dpi=200)
    # create a 1x2 grid and add one ax (graphs are called axes) to position 1 
    ax2 = fig2.add_subplot(1,1,1)

    bins = 50  
    
    data1_clean = np.array(data1_clean) #/ 3000000000 *1E-6 / 1.6E-19

    #create histogram from TTree
    counts_data1, bining_data1, patches_data1 = ax2.hist(data1_clean, bins= bins, label="Counts: " + str(len(data1_clean)), zorder=10)
    
    #create histogram with given bining for calculating neutron damage factor
    # counts_data1, bining_data1, patches_data1 = ax2.hist(data1_clean, bins= neutrons_bins, label="Counts: " + str(len(data1_clean)), zorder=10)
    plt.close()
    
    #Plot stuff
    #Start a figure and set its size 
    fig2 = plt.figure(figsize=(6, 6), dpi=200)
    # create a 1x2 grid and add one ax (graphs are called axes) to position 1 
    ax2 = fig2.add_subplot(1,1,1)
    
    #replot above histogram from TTree
    counts_, bins_, _ = ax2.hist((bining_data1[:-1] + bining_data1[1:]) / 2,
                                  bins=bining_data1,
                                        weights=counts_data1, #/ 3000000000 *1E-6 / 1.6E-19,
                                       # weights=counts_data1/sum(counts_data1),
                                      range=(min(bining_data1),
                                              max(bining_data1)),
                                       # label="Counts: " + str(int(len(data1_clean)  )))  #/ 3000000000 *1E-6 / 1.6E-19)) )
                                       # label="QGSP_BIC_AllHP"  )
                                       label="QGSP_INCLXX_HP"  )
    
    print(sum(counts_))
    # print(sum(counts_*neutrons_ydata))
    
    
    #Plot with grid
    ax2.grid()
    #Set title for ax3
    # ax1.set_title(str(args[0])) 
    #Set axis labels for ax3
    ax2.set_xlabel("E / MeV")
    # ax2.set_xlabel("x / cm")
    ax2.set_ylabel("counts")
    
    #Set x- and y-range
    # ax2.set_xlim(0,20)
    
    
    # Set legend for ax1
    ax2.legend(loc='best')
    
    plt.savefig('Energy Neutrons.pdf')
    plt.show()
    plt.close(fig2)
    
    
    #Set legend for ax3
    # ax1.legend(loc='best')
    # except:
    #     print("Something went wrong with heatmap")
    #     print(histo_path_1)
    #     print(histo_path_2)

        
#main function for different use cases
def main(plottype, *args):
    #if argument is "every" then plot everything in the root file
    if plottype == "every":
    # if "every" in args:
        print("Plotting every plot in the Root file:")
        plot_all(path)
    elif plottype == "single":
    # elif "single" in args:
        print("Plotting a single plot:")
        # print(args[0])
        plot_one(path, args[0], 1000)
    elif plottype == "heatmap":
    # elif "heatmap" in args:
        print("Plotting a heatmap:")
        plot_heatmap(path, args[0], args[1], *args[2:])
    else:
        print("Else-case")
        print("Try to keep it like")
        print("PlotRootFile.py every")
        print("PlotRootFile.py single pathname")
        print("PlotRootFile.py heatmap pathname1 pathname2 title")

#do stuff if this script is called directly otherwise do nothing
if __name__ == "__main__":
    #if script is called without argument sys.argv only contains one element (the program name)
    #execute main function to plot everything in Root File
    if len(sys.argv) == 1:
        main("every")
        
    #if arguments are given, try to execute main function with given arguments
    else:
        # try:
            # print(sys.argv)
            # print(sys.argv[1])
            # print(sys.argv[2])
            # main(sys.argv)
            main(sys.argv[1], *sys.argv[2:])
            # plot_one(path, "SD1/2", 100)
        # print error message
        # except:
        #     print("Ooops something is wrong. Maybe invalid arguments")
        #     print("Try to keep it like")
        #     print("PlotRootFile.py every")
        #     print("PlotRootFile.py single pathname")
        #     print("PlotRootFile.py heatmap pathname1 pathname2 title")
    
         
    

#Get TTREE data
# file = uproot.open("RunData_2.root")["Total"]
# file.show()

# Ekin = file["Ekin"].array()
# print(str(len(Ekin)) + ' Total entries in file')


    
    
# #Check if output file already exists otherwise create it filled with data as numpy array
# if os.path.isfile(filepath + '/' + 'Ekin' + filename +'.npy'):
#     print('File already exists.')
    
#     mainfile = uproot.open("RunData_0.root")
    
#     mainkeys = mainfile.keys()
    
    
#     file = uproot.open("RunData_0.root")["Total"]
#     #file.show()
    
#     Ekin = file["Ekin"].array()
#     print(str(len(Ekin)) + ' Total entries in file')

# else: 
#     mainfile = uproot.open("RunData_0.root")
    
#     mainkeys = mainfile.keys()
#     print(mainkeys)
    
    
#     file = uproot.open("RunData_0.root")["SD1"]
#     #file.show()
    
#     Ekin = file["Ekin"].array()
#     xpos = file["Xpos"].array()
#     ypos = file["Ypos"].array()
    
#     Ekin_clean = []
#     xpos_clean = []
#     ypos_clean = []
    
    
#     # print(len(Ekin))
    
#     # for i in range(len(Ekin)):
#     #     print(Ekin[i], xpos[i], ypos[i])
    
#     for i in range(len(Ekin)):
#         if (Ekin[i] > 0) and (abs(angle(xpos[i], dist)) < angular_cut) and (abs(angle(ypos[i], dist)) < angular_cut):
#             Ekin_clean += [Ekin[i]]
#             xpos_clean += [xpos[i]]
#             ypos_clean += [ypos[i]]
            
#         if i%10000 == 0  :
#             print(str(i/(len(Ekin)) *100) + ' percent done of writing in file!')
            
#     Ekin_clean = np.array(Ekin_clean)
#     xpos_clean = np.array(xpos_clean)
#     ypos_clean = np.array(ypos_clean)
    
#     # print(len(Ekin_clean))
#     # print(len(xpos_clean))
#     # print(len(ypos_clean))

#     np.save(arr=Ekin_clean, file=filepath + '/' + 'Ekin' + filename +'.npy')
#     np.save(arr=xpos_clean, file=filepath + '/' + 'xpos' + filename +'.npy')
#     np.save(arr=ypos_clean, file=filepath + '/' + 'ypos' + filename +'.npy')

# Ekin = np.load(filepath + '/' + 'Ekin' + filename +'.npy')
# xpos = np.load(filepath + '/' + 'xpos' + filename +'.npy')
# ypos = np.load(filepath + '/' + 'ypos' + filename +'.npy')

# print(str(len(xpos)) + ' Cut entries in file')

# Ekin_cut = []
# xpos_cut = []
# ypos_cut = []
# xangular = []
# yangular = []

# for i in range(len(Ekin)):
# # for i in range(100000):
#     xangular += [angle(xpos[i], dist)]
#     yangular += [angle(ypos[i], dist)]
    
#     if i%100000 == 0  :
#         print(str(round(i/(len(Ekin)) *100, 3)) + ' percent done of calculating angle!')



# for j in angular_cut_arr:

#     angular_cut = j
    
#     Ekin_cut = []
#     xpos_cut = []
#     ypos_cut = []
#     xangular_cut = []
#     yangular_cut = []
        
#     for i in range(len(Ekin)):
#     # for i in range(100000):
#         if (abs(angle(xpos[i], dist)) < angular_cut) & (abs(angle(ypos[i], dist)) < angular_cut) :
#             Ekin_cut += [Ekin[i]]
#             xpos_cut += [xpos[i]]
#             ypos_cut += [ypos[i]]
#             xangular_cut += [angle(xpos[i], dist)]
#             yangular_cut += [angle(ypos[i], dist)]
        
#         if i%100000 == 0  :
#             print(str(i/(len(Ekin)) *100) + ' percent done of cutting data!')
            
    # print(len(xpos_cut))
    
    # #-----------------------------------------------
    #Plot stuff
    #Start a figure and set its size 
    # fig = plt.figure(figsize=(6, 6))
    # #-----------------------------------------------
    # # #create a 2x2 grid  (first is rows, second is columns) and add one ax (graphs are called axes) to position 1 
    # ax1 = fig.add_subplot(1, 1, 1)
    
    # # Plot Histogram
    # counts_Ekin, bining_Ekin, patches_Ekin = ax1.hist(Ekin_cut, bins= 75, label=str(len(Ekin))+' entries', zorder=10)
    
    # #ax1.hist(Ekin_cut, bins= bining_Ekin, label=str(len(Ekin_cut))+' entries', zorder=20)
    
    # #Set title for ax3
    # # ax1.set_title('Ekin of neutrons at ' + str(angular_cut) + ' degree')
    # ax1.set_title('Energy distribution of neutrons emerging from the target under 2.6° \n produced by a 26.5 MeV deuteron beam on a thick Carbon target')
    
    # # #Set axis labels for ax3
    # ax1.set_xlabel("Ekin / MeV")
    # ax1.set_ylabel("arbitrary units")
    
    # #Set x- and y-range
    # ax1.set_xlim(-2,2)
    # #ax1.set_ylim(0, 15)
    
    #Plot with grid
    # ax1.grid()
     
    #Set legend for ax3
    # ax1.legend(loc='best')
    
    
    
    #-----------------------------------------------
    
    #create a 1x2 grid and add one ax (graphs are called axes) to position 1 
    # ax3 = fig.add_subplot(1,2,1)
    
    
    # h = ax3.hist2d(xangular, Ekin, bins=100, cmap='Reds', norm=matplotlib.colors.LogNorm())
    
    # cb = fig.colorbar(h[3], ax=ax3)
    # cb.set_label('Number of entries')
    
    # #Set title for ax3
    # ax3.set_title('Ekin-angular-Plot ' + str(angular_cut) + ' degree')
    
    # #Set axis labels for ax3
    # ax3.set_xlabel("angle")
    # ax3.set_ylabel("Ekin")
     
    # #Plot with grid
    # ax3.grid()
    
    # #Set legend for ax3
    # ax3.legend(loc='best')
    
    #-----------------------------------------------
    # create a 1x2 grid and add one ax (graphs are called axes) to position 1 
    # ax5 = fig.add_subplot(1,2,2)
    
    
    # h = ax5.hist2d(xangular, Ekin, bins=100, cmap='Reds', norm=matplotlib.colors.LogNorm())
    
    # cb = fig.colorbar(h[3], ax=ax5)
    # cb.set_label('Number of entries')
    
    # #Set title for ax3
    # ax5.set_title('Ekin-angular-Plot ' + str(angular_cut) + ' degree')
    
    # #Set axis labels for ax3
    # ax5.set_xlabel("Ekin")
    # ax5.set_ylabel("angular")
     
    # #Plot with grid
    # ax5.grid()
    
    # #Set legend for ax3
    # ax5.legend(loc='best')
    
    # -----------------------------------------------
    
    #create a 1x2 grid and add one ax (graphs are called axes) to position 1 
    # ax2 = fig.add_subplot(2,2,3)
    
    # #Plot Histogram
    # counts_x , bining_x , patches_x = ax2.hist(xpos, bins= 1000, label=str(len(xpos))+' entries', zorder=10)
    
    # ax2.hist(xpos_cut, bins=bining_x, label=str(len(xpos_cut))+' entries', zorder=20)
    
    # x_data = []
    # counts_x_clean = []
    # fitlimit = 50
    
    # for i in range(len(bining_x)-1):
    #     if ((bining_x[i] + bining_x[i+1])/2 > -fitlimit)  & ((bining_x[i] + bining_x[i+1])/2 < fitlimit):
    #         x_data += [ (bining_x[i] + bining_x[i+1])/2 ]
    #         counts_x_clean += [counts_x[i]]
            
    
    
    # gaus_i, gauserror_i = op.curve_fit(function_gaus,
    #                                           #function_breitwigner,
    #                                           x_data, counts_clean,
    #                                           p0=[10000, 0 , 0.5, 100],
    #                                           maxfev=9999999)
    
    # gaus2_i, gaus2error_i = op.curve_fit(function_gaus2,
    #                                           #function_breitwigner,
    #                                           x_data, counts_x_clean,
    #                                           #p0=[10000, 0 , 0.5, 100],
    #                                           maxfev=9999999)
    
    # print(gaus2_i[2])
    # print(gaus2_i[6])
    
    # bw_i, bwerror_i = op.curve_fit(function_breitwigner,
    #                                           x_data, counts_clean,
    #                                           p0=[10000, 0 , 0.5, 100],
    #                                           maxfev=9999999)
    
    #Create linear space for fit of function to plot- here: create 100 points from 0 to 5
    # x_plot = np.linspace(-50, 50, 100) 
    
    #print(fitpara_i[3])
    
    #Plot fit function in linear space
    # ax2.plot(x_plot, function_gaus(x_plot, gaus_i[0], gaus_i[1], gaus_i[2], gaus_i[3]),
    #                           'r-',
    #                           label='Gauss',
    #                           zorder = 100)
    
    # ax2.plot(x_plot, function_gaus2(x_plot, gaus2_i[0], gaus2_i[1], gaus2_i[2], gaus2_i[3], gaus2_i[4], gaus2_i[5], gaus2_i[6]),
    #                           'r-',
    #                           label='Gaus+Gaus',
    #                           linewidth=3,
    #                           zorder = 100)
    
    # ax2.plot(x_plot, function_gaus(x_plot, gaus2_i[0], gaus2_i[1], gaus2_i[2], gaus2_i[3]),
    #                           'g-',
    #                           label='Gauss1',
    #                           zorder = 100)
    
    # ax2.plot(x_plot, function_gaus(x_plot, gaus2_i[4], gaus2_i[5], gaus2_i[6], gaus2_i[3]),
    #                           'y-',
    #                           label='Gauss2',
    #                           zorder = 100)
    
    # ax2.plot(x_plot, function_breitwigner(x_plot, bw_i[0], bw_i[1], bw_i[2], bw_i[3]),
    #                           'g-',
    #                           label='Breitwigner',
    #                           zorder = 100)
    
    #Set title for ax3
    # ax2.set_title('x-pos ' + str(angular_cut) + ' degree')
    
    # #Set axis labels for ax3
    # ax2.set_xlabel("x-pos / cm")
    # ax2.set_ylabel("entries")
    
    # #Set x- and y-range
    # #ax2.set_xlim(-2,2)
    # #ax1.set_ylim(0, 15)
     
    # #Plot with grid
    # ax2.grid()
    
    # #Set legend for ax3
    # ax2.legend(loc='best')
    
    # #-----------------------------------------------
    
    #create a 1x2 grid and add one ax (graphs are called axes) to position 1 
    # ax4 = fig.add_subplot(1,1,1)
    
    # #Plot Histogram
    # counts_x , bining_x , patches_x = ax4.hist(xangular, bins= 100, label=str(len(xangular))+' entries', zorder=10)
    
    # ax4.hist(xangular_cut, bins=bining_x, label=str(len(xpos_cut))+' entries', zorder=20)
    
    # x_data = []
    # counts_x_clean = []
    # fitlimit = 6
    
    # for i in range(len(bining_x)-1):
    #     if ((bining_x[i] + bining_x[i+1])/2 > -fitlimit)  & ((bining_x[i] + bining_x[i+1])/2 < fitlimit):
    #         x_data += [ (bining_x[i] + bining_x[i+1])/2 ]
    #         counts_x_clean += [counts_x[i]]
            
    
    
    # gaus_i, gauserror_i = op.curve_fit(function_gaus,
    #                                           #function_breitwigner,
    #                                           x_data, counts_clean,
    #                                           p0=[10000, 0 , 0.5, 100],
    #                                           maxfev=9999999)
    
    # gaus2_i, gaus2error_i = op.curve_fit(function_gaus2,
    #                                           #function_breitwigner,
    #                                           x_data, counts_x_clean,
    #                                           #p0=[10000, 0 , 0.5, 100],
    #                                           maxfev=9999999)
    
    # print(gaus2_i[2])
    # print(gaus2_i[6])
    
    # bw_i, bwerror_i = op.curve_fit(function_breitwigner,
    #                                           x_data, counts_clean,
    #                                           p0=[10000, 0 , 0.5, 100],
    #                                           maxfev=9999999)
    
    #Create linear space for fit of function to plot- here: create 100 points from 0 to 5
    # x_plot = np.linspace(-2, 2, 100) 
    
    #print(fitpara_i[3])
    
    #Plot fit function in linear space
    # ax2.plot(x_plot, function_gaus(x_plot, gaus_i[0], gaus_i[1], gaus_i[2], gaus_i[3]),
    #                           'r-',
    #                           label='Gauss',
    #                           zorder = 100)
    
    # ax2.plot(x_plot, function_gaus2(x_plot, gaus2_i[0], gaus2_i[1], gaus2_i[2], gaus2_i[3], gaus2_i[4], gaus2_i[5], gaus2_i[6]),
    #                           'r-',
    #                           label='Gaus+Gaus',
    #                           zorder = 100)
    
    # ax2.plot(x_plot, function_gaus(x_plot, gaus2_i[0], gaus2_i[1], gaus2_i[2], gaus2_i[3]),
    #                           'g-',
    #                           label='Gauss1',
    #                           zorder = 100)
    
    # ax2.plot(x_plot, function_gaus(x_plot, gaus2_i[4], gaus2_i[5], gaus2_i[6], gaus2_i[3]),
    #                           'y-',
    #                           label='Gauss2',
    #                           zorder = 100)
    
    # ax2.plot(x_plot, function_breitwigner(x_plot, bw_i[0], bw_i[1], bw_i[2], bw_i[3]),
    #                           'g-',
    #                           label='Breitwigner',
    #                           zorder = 100)
    
    #Set title for ax3
    # ax4.set_title('x-angular ' + str(angular_cut) + ' degree')
    # ax4.set_title("Angular Distribution of neutrons emerging from the target")
    
    # # #Set axis labels for ax3
    # ax4.set_xlabel("theta / degree")
    # ax4.set_ylabel("arbitrary units")
    
    # #Set x- and y-range
    # #ax2.set_xlim(-2,2)
    # #ax1.set_ylim(0, 15)
     
    # #Plot with grid
    # ax4.grid()
    
    #Set legend for ax3
    # ax4.legend(loc='best')
    #-----------------------------------------------
    
    #Create an output file to save the figure directly
    # plt.savefig(str(j) + 'degree.pdf')
    # plt.savefig('Plot0.pdf')
    
    # #Show the figure
    # plt.show()
      
    # #Close the file/figure 
    # plt.close()
    
    #Set x- and y-range
    # ax1.set_xlim(1,2.5)
    #ax1.set_ylim(0, 15)
    
    # plt.savefig('Plot0.pdf')
