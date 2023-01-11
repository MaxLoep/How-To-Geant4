# to run in Spyder console: runfile('PlotRootFile.py', args)

import uproot
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from scipy import optimize as op
import os
import math
import sys

path = "build\\Output\\Root Files\\ID_15188.root"

#Function to plot a T1HD Histogram from a root file with matplotlib - UNNICE PLOT
def plot_TH1D(name, title):
    # Plot stuff
    # Start a figure and set its size 
    fig = plt.figure(figsize=(6, 6))
    
    #create a 2x2 grid  (first is rows, second is columns) and add one ax (graphs are called axes) to position 1 
    ax1 = fig.add_subplot(1, 1, 1)
    
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
 
#Function to plot a TTree Histogram from a root file with matplotlib - UNNICE PLOT
def plot_TTree(data, TTreeFolder, TTreeBranch, bins): 
    #Plot stuff
    #Start a figure and set its size 
    fig = plt.figure(figsize=(6, 6))

    #create a 2x2 grid  (first is rows, second is columns) and add one ax (graphs are called axes) to position 1 
    ax1 = fig.add_subplot(1, 1, 1)
    counts_Ekin, bining_Ekin, patches_Ekin = ax1.hist(data, bins= bins, label="Counts: " + str(len(data)), zorder=10)
    
    #Set title for ax1
    ax1.set_title(str(TTreeFolder) + " - " +  str(TTreeBranch))
    
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
                

        #if key is no TTree, it is a TH1D histogramm    
        except: 
            print(str(mainkeys[i]) + " is not a TTree")
            #plot the histogram
            plot_TH1D(key, mainkeys[i])
    
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
        ax1.set_xlabel(" / cm")
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

        
#main function for different use cases
def main(arg):
    if arg == "every":
        plot_all(path)
    else:
        # print("arg not given")
        # plot_all(path)
        plot_one(path, arg, 100)

#do stuff if this script is called directly otherwise do nothing
if __name__ == "__main__":
    #if script is called without argument plot everything in Root File
    if len(sys.argv) == 1:
        main("every")
        
    else:
        try:
            # print(sys.argv[1])
            main(sys.argv[1])
            # plot_one(path, "SD1/2", 100)
        except:
            print("Argument is no valid Plotname")
    
         
    

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
