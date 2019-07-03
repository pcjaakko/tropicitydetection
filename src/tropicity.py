#!/usr/bin/env python3.7

import os
import signal
import sys
import math
import subprocess
from argparse import ArgumentParser

import libtropicity as tr


def parseCommandline():
    parser = ArgumentParser(prog="tropicity.py", description="nothing yet", add_help=True)
    parser.add_argument("--input", help=" ", dest="inputfile", action="store", type=str, required=True)
    argparse = parser.parse_args()
    return argparse


def main():
    # signal.signal(signal.SIGINT, signal_handler)
    args = parseCommandline()
    print(args)
    # print('init S:')
    # print(args.source)
    C = tr.cube(args.inputfile)
    # print(S)
#    C = ap.canvas(args.out_filename, args.canvas_width, args.canvas_height)
#    C.bucket_fill(100,100,100)
#    C.render_scene(S)
#    C.highlight_edges()
##    C.draw_coast(S)
#    C.annotate_peaks(S)
##    C.annotate_islands(S)
#    C.label_axis(S)

if __name__ == "__main__":
    main()

