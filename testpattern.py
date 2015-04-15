#!python
"""TinkerBelle testpattern"""


import png
import numpy
import sys
import argparse
import serial
import time

STEPS_PER_ROW = 12  # Dunno :D
# NOTE THAT THE LOW BITS ARE CLOSER TO THE HEAD!
DEFAULT_BAUDRATE=57600


def get_bitmap(filename):
    r = png.Reader(filename="ringtest.png")
    img = r.asDirect()
    bitmap = numpy.array([[x for x in y] for y in img[2]])
    return bitmap


def bitmap_to_stripes(bitmap, xres, yres, bph):
    for stripe in xrange(yres/bph):
        tmp = list()
        for x in xrange(xres):
            # Anything nonzero is a pixel.
            pout = 0
            for phead in xrange(bph):
                # Also, a neat quirk of python that we can use for simpler dev
                # and that is: True shifted left is the same as 1 shifted left.
                bit = (
                        ((bitmap[(stripe*bph)+((bph-1)-phead)][x])
                            and 1)
                    << phead)
                if bit != 0:
                    print "BIT:",bit
                pout += bit
            #print "POUT:",pout
            tmp.append(phead)
        yield tmp


def out(thestr):
    """
    Wait for the "OK" and send data, but the other way around :)
    """


def configself():
    "FOO"
    if "darwin" in sys.platform.lower():
        defaultserial = "/dev/tty.usbserial-A900F9XQ"
    elif "win" in sys.platform.lower():
        defaultserial="COM4"
    elif "linux" in sys.platform.lower():
        defaultserial="/dev/ttyUSB0"
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("pngfile", metavar="pngfile",
            help="PNG file to load for layer duplication.")
    parser.add_argument("-p" "--serialport", dest="serialport",
            help="Serial port to use.", default=defaultserial)
    parser.add_argument("-b", "--baud",
        dest="baudrate",
        type=int,
        help="set baud rate, default %s" % DEFAULT_BAUDRATE,
        default=DEFAULT_BAUDRATE
    )

    return parser.parse_args()

def main():
    """
    Note: X,Z should already be zeroed, and Y should be off the zero.
    """
    args = configself()
    #print args, args.baudrate
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
