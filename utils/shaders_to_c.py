#!/usr/bin/python3

#
# Little script that converts a file into a C string
#

import argparse
import os

parser = argparse.ArgumentParser(
    prog="shaders_to_c",
    description="convert files into a C string",
    epilog="example: \npython3 utils/shaders_to_c.py --out-dir /tmp/shaders ./src/renderer/shaders/*")
parser.add_argument('--out-dir', type=str)
parser.add_argument('files', type=str, nargs='+')

def main(dest, files):

    if dest != None and not os.path.exists(dest):
        os.makedirs(dest)
    
    for filepath in files:
        f = open(filepath, "r")
        filename = os.path.split(filepath)[1]
        filename = filename.replace(".", "_")

        outname = os.path.join(dest, filename + ".c")
        out = open(outname, "w+")

        print("Generating " + outname)
        
        out.write("static const char* " + filename + " = ")
        for line in f:
            # Escape characters
            line.replace("\"", "\\\"")
            line.replace("\n", "\\n")
            line.replace("\\", "\\\\")
            
            out.write("\"" + line[:-1] + "\\n\"\n")

        out.write(";\n")

        out.close()
        f.close()

    print("Done")
        
if __name__ == "__main__":
    args = parser.parse_args()
    main(args.out_dir, args.files)
