#!python
"""TinkerBelle testpattern"""


import png
import numpy
import sys
import argparse
import serial
from collections import deque
# import time

STEPS_PER_ROW = 12  # Dunno :D
# NOTE THAT THE LOW BITS ARE CLOSER TO THE HEAD!
DEFAULT_BAUDRATE = 57600


def get_bitmap(filename):
    r = png.Reader(filename="ringtest.png")
    img = r.asDirect()
    bitmap = [[x for x in y] for y in img[2]]
    bitmap = [[{0: 0,255: 1}[x] for x in y] for y in bitmap]
    return bitmap


def xbytes_to_word(pixels):
    """
    When give a list of pixels, from first to last turn them into
    a word. First pixel is lowest value.
    """
    word = 0
    value = 1
    for pixel in pixels:
        if pixel:
            word += value
        value = value << 1
    return word


def strips_to_stripe(strips, bph):
    """
    Take strips, a list of rows, most to least (natural bitmap) value
    and turn them into a stripe of words, each a vertical line
    single bit packed. We reverse the strips when we make them btw.
    """
    # Reverse the order...
    #print "F", strips
    rstrips = strips[::-1]
    #print "R", rstrips
    # prep the result field
    output = list()

    # Then we iterate them.
    while len(rstrips[0]):
        output.append(
            xbytes_to_word(
                [strip.pop() for strip in rstrips]))
    return output


def bitmap_to_stripes(bitmap, xres, yres, bph):
    assert yres % bph == 0
    #print "B",bitmap
    for strip in xrange(yres/bph):
        print "Calculating strip:",strip
        print "offset:", strip*bph,":",(strip+1)*bph
        yield strips_to_stripe(bitmap[strip*bph:(strip+1)*bph], bph)


def configself():
    "FOO"
    if "darwin" in sys.platform.lower():
        defaultserial = "/dev/tty.usbserial-A900F9XQ"
    elif "win" in sys.platform.lower():
        defaultserial = "COM4"
    elif "linux" in sys.platform.lower():
        defaultserial = "/dev/ttyUSB0"
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("pngfile", metavar="pngfile", default=None,
                        help="PNG file to load for layer duplication.")
    parser.add_argument("-p" "--serialport", dest="serialport",
            help="Serial port to use.", default=defaultserial)
    parser.add_argument("-b", "--baud",
        dest="baudrate",
        type=int,
        help="set baud rate, default %s" % DEFAULT_BAUDRATE,
        default=DEFAULT_BAUDRATE)
    parser.add_argument("--test",
                        action="store_true",
                        help="Go to test mode.")

    return parser.parse_args()


def test(args):
    """
    Tiny little test harness
    """
    print "TEST MODE"
    print "XBytes that should be 9:", xbytes_to_word([1, 0 ,0 ,1])
    for row in bitmap_to_stripes([[1, 1, 1, 0],[0,1,1,1],[1,1,1,1],[0,1,1,1],[1,1,0,0],[0,0,1,1]], 4, 6, 3):
        print row
    for row in bitmap_to_stripes(get_bitmap(args.pngfile), 384, 384, 12):
        print row






def main():
    """
    Note: X,Z should already be zeroed, and Y should be off the zero.
    """
    args = configself()
    if args.test:
        return test(args)



    print "INIT SERIAL ", args.serialport, args.baudrate
    the_serial = serial.Serial(args.serialport, args.baudrate, timeout=10)
    print the_serial

    def out(cmd):
        print "WROTE:", the_serial.write("%s\r\n" % cmd), "BYTES"
        result = ""
        while (not "OK" in result) and (not "DATA?" in result):
            if result:
                print result
            result = the_serial.readline()
            print result, len(result)
        return result

    print "INIT PRINTER"
    print out("M114")
    print out("G28 Y")   # Zero the machine
    print out("G92 Y0")  #Zero Y
    print out("G92X0")
    print out("G92Z0")

    print "STRIPES..."
    for stripe in bitmap_to_stripes(
            get_bitmap(args.pngfile), 384, 384, 12):
        #print stripe
        pass

if __name__ == "__main__":
    main()
