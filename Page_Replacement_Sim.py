"""
The purpose of this programming project is to explore page replacement algorithms.
This can be accomplished by developing a simple simulator that implements various page replacement algorithms and determining the number of 
page faults for a given reference string. 
A secondary objective of this assignment is to reinforce good software project design by using multiple source code modules in your solution.
"""

#**************************************************************
# Joseph Mace
# Operating Systems
# Programming Project #5: Simulation of Page Replacement Strategies
# April 7, 2025
#**************************************************************

import random

#**************************************************************
# First in first out functon

# This function uses the first in first out algorithm for page replacement

# Return Value is the number of faults

# Parameters is the number of frames and the reference string

# Local Variables is the list of frames and the number of faults
#**************************************************************

def FIFO (num_frames, ref_string):
    frames = []
    faults = 0
    
    for ref in ref_string:
        if ref not in frames:
            faults += 1
            
            if len(frames) < num_frames:
                frames.append(ref)
            else:
                frames.pop(0)
                frames.append(ref)
    
    return faults

#**************************************************************
# Least Recently Used

# This function uses the first in Least Recently Used alogorithm

# Return Value is the number of faults

# Parameters is the number of frames and the reference string

# Local Variables is the list of frames and the number of faults
#**************************************************************
    
def LRU (num_frames, ref_string):
    frames = []
    faults = 0
  
    for ref in ref_string:
        if ref not in frames:
            faults += 1
            
            if len(frames) < num_frames:
                # add ref to end of page if there is room
                frames.append(ref)
            else:
                # remove the first index and add the new ref to the last index
                frames.pop(0)
                frames.append(ref)
        else:
            # if ref is in frames we remove it and 
            # add it to the end to mark is as recently used
            frames.remove(ref)
            frames.append(ref)
            
    return faults

#**************************************************************
# First in Optimal Page Replacement

# This function uses the looks ahead and tracks the index of each item in ref_string
# if there is a fault detected then it replaces the item with the furthest index

# Return Value is the number of faults

# Parameters is the number of frames and the reference string

# Local Variables is the list of frames and the number of faults
# This function also uses a dictionary future_postions to store future positions 
# of the items currently in frames found in the reference string

# Also uses a varable farthest_frame that uses the max function to identify the
# highest index to find the item to replace 
#**************************************************************

def OPT (num_frames, ref_string):
    frames = []
    faults = 0
        
    for i, ref in enumerate(ref_string):
        if ref not in frames:
            faults += 1
            
            if len(frames) < num_frames:
                frames.append(ref)
            else:
                # Dictionary to store item with next occuring index
                future_positions = {}
                
                for j, frame in enumerate(frames):
                    # Find next position of each page
                    try:
                        future_positions[frame] = ref_string[i + 1].index(frame)
                    except:
                        # frame is not used again
                        future_positions[frame] = float('inf')
                        
                farthest_frame = max(future_positions, key = future_positions.get)
                
                frames[frames.index(farthest_frame)] = ref
                
    return faults

#**************************************************************
# Random replacement

# This function uses the Random Replacement method 

# Return Value is the number of faults

# Parameters is the number of frames and the reference string

# Local Variables are frames and faults (same as the other funcions)
# Also uses a random index variable to generate a random index within the bounds
# of the lenth of the page
#**************************************************************
    
def RAND (num_frames, ref_string):
    frames = []
    faults = 0
    
    for ref in ref_string:
        if ref not in frames:
            faults += 1
            
            if len(frames) < num_frames:
                frames.append(ref)
            else:
                random_index = random.randint(0, num_frames - 1)
                frames[random_index] = ref
    return faults

# -*- Main Program -*-

file = input("Input file name: ")

# Open the file and read data
with open(file, 'r') as f:
    lines = f.readlines()
    ref_string = list(map(int, lines[0].strip().split()))  # Reading reference string
    num_frames = int(lines[1].strip())  # Reading number of frames

print("Page Reference String: ", ref_string)
print ("Number of Frames: ", num_frames)

print("FIFO: ", FIFO(num_frames, ref_string))
print("LRU: ", LRU(num_frames, ref_string))
print("OPT: ", OPT(num_frames, ref_string))
print("RAND: ", RAND(num_frames, ref_string))
