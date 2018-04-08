# "Spock" Keyboard

This repo holds schematics and code for the Spock keyboard that was
designed by Wez Furlong and modeled on his hands.

The keyboard is a split ergonomic layout in an ortholinear (straight columns,
aligned to your fingers rather than staggered) arrangement with splayed columns
that more closely match the finger separation when the hand is at rest,
thumb clusters to make use of the strong but underused thumbs and a cluster
for classic inverted-T arrow keys.

The Spock name comes from the finger splay which is reminiscent of the Vulcan
hand gesture.

The initial hardware revision is designed around a prototyping friendly
arrangement, with the PCB set up with a minimized keyboard matrix (saving the
headache of routing the matrix while soldering) that is terminated at header
blocks.  No controller hardware is proscribed, making this a more convenient
physical base for experimenting with different controller and interfaces (USB,
BLE and so on).

## What's in this repo?

The hardware directory holds the kicad pcb defintion, gerber files and plate
definitions suitable for building a sandwich style case.  The PCB and gerber
files meet the DRC requirements for Seeed's PCB fabrication service.  The case
design files are suitable for use with Ponoko's service.

The Spockduino directory holds some basic code for a keyboard firmware that uses
the Adafruit Feather M0 Express controller, and a SparkFun SX1509 breakout to
implement a USB keyboard.

## Keyboard Layout Editor

You can view the layout for the left hand here:

http://www.keyboard-layout-editor.com/#/gists/912c57ebc2bcc78ba9ccc6bbb83db161

## Bill of Materials

Should you decide to build one of these for yourself, you will need:

* 84 x Cherry MX compatible PCB mount key switches (each half requires 42)
* 70 x 1u Cherry MX key caps
* 8 x 1.25u Cherry MX key caps
* 6 x 1.5u Cherry MX key caps
* 84 x 1N4148 signal diodes
* 20 x M3 spacer to hold the case top and bottom apart.  Choose the height
  that meets your taste and internal electroncs selection.
* 20 x M3 nuts
* 20 x M3 screws
* Top and bottom plate.  See hardware/spock-case-ponoko-p2.svg for left+right and top+bottom.
  I recommend using matte acrylic for its appealing combination of weight, appearance and price.
  Note that you don't strictly need the plates if you want to rock a bare PCB, but you should
  take care to avoid shorting the contacts under the PCB.
* 2x 2x12 header block.  Pick this to match your wiring.  I used right angled
  male header block to save on vertical space.
* Microcontroller of your choice.  I'm using an Adafruit Feather M0 Express with
  the Spockduino code.  You should be able to use other Feather boards without
  changing the wiring.
* 1x SX1509 IO Expander
* Misc jumper wires
* 2x Jack and 1x connecting cable with 4+ wires.  Some folks like TRRS audio cables and
  jacks, and others like RJ45 jacks and cables.

note: you can trade two of the 1.25u key caps in the thumb clusters for 1u to make
room to trade two of the other 1.25u caps with 1.5u caps for a total of 72x 1u caps,
4x 1.25u caps and 8x 1.5u caps.

There are no explicitly designed in attachment points for the controller, expander
or jacks so you may want to apply friction, tape or hot glue as suits your taste
and overall assembly.

## Wiring/Assembly

The PCB is reversible and can be used for a left or right hand.  For left hand
assembly you will place the switches and diodes on the side that has the
keyswitch labels (eg: `k01` through `k77`).  For the right hand you will place
them on the other side of the board.

If you are using the top plate, take care to assemble in the order described
below as it can be difficult to go back to correct a missed step once you
start soldering the keyswitches.

This initial rev is designed for prototyping, so I attached header to the Feather
and expander breakout boards and used jumper cables to connect them up.  Play
around with this before you start soldering keyswitches to make sure that you
can fit everything together.

1. Solder the diodes first.  The black bar of the diode must be aligned to the thicker
   white bar.  For the left hand the diodes go in on the side with the `dkXX` labels,
   for the right hand they go on the opposite side.
2. Solder the header block second.  If you are using a plate the header pins must be
   on the other side of the board from the diodes.  If you are rocking a plateless
   build then you may want the header pins on the top side.
3. If using a plate, insert the M3 nuts and spacers now.  The nuts go on the same side
   as the diodes, with the spacers on the other side.
4. Place the plate; it goes on the same side as the diodes and should click in
   and lightly grip on the top of the M3 spacers where they protude through
   the M3 nuts
5. Place the key switches through the plate.  The switch stem should be on the same
   side of the board as the diodes and plate.  The switch should friction fit
   in the PCB.  Take care to check that both of the pins on the underside of
   the switch are protuding from the holds on the reverse of the PCB.  You should
   see the pins in the holes and they may protude by about 1mm.  It is possible to
   fold a pin over when placing it, so you may need to wiggle it out of the plate and pcb and
   straighten up the pin.   Make sure that you check every switch!
6. Satisfy yourself that you can assemble the case and house the controller and expander
   before you go further.
7. Solder in the key switches.  Make sure that you solder both pins on each switch!

The Spockduino code assumes that you connected the pins like this:

### Feather

The Feather is attached to the left hand.

* Pins labelled A0-A5 on the left side are connected to r0-r5 in the header block
* Pins labelled 5-12 on the right side are connected to c0-c6 in the header block
* 3V3, GND, SDA, SDL are connected to the pins with the same names
  on the IO expander.  These 4 lines are intended to be carried over
  the connecting cable between the halves so you may want to connec the
  feather to a TRRS or RJ45 block and do the same on the other half
  for the Expander

### SX 1509

The SX1509 is attached to the right hand.

* Pins 0-5 are connected to r0-r5 in the header block.
* Pins 8-14 are connected to c0-c6 in the header block.
* 3V3, GND, SDA, SDL are connected to the pins with the same names
  on the Feather as noted above.

### Flashing

Use the arduino IDE to upload the spock sketch to the device.  I found
that this often requires a couple of attempts with the Feather M0 Express,
as the bootloader seems to take a little longer to kick in than the uploader
expects.

## Errata

The first and current rev of the PCB design has incorrect labels for the header
block on the back side of the PCB.  The labels on the front side are correct.
In case you are unsure which is the front, the front side of the pcb also has
labels for the switch positions.

## License and Copying

Unless stated in individual files, the contents of this repo are made available
under the terms of the MIT License.
