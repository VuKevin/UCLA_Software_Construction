#!/usr/bin/env python
# Kevin Vu | CS 35L | Winter 2015 | Josh | Hwk 3 Python Script
# comm.py -- The comm utility shall read file1 and file2,
    # which should be ordered in the current collating sequence,
    # and produce three text columns as output: "
    # lines only in file1, lines only in file2, and lines in both files
    
# Hwk: Rewriting a script -- Guidelines
    # Do it in the style of randline.py, build on from it!
    # Support all the options and operands required by POSIX
        # Options -1, -2, -3 are supported where option means to suppress output column
        # Operands file1, file2 are supported
        # If ONE of the files is '-', use the standard input is used
        # Option -123 means comm will write nothing to stdout
        # Not Option -1
            # Lines contained only in file 1 shall be included at beg of line
        # Not Option -2
            # Lines contained only in file 2 shall be included after 8 spaces
        # Not Option 3
            # Lines in both files shall be included after 16 spaces 
        # Stderror uses limited to diagnostic messages
        # Exit value of greater than 0 means an error occured
    # Report an error if given the wrong number of input != 2
    # Support -u for unsorted
            
import random
import sys 
import locale
from optparse import OptionParser

locale.setlocale(locale.LC_ALL, 'C')

class comm: 
    # Default Constructor w/ parameters
    def __init__(self, filename1, filename2): 
        
    # "-" is an indicator that you should read from stdin 
        if (filename1 == "-"): 
            # Read from stdin since file equals -
            f = sys.stdin
        else: 
            # Function to open, and subsequently read
            f = open(filename1, 'r') 
        self.lines1 = f.readlines()
        f.close()
        
        if (filename2 == "-"):
            g = sys.stdin
        else:
            g = open(filename2, 'r') 
        self.lines2 = g.readlines()
        g.close()
        

    # Define two accessor functions
    def columnI(self): 
        return self.lines1
    def columnII(self):
        return self.lines2
    
    # Define a function to determine sorted files
    def isSorted(self, filename, fLen): 
        index1 = 0
        while (index1 < (fLen - 1)):
            if (filename[index1] > filename[index1 + 1]):
                return False
            else:
                index1 = index1 + 1
        return True
    
# int main() {}
def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE FILE2
    Output up to 3 columns where:
    - Column index1 are lines unique to FILE
    - Column II are lines unique to FILE2
    - Column III are lines in both FILE and FILE2 
    - Options available include -1, -2, -3, and extra option -u for comparing unsorted files"""
    
    
# Optparse: Create an instance of OptionParser"

    # Declare OptionParser instance
    parser = OptionParser(version=version_msg, usage=usage_msg) 
    
    # Options set-up
    parser.add_option("-u", action="store_true", dest="unsorted", default=0, help="comparing unsorted files")
    parser.add_option("-1", action="store_true", dest="dashOne", default=0, help="Suppress the output column of lines unique to file1")
    parser.add_option("-2", action="store_true", dest="dashTwo", default=0, help="Suppress the output column of lines unique to file2.")
    parser.add_option("-3", action="store_true", dest="dashThree", default=0, help="Suppress the output column of lines duplicated in file1 and file2.")
    options, args = parser.parse_args(sys.argv[1:])

    # Error handling
    if (len(args)) != 2:
        parser.error("Invalid # of input files")
    if (args[0] == "-" and args[1] == "-"):
        parser.error("Both files cannot refer to standard input")

    input_file1 = args[0]
    input_file2 = args[1]
    unsorted = options.unsorted
    dashOne = options.dashOne
    dashTwo = options.dashTwo
    dashThree = options.dashThree
    sortError = ""
    msg = ""
    
    
    try:
        generator = comm(input_file1, input_file2)
        listOne = generator.columnI()
        listTwo = generator.columnII()

    #File DNE
    except IOError as err:
        errno, strerror = err.args
        parser.error("index1/O error({0}): {1}". format(errno, strerror))
       
    # Fix new line 
    if "\n" in listOne[len(listOne) - 1]:
        msg = "Newline detected"
        # sys.stdout.write(msg)
        pass
    else:
        listOne[len(listOne) - 1] += '\n'
    if "\n" in listTwo[len(listTwo) - 1]:
        msg = "Newline detected."
        # sys.stdout.write(msg)
        pass
    else:
        listTwo[len(listTwo) - 1] += '\n'
        
    #  If the -1, -2, and -3 options are all selected, comm shall write nothing to standard output.
    if (dashOne and dashTwo and dashThree): 
        return
    
# Case -> already sorted
    if (not unsorted):
        # CHECKPOINT: Confirm te files are sorted #
        #  TA: "It's ok that we just silently exit the program"
        # I chose to exit the program with an error message
        if generator.isSorted(listOne, len(listOne)):
            pass
        else:
            sortError = sortError + "The 1st input file is not sorted"
            sortError = sortError + "\n"
        if generator.isSorted(listTwo, len(listTwo)):
            pass
        else:
            sortError = sortError + "The 2nd input file is not sorted"
            sortError = sortError + "\n"
        # If they are NOT sorted, then the user should have used option -u, so return
        if sortError != "":
            sys.stdout.write(sortError)
            return
        # /CHECKPOINT #

        # Assemble and print depending on the options
        index1 = 0
        index2 = 0
        
        # Main Loop: Once either list runs out of elements
        # we will exit the loop
        while ((index1 < len(listOne)) and (index2 < len(listTwo))):
            if (listOne[index1] < listTwo[index2]):
                if (not dashOne):
                    sys.stdout.write(listOne[index1])
                index1 = index1 + 1
            elif (listOne[index1] > listTwo[index2]): 
                if (not dashTwo):
                    if (dashOne):
                        sys.stdout.write(listTwo[index2])
                    else:
                        sys.stdout.write("        " + listTwo[index2])
                index2 = index2 + 1
            else: # element should be in both input files
                if (not dashThree):
                    if (dashOne and dashTwo):
                        sys.stdout.write(listOne[index1])
                    elif (dashOne or dashTwo):
                        sys.stdout.write("        " + listOne[index1])
                    else:
                        sys.stdout.write("                " + listOne[index1])
                index1 = index1 + 1
                index2 = index2 + 1


        # Deal with the remaining elements in either listOne or listTwo 
            # Case: List 1 is still remaining and -1 is not enabled
        if (not dashOne): 
            if (index1 < len(listOne)):
                r = index1
                while (r < len(listOne)):
                    sys.stdout.write(listOne[r])
                    r = r + 1
            # Case: List 2 is still remaining and -2 is not enabled
        if (not dashTwo):
            if (index2 < len(listTwo)):
                r = index2
                while (r < len(listTwo)):
                    if (dashOne):
                        sys.stdout.write(listTwo[r])
                    else:
                        sys.stdout.write("        " + listTwo[r])
                    r = r + 1
                    
                    
# Case -> unsorted (b/c user decided to use option -)
    if (unsorted): 
        answer = ""

        if '\n' not in listOne[len(listOne) - 1]:
            listOne[len(listOne) - 1] += '\n'
        if '\n' not in listTwo[len(listTwo) - 1]:
            listTwo[len(listTwo) - 1] += '\n'

        for index1 in range(len(listOne)):
            flagFetch = False
            for index2 in range(len(listTwo)):
                if listOne[index1] == listTwo[index2]:
                    if not dashThree:
                        if dashOne and dashTwo:
                            sys.stdout.write(listOne[index1])
                        elif dashTwo or dashOne:
                            sys.stdout.write("        " + listOne[index1])
                        else:
                            sys.stdout.write("        " + "        " + listOne[index1])
                    listTwo.pop(index2)
                    flagFetch = True
                    break
            if not dashOne:
                if not flagFetch:
                    sys.stdout.write(listOne[index1])

        if (not dashTwo):
            for r in range(len(listTwo)):
                if (dashOne):
                    sys.stdout.write(listTwo[r])
                else:
                    sys.stdout.write("        " + listTwo[r])
    # 0 Indicates that everything was sucessful
    return 0 

# Kept from randline.py
if __name__ == "__main__":
    main()